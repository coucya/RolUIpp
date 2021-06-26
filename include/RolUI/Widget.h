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

    class WidgetEventListener : public IEventListener {
        friend class Widget;

      protected:
        IntrusiveListNode _brother;
    };

    class Widget : public IWidget, public IEventListener {
        friend class Window;

      public:
        typedef IntrusiveView<IntrusiveList::iterator, Widget, IntrusiveListNode> ChlidrenView;

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

        Point pos() override;
        Size size() override;

        void set_pos(const Point& pos) noexcept;
        void set_size(const Size& size) noexcept;

        void set_pos(int32_t x, int32_t y) noexcept;
        void set_size(uint32_t w, uint32_t h) noexcept;

        void set_parent(Widget* widget) noexcept;

        void add_child(Widget* widget) noexcept;
        void remove_child(Widget* widget) noexcept;

        void add_listener(WidgetEventListener* listener) noexcept;
        void remove_Listener(WidgetEventListener* listener) noexcept;

        bool on_event(IEvent* event) override;
        void draw(IPainter* painter) override;

      protected:
        bool event_distribute_to_children(IEvent* event);
        bool event_distribute_to_listener(IEvent* event);

      protected:
        Point _pos;
        Size _size;

        Window* _window;
        Widget* _parent;

        IntrusiveListNode _brother;
        IntrusiveList _children;

        IntrusiveList _listeners;

      private:
        void _set_window(Window* w) noexcept;
        void _set_window_for_chilren(Window* w);
    };

} // namespace RolUI
