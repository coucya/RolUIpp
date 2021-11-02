#pragma once

#include <stdint.h>

#include "RolUI/IDisplay.hpp"
#include "RolUI/IDispatcher.hpp"
#include "RolUI/IEvent.hpp"
#include "RolUI/Point.hpp"

namespace RolUI {

    class Widget;

    class Window : public IDisplay, public IDispatcher {
      protected:
      public:
        Window() noexcept {}

        ~Window() override {}

        Widget* content_widget() const noexcept;
        void set_content_widget(Widget* w) noexcept;

        IPainter* painter() override;
        void begin_draw() override;
        void end_draw() override;

        void draw() noexcept;

        void dispatch_event() override;

        bool send_event(Widget* w, IEvent* e);

        Widget* get_widget_by_pos(Point pos) const noexcept;

      private:
        void _draw_widget(RolUI::Widget* widget, RolUI::IPainter* painter) noexcept;

      protected:
        Widget* _content_widget = nullptr;
    };

} // namespace RolUI
