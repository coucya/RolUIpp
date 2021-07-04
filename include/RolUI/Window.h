#pragma once

#include <stdint.h>

#include "IWindow.h"
#include "events/MouseEvent.h"

namespace RolUI {

    class Widget;

    class Window : public IWindow {
      protected:
        Window() noexcept {}

      public:
        ~Window() override {}

        Widget* widget() noexcept { return _widget; }
        const Widget* widget() const noexcept { return _widget; }
        void set_widget(Widget* widget);

        MouseDispatcher& mouse_dispatcher() noexcept { return _mouse_dispatcher; }

        void distribute_event();

        IPainter* painter() override;
        void draw() override;

      private:
        void _draw_widget(RolUI::Widget* widget, RolUI::IPainter* painter);

      protected:
        Widget* _widget = nullptr;
        MouseDispatcher _mouse_dispatcher;
    };

} // namespace RolUI
