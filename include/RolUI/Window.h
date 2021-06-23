#pragma once

#include <stdint.h>

#include "IWindow.h"
#include "Widget.h"

namespace RolUI {

    class Window : public IWindow {
      protected:
        Window() {}

      public:
        ~Window() override {}

        Widget* widget() noexcept { return _widget; }
        const Widget* widget() const noexcept { return _widget; }
        void set_widget(Widget* widget) noexcept { _widget = widget; }

        IPainter* painter() override;
        void draw() override;

      private:
        void _draw_widget(RolUI::Widget* widget, RolUI::IPainter* painter);

      protected:
        Widget* _widget;
    };

} // namespace RolUI
