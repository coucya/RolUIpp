#pragma once

#include <stdint.h>

#include "RolUI/Point.hpp"
#include "RolUI/Vector.hpp"
#include "RolUI/Size.hpp"
#include "RolUI/IPainter.hpp"
#include "RolUI-events/MouseEvent.hpp"
#include "RolUI-events/CharEvent.hpp"
#include "RolUI-events/KeyboardEvent.hpp"

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

        RolUI::Point pos() const override;
        RolUI::Size size() const override;

        RolUI::IPainter* painter() override;
        void begin_draw() override;
        void end_draw() override;

        int load_image(const char* filename) noexcept;

        void dispatch_event(double timeout) override;

      private:
        void _init_nanovg();

        void _init_event_callback();

      private:
        void* _nvg_context;
        GLFWPainter _painter;

        RolUI::MouseDispatcher _mouse_dispatcher;
        RolUI::CharEventDispatcher _char_dispatcher;
        RolUI::KeyboardDispatcher _key_dispatcher;
    };

} // namespace RolUIBackend
