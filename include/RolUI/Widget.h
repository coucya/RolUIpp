#pragma once

#include <stdint.h>

#include "Size.h"
#include "Point.h"
#include "IWidget.h"
#include "IEvent.h"
#include "IEventListener.h"
#include "IntrusiveList.h"

namespace RolUI {

    class Widget;
    class Window;

    class WidgetListener : public IEventListener {
      public:
        typedef bool (*CallbackFun)(IEvent* e);

      public:
        static constexpr uint32_t max_linstener_number = 64;

      public:
        WidgetListener() noexcept;
        ~WidgetListener();

        void add_listener(EventType et, CallbackFun cb);
        void remove_listener(EventType et, CallbackFun cb);

        virtual bool on_event(IEvent* e) override;

      private:
        struct _ListenerNode {
            IntrusiveListNode brother;
            EventType event_type;
            CallbackFun callback;

            _ListenerNode() noexcept : event_type(), callback(nullptr) {}
            _ListenerNode(EventType et, CallbackFun cb) noexcept
                : event_type(et), callback(cb) {}
            void clear() { *this = {}; }
        };

        IntrusiveList<_ListenerNode> _free_list;
        IntrusiveList<_ListenerNode> _in_use_list;
        _ListenerNode _linsteners[max_linstener_number];
    };

    class Widget : public IWidget, public WidgetListener {
        friend class Window;

      public:
        typedef IntrusiveView<IntrusivePrimeList::iterator, Widget, IntrusiveListNode> ChlidrenView;

      public:
        Widget() noexcept : _parent(nullptr), _window(nullptr) {}
        Widget(Widget* parent) noexcept;

        Widget(const Widget&) = delete;
        Widget(Widget&&) = delete;

        virtual ~Widget();

        Widget& operator=(const Widget&) = delete;
        Widget& operator=(Widget&&) = delete;

        Widget* parent() noexcept { return _parent; }
        const Widget* parent() const noexcept { return _parent; }

        Window* window() noexcept { return _window; }
        const Window* window() const noexcept { return _window; }

        ChlidrenView children_view() noexcept;
        const ChlidrenView children_view() const noexcept;

        ChlidrenView children_view_reverse() noexcept;
        const ChlidrenView children_view_reverse() const noexcept;

        Point pos() override;
        Size size() override;

        void set_pos(const Point& pos) noexcept;
        void set_size(const Size& size) noexcept;

        void set_pos(int32_t x, int32_t y) noexcept;
        void set_size(uint32_t w, uint32_t h) noexcept;

        void set_parent(Widget* widget) noexcept;

        void add_child(Widget* widget) noexcept;
        void remove_child(Widget* widget) noexcept;

        void draw(IPainter* painter) override;

      protected:
        Point _pos;
        Size _size;

        Window* _window;
        Widget* _parent;

        IntrusiveListNode _brother;
        IntrusivePrimeList _children;

      private:
        void _set_window(Window* w) noexcept;
        void _set_window_for_chilren(Window* w);
    };

} // namespace RolUI
