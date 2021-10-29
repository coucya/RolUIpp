#pragma once

#include <stdint.h>

#include "RolUI/Point.h"
#include "RolUI/Vector.h"
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

      private:
        void* _nvg_context;
        GLFWPainter _painter;
    };

} // namespace RolUIBackend
