
#include <cstdint>
#include <optional>
#include <exception>
#include <stdexcept>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "GLFWppWindow.h"

namespace RolUIGLFW {

    static bool _is_init = false;

    static bool _init() {
        if (_is_init) return true;

        if (glfwInit() == GLFW_FALSE)
            return false;
        _is_init = true;
        return true;
    }

    static std::optional<GLFWwindow*> _create_window(size_t w, size_t h, const char* title = "") {
        if (!_is_init) _init();
        if (!_is_init) return {};

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow* _glfw_window = glfwCreateWindow(w, h, title, nullptr, nullptr);
        return _glfw_window ? _glfw_window : std::optional<GLFWwindow*>();
    }

    static void _destroy_window(GLFWwindow* w) {
        if (w) glfwDestroyWindow(w);
    }

    /* ================================================================ */

    using namespace _details;

    void GLFWppWindowBase::_exit_callback(GLFWwindow* w) {
        GLFWppWindow* wd = (GLFWppWindow*)glfwGetWindowUserPointer(w);
        if (wd == nullptr || !wd->on_exit) return;
        wd->on_exit();
    }
    void GLFWppWindowBase::_size_callback(GLFWwindow* win, int w, int h) {
        GLFWppWindow* wd = (GLFWppWindow*)glfwGetWindowUserPointer(win);
        if (wd == nullptr || !wd->on_size) return;
        wd->on_size(w, h);
    }
    void GLFWppWindowBase::_char_callback(GLFWwindow* w, unsigned int unicode) {
        GLFWppWindow* wd = (GLFWppWindow*)glfwGetWindowUserPointer(w);
        if (wd == nullptr || !wd->on_char) return;
        wd->on_char(unicode);
    }
    void GLFWppWindowBase::_cursor_enter_callback(GLFWwindow* w, int enter) {
        GLFWppWindow* wd = (GLFWppWindow*)glfwGetWindowUserPointer(w);
        if (wd == nullptr) return;
        if (enter && wd->on_cursor_enter)
            wd->on_cursor_enter();
        else if (!enter && wd->on_cursor_leave)
            wd->on_cursor_leave();
    }
    void GLFWppWindowBase::_cursor_pos_callback(GLFWwindow* w, double x, double y) {
        GLFWppWindow* wd = (GLFWppWindow*)glfwGetWindowUserPointer(w);
        if (wd == nullptr || !wd->on_cursor_pos) return;
        wd->on_cursor_pos(x, y);
    }
    void GLFWppWindowBase::_mouse_button_callback(GLFWwindow* w, int button, int action, int mods) {
        GLFWppWindow* wd = (GLFWppWindow*)glfwGetWindowUserPointer(w);
        if (wd == nullptr || !wd->on_mouse_button) return;
        wd->on_mouse_button(button, action, mods);
    }
    void GLFWppWindowBase::_scroll_callback(GLFWwindow* w, double x_offset, double y_offset) {
        GLFWppWindow* wd = (GLFWppWindow*)glfwGetWindowUserPointer(w);
        if (wd == nullptr || !wd->on_scroll) return;
        wd->on_scroll(x_offset, y_offset);
    }
    void GLFWppWindowBase::_key_callback(GLFWwindow* w, int key, int scancode, int action, int mods) {
        GLFWppWindow* wd = (GLFWppWindow*)glfwGetWindowUserPointer(w);
        if (wd == nullptr || !wd->on_scroll) return;
        wd->on_key(key, scancode, action, mods);
    }

    /* ================================================================ */

    GLFWppWindow::GLFWppWindow(size_t w, size_t h, const char* title) : _glfw_window(nullptr) {
        auto win_opt = _create_window(w, h, title);

        if (!win_opt.has_value())
            throw std::runtime_error("can't create glfw window.");
        GLFWwindow* win = win_opt.value();

        glfwSetWindowCloseCallback(win, &_exit_callback);
        glfwSetWindowSizeCallback(win, &_size_callback);
        glfwSetCharCallback(win, &_char_callback);
        glfwSetCursorEnterCallback(win, &_cursor_enter_callback);
        glfwSetCursorPosCallback(win, &_cursor_pos_callback);
        glfwSetMouseButtonCallback(win, &_mouse_button_callback);
        glfwSetScrollCallback(win, &_scroll_callback);
        glfwSetKeyCallback(win, &_key_callback);

        _glfw_window = win;
        glfwSetWindowUserPointer(_glfw_window, this);
    }

    GLFWppWindow::GLFWppWindow(GLFWppWindow&& w) : _glfw_window(w._glfw_window) {
        w._glfw_window = nullptr;
        if (_glfw_window)
            glfwSetWindowUserPointer(_glfw_window, this);
    }

    GLFWppWindow::~GLFWppWindow() {
        if (!_glfw_window) return;
        _destroy_window(_glfw_window);
    }

    GLFWppWindow& GLFWppWindow::operator=(GLFWppWindow&& w) {
        if (_glfw_window)
            _destroy_window(_glfw_window);

        _glfw_window = w._glfw_window;
        if (_glfw_window)
            glfwSetWindowUserPointer(_glfw_window, this);

        return *this;
    }

    bool GLFWppWindow::make_opengl_context() {
        glfwMakeContextCurrent(_glfw_window);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            return false;
        return true;
    }

    void GLFWppWindow::set_title(const char* title) {
        glfwSetWindowTitle(_glfw_window, title);
    }
    void GLFWppWindow::set_size(int w, int h) {
        glfwSetWindowSize(_glfw_window, w, h);
    }

    std::tuple<int, int> GLFWppWindow::window_pos() const noexcept {
        int x, y;
        glfwGetWindowPos(_glfw_window, &x, &y);
        return {x, y};
    }
    int GLFWppWindow::x() const noexcept {
        auto [x, y] = window_pos();
        return x;
    }
    int GLFWppWindow::y() const noexcept {
        auto [x, y] = window_pos();
        return y;
    }

    std::tuple<uint32_t, uint32_t> GLFWppWindow::window_size() const noexcept {
        int w, h;
        glfwGetWindowSize(_glfw_window, &w, &h);
        return {w, h};
    }
    uint32_t GLFWppWindow::width() const noexcept {
        auto [w, h] = window_size();
        return w;
    }
    uint32_t GLFWppWindow::height() const noexcept {
        auto [w, h] = window_size();
        return h;
    }

    std::tuple<double, double> GLFWppWindow::cursor_pos() const noexcept {
        double x, y;
        glfwGetCursorPos(_glfw_window, &x, &y);
        return {x, y};
    }

    bool GLFWppWindow::should_close() {
        return glfwWindowShouldClose(_glfw_window) == GLFW_TRUE;
    }

    void GLFWppWindow::poll_events() { glfwPollEvents(); }
    void GLFWppWindow::wait_events() { glfwWaitEvents(); }
    void GLFWppWindow::wait_events_timeout(double timeout) { glfwWaitEventsTimeout(timeout); }

    void GLFWppWindow::hide() { glfwHideWindow(_glfw_window); }
    void GLFWppWindow::show() { glfwShowWindow(_glfw_window); }

    void GLFWppWindow::swap_buffer() { glfwSwapBuffers(_glfw_window); }

    void GLFWppWindow::close() { glfwSetWindowShouldClose(_glfw_window, GLFW_TRUE); }

} // namespace RolUIGLFW
