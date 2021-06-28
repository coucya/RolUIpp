#pragma once

#include <stdint.h>

#include "RolUI/Point.h"
#include "RolUI/vector.h"
#include "RolUI/Window.h"
#include "RolUI/events/MouseEvent.h"

#include "GLFWPainter.h"
#include "GLFWppWindow.h"

namespace RolUIBackend {

    class GLFWWindow : public GLFWppWindow, public RolUI::Window {
      private:
      public:
        GLFWWindow(uint32_t w, uint32_t h, const char* title = "");

        GLFWWindow(const GLFWWindow&) = delete;
        GLFWWindow(GLFWWindow&&) = delete;

        ~GLFWWindow();

        GLFWWindow& operator=(const GLFWWindow&) = delete;
        GLFWWindow& operator=(GLFWWindow&&) = delete;

        RolUI::IPainter* painter() override;
        void draw() override;

        void run();

      private:
        void _init_nanovg();

        bool _distribute_mouse_event_to_widget(RolUI::Widget* w, RolUI::Point widget_pos);
        void _distribute_mouse_event();

      private:
        struct _MouseState {
            RolUI::Point last_pos;
            RolUI::Point current_pos;
        };

      private:
        void* _nvg_context;
        GLFWPainter _painter;

        _MouseState _mouse_state;
    };

} // namespace RolUIBackend
