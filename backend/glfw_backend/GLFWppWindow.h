#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <functional>
#include <tuple>

#include "GLFW/glfw3.h"

namespace RolUIBackend {

    namespace _details {

        class GLFWppWindowBase {
          public:
            typedef std::function<void()> ExitFunc;
            typedef std::function<void(int, int)> SizeFunc;
            typedef std::function<void(unsigned int)> CharFunc;
            typedef std::function<void(int)> CursorEnterFunc;
            typedef std::function<void(double, double)> CursorPosFunc;
            typedef std::function<void(int, int, int)> MouseButtonFunc;
            typedef std::function<void(double, double)> ScrollFunc;

            ExitFunc on_exit;
            SizeFunc on_size;
            CharFunc on_char;
            CursorEnterFunc on_cursor_enter;
            CursorPosFunc on_cursor_pos;
            MouseButtonFunc on_mouse_button;
            ScrollFunc on_scroll;

          protected:
            static void _exit_callback(GLFWwindow* w);
            static void _size_callback(GLFWwindow* win, int w, int h);
            static void _char_callback(GLFWwindow* w, unsigned int unicode);
            static void _cursor_enter_callback(GLFWwindow* w, int enter);
            static void _cursor_pos_callback(GLFWwindow* w, double x, double y);
            static void _mouse_button_callback(GLFWwindow* w, int button, int action, int mods);
            static void _scroll_callback(GLFWwindow* w, double x_offset, double y_offset);
        };

    } // namespace _details

    class GLFWppWindow : public _details::GLFWppWindowBase {
      public:
        GLFWppWindow(size_t w, size_t h, const char* title);

        GLFWppWindow(const GLFWppWindow& w) = delete;
        GLFWppWindow(GLFWppWindow&& w);

        ~GLFWppWindow();

        GLFWppWindow& operator=(const GLFWppWindow& w) = delete;
        GLFWppWindow& operator=(GLFWppWindow&& w);

        bool make_opengl_context();

        void poll_events();
        void wait_events();

        void set_title(const char* title);
        void set_size(int w, int h);

        std::tuple<int, int> window_pos() const noexcept;
        int x() const noexcept;
        int y() const noexcept;

        std::tuple<uint32_t, uint32_t> window_size() const noexcept;
        uint32_t width() const noexcept;
        uint32_t height() const noexcept;

        std::tuple<double, double> cursor_pos() const noexcept;

        bool should_close();

        void hide();
        void show();

        void swap_buffer();

        void close();

      private:
        GLFWwindow* _glfw_window;
    };

} // namespace RolUIBackend
