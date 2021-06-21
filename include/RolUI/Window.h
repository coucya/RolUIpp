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

        void draw() override;

      protected:
        Widget* _widget;
    };

} // namespace RolUI
