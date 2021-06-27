#pragma once

#include <stdint.h>
#include <optional>
#include <functional>

#include "GLFW/glfw3.h"

#define GLFWpp_MAX_WINDOW_NUMBER 16

namespace RolUIBackend {

    class GLFWppWindow {
      public:
        static const uint32_t max_window_number = GLFWpp_MAX_WINDOW_NUMBER;

      private:
        static void _char_callback(GLFWwindow* w, unsigned int unicode);
        static void _cursor_enter_callback(GLFWwindow* w, int enter);
        static void _cursor_pos_callback(GLFWwindow* w, double x, double y);
        static void _mouse_button_callback(GLFWwindow* w, int button, int action, int mods);
        static void _scroll_callback(GLFWwindow* w, double x_offset, double y_offset);

      public:
        typedef std::function<void(unsigned int)> CharFun;
        typedef std::function<void(int)> CursorEnterFun;
        typedef std::function<void(double, double)> CursorPosFun;
        typedef std::function<void(int, int, int)> MouseButtonFun;
        typedef std::function<void(double, double)> ScrollFun;

        void set_char_callback(CharFun&& callback);
        void set_cursor_enter_callback(CursorEnterFun&& callback);
        void set_cursor_pos_callback(CursorPosFun&& callback);
        void set_mouse_button_callback(MouseButtonFun&& callback);
        void set_scroll_callback(ScrollFun&& callback);

      public:
        GLFWppWindow(size_t w, size_t h, const char* title = "");

        GLFWppWindow(const GLFWppWindow& w);
        GLFWppWindow(GLFWppWindow&& w);

        ~GLFWppWindow();

        GLFWppWindow& operator=(const GLFWppWindow& w);
        GLFWppWindow& operator=(GLFWppWindow&& w);

        void make_opengl_context();

        void poll_events();
        void wait_events();

        void set_title(const char* title);
        void set_size(int w, int h);

        size_t width();
        size_t height();

        bool should_close();

        void hide();
        void show();

        void swap_buffer();

        void close();

        void run();

      private:
        uint32_t _glfw_handle;
    };

} // namespace RolUIBackend
