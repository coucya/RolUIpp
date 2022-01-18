#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <vector>
#include <tuple>

#include "./Point.hpp"
#include "./Rect.hpp"
#include "./Size.hpp"
#include "./IEvent.hpp"
#include "./IWidget.hpp"
#include "./IEventListener.hpp"
#include "./WidgetState.hpp"
#include "./Style.hpp"
#include "./sigslot/Signal.hpp"
#include "./sigslot/Slot.hpp"

namespace RolUI {

    class Window;
    class Widget;
    class IPainter;
    class Application;

    typedef bool (*EventCallbackFunc)(IEvent*);
    typedef std::function<bool(IEvent*)> EventCallback;

    bool send_event(Widget* w, IEvent* e);

    Size perlayout(Widget* w, Constraint constraint);
    void set_rect(Widget* w, Rect rect);

    class Widget : public IWidget, public IEventListener, public HasSlot {
        friend Window;
        friend Application;
        friend Size perlayout(Widget*, Constraint);
        friend void set_rect(Widget* w, Rect rect);

      public:
        Widget() noexcept;
        // Widget(Widget* parent) noexcept;

        Widget(const Widget&) = delete;
        Widget(Widget&&) = delete;

        virtual ~Widget();

        Widget& operator=(const Widget&) = delete;
        Widget& operator=(Widget&&) = delete;

        Point pos() const noexcept;
        Size size() const noexcept;
        Rect rect() const noexcept;

        Point abs_pos() const noexcept;
        Rect abs_rect() const noexcept;

        Widget* parent() const noexcept;

        size_t child_count() const noexcept;
        Widget* get_child(size_t idx) const noexcept;

        void add_child(Widget* w) noexcept;
        void remove_child(Widget* w) noexcept;

        Widget* get_child_by_pos(Point pos) const noexcept;

        size_t add_listener(const EventType* et, EventCallback&& callback);
        void remove_listener(size_t handle);

        bool on_event(IEvent* e) override;

      protected:
        virtual void on_draw(IPainter* painter) override;

        virtual Size perlayout(Constraint constraint) override;

        virtual bool hit_test(Point local_pos) const override;

      private:
        typedef std::vector<Widget*> Childrens;

        Widget* _get_child(size_t idx) const noexcept;
        void _add_child(Childrens::iterator pos, Widget* w) noexcept;
        void _remove_child(Childrens::iterator pos) noexcept;

        Childrens::iterator _child_begin_it() const noexcept;
        Childrens::iterator _child_end_it() const noexcept;
        Childrens::reverse_iterator _child_rbegin_it() const noexcept;
        Childrens::reverse_iterator _child_rend_it() const noexcept;

        Childrens::iterator _find_child_it(Widget* w) const noexcept;

      private:
        Widget* _parent = nullptr;

        Point _pos;
        Size _size;

        Childrens _children;

        typedef std::tuple<const EventType*, EventCallback, size_t> CallbackItem;
        size_t _event_handle = 0;
        std::vector<CallbackItem> _callbacks;
    };

} // namespace RolUI
