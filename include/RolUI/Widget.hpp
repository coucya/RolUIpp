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
#include "./sigslot/Signal.hpp"
#include "./sigslot/Slot.hpp"

namespace RolUI {

    class Window;
    class Widget;
    class IPainter;
    class Application;

    namespace widget {
        class SingleChildWidget;
        class MultiChildWidget;
    } // namespace widget

    typedef bool (*EventCallbackFunc)(IEvent*);
    typedef std::function<bool(IEvent*)> EventCallback;

    bool send_event(Widget* w, IEvent* e);

    void set_rect(Widget* w, Rect rect);

    class Widget : public IWidget, public IEventListener, public HasSlot {
        friend class Window;
        friend class Application;

        friend class widget::SingleChildWidget;
        friend class widget::MultiChildWidget;

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

        virtual bool handle_event(IEvent* e) override;

        virtual Widget* get_child_by_pos(Point pos) const noexcept;

        virtual void on_draw(IPainter* painter) override;

        virtual Size perlayout(Constraint constraint) override;

        virtual bool hit_test(Point local_pos) const override;

      private:
        Widget* _parent = nullptr;

        Point _pos;
        Size _size;
    };

} // namespace RolUI
