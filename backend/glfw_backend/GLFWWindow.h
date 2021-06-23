#pragma once

#include <stdint.h>
#include <unordered_map>

#include "GLFW/glfw3.h"

#include "RolUI/Window.h"

namespace RolUIBackend {

    class GLFWWindow : public RolUI::Window {
      private:
        static bool _is_init;

        static void _init();

      public:
        GLFWWindow(size_t w, size_t h, const char* title = "");

        GLFWWindow(const GLFWWindow&) = delete;
        GLFWWindow(GLFWWindow&&) = default;

        ~GLFWWindow() override;

        GLFWWindow& operator=(const GLFWWindow&) = delete;
        GLFWWindow& operator=(GLFWWindow&&) = default;

        void make_opengl_context();
        void init_nanovg();

        void poll_events();
        void wait_events();

        // void set_title(const char* title);
        // void set_size(int w, int h);

        size_t width();
        size_t height();

        bool should_close();

        void hide();
        void show();

        void display();

        void close();

        void run();

        RolUI::IPainter* painter() override;

      private:
        GLFWwindow* _glfw_window;
        void* nvg_context;
    };

} // namespace RolUIBackend
