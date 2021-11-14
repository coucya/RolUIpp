#pragma once

#include <stdint.h>

#include "RolUI/IDisplay.hpp"
#include "RolUI/IDispatcher.hpp"
#include "RolUI/IEvent.hpp"
#include "RolUI/Point.hpp"
#include "RolUI/Rect.hpp"
#include "RolUI/Size.hpp"
#include "RolUI/sigslot/Slot.hpp"

namespace RolUI {

    class Application;
    class Widget;

    class Window : public IDisplay, public IDispatcher, public HasSlot {
        friend Application;

      public:
        Window() noexcept;
        ~Window() override;

        Application* application() const noexcept;

        Widget* content_widget() const noexcept;
        void set_content_widget(Widget* w) noexcept;

        IPainter* painter() override;
        void begin_draw() override;
        void end_draw() override;

        void draw() noexcept;

        void dispatch_event(double timeout) override;

        Widget* get_widget_by_pos(Point pos) const noexcept;

      private:
        void _init_event_bind() noexcept;

        void _draw_widget(RolUI::Widget* widget, Point base_pos, Rect scissor, RolUI::IPainter* painter) noexcept;

        void _do_size_change(Size s) noexcept;

      protected:
        Widget* _content_widget = nullptr;

        Application* _application;
    };

} // namespace RolUI
