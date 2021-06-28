#include <stdexcept>
#include <optional>

#include "glad/glad.h"

#include "GLFWppWindow.h"

namespace RolUIBackend {

    class _GLFWppWindow {
        friend class GLFWppWindow;

      public:
        void do_char_callback(unsigned int unicode) {
            if (_char_callback)
                _char_callback(unicode);
        }
        void do_cursor_enter_callback(int enter) {
            if (_cursor_enter_callback)
                _cursor_enter_callback(enter);
        }
        void do_cursor_pos_callback(double x, double y) {
            if (_cursor_pos_callback)
                _cursor_pos_callback(x, y);
        }
        void do_mouse_button_callback(int button, int action, int mods) {
            if (_mouse_button_callback)
                _mouse_button_callback(button, action, mods);
        }
        void do_scroll_callback(double x_offset, double y_offset) {
            if (_scroll_callback)
                _scroll_callback(x_offset, y_offset);
        }

      private:
        GLFWppWindow::CharFun _char_callback;
        GLFWppWindow::CursorEnterFun _cursor_enter_callback;
        GLFWppWindow::CursorPosFun _cursor_pos_callback;
        GLFWppWindow::MouseButtonFun _mouse_button_callback;
        GLFWppWindow::ScrollFun _scroll_callback;
    };

    struct _WindowNode {
        uint32_t rc;
        GLFWwindow* glfw_ptr;
        _GLFWppWindow win;
    };

    bool _is_init = false;

    const uint32_t GLFWppWindow::max_window_number;
    _WindowNode _windows[GLFWpp_MAX_WINDOW_NUMBER];

    static void _init();

    static uint32_t _window_hash(GLFWwindow* ptr);

    static std::optional<uint32_t> _find_window_index_eq(GLFWwindow* ptr);
    static std::optional<uint32_t> _find_window_index_null(GLFWwindow* ptr);

    static std::optional<uint32_t> _create_window(size_t w, size_t h, const char* title = "");
    static void _destroy_window(uint32_t handle);

    static void _inc_window_rc(uint32_t handle);
    static void _dec_window_rc(uint32_t handle);

    static GLFWwindow* _get_glfw_window_by_handle(uint32_t handle);
    static _GLFWppWindow* _get_window_by_glfw_window(GLFWwindow* ptr);
    static _GLFWppWindow* _get_window_by_handle(uint32_t handle);

    void _init() {
        if (_is_init) return;

        if (glfwInit() == GLFW_FALSE)
            throw std::runtime_error("glfw initialization error.");

        _is_init = true;
    }

    uint32_t _window_hash(GLFWwindow* ptr) {
        uintptr_t p = (uintptr_t)ptr;
        uint32_t idx = (p / 4) % GLFWpp_MAX_WINDOW_NUMBER;
        return idx;
    }
    std::optional<uint32_t> _find_window_index_null(GLFWwindow* ptr) {
        uint32_t idx = _window_hash(ptr);

        for (uint32_t i = idx; i < GLFWpp_MAX_WINDOW_NUMBER; i++) {
            auto wp = _windows[idx].glfw_ptr;
            if (wp == nullptr) return i;
        }
        return {};
    }
    std::optional<uint32_t> _find_window_index_eq(GLFWwindow* ptr) {
        uint32_t idx = _window_hash(ptr);

        for (uint32_t i = idx; i < GLFWpp_MAX_WINDOW_NUMBER; i++) {
            auto wp = _windows[idx].glfw_ptr;
            if (wp == ptr) return i;
        }
        return {};
    }

    std::optional<uint32_t> _create_window(size_t w, size_t h, const char* title) {
        if (!_is_init) _init();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        auto* _glfw_window = glfwCreateWindow(w, h, title, nullptr, nullptr);
        if (_glfw_window == nullptr)
            throw std::runtime_error("glfw create window error.");

        auto idx_opt = _find_window_index_null(_glfw_window);
        if (idx_opt.has_value() == false) {
            glfwDestroyWindow(_glfw_window);
            return {};
        }

        uint32_t idx = idx_opt.value();

        auto& _w = _windows[idx];
        auto* data = &_w.win;

        _w.glfw_ptr = _glfw_window;
        _w.rc = 0;

        new (data) _GLFWppWindow();

        return idx;
    }

    void _destroy_window(uint32_t handle) {
        if (handle >= GLFWpp_MAX_WINDOW_NUMBER) return;

        auto& _w = _windows[handle];

        if (_w.glfw_ptr != nullptr) {
            glfwDestroyWindow(_w.glfw_ptr);
            _w.win.~_GLFWppWindow();
        }

        _w.glfw_ptr = nullptr;
        _w.rc = 0;
    }

    void _inc_window_rc(uint32_t handle) {
        if (handle >= GLFWpp_MAX_WINDOW_NUMBER) return;
        if (_windows[handle].glfw_ptr == nullptr) return;
        _windows[handle].rc++;
    }
    void _dec_window_rc(uint32_t handle) {
        if (handle >= GLFWpp_MAX_WINDOW_NUMBER) return;
        if (_windows[handle].glfw_ptr == nullptr) return;
        _windows[handle].rc--;

        if (_windows[handle].rc == 0)
            _destroy_window(handle);
    }

    _GLFWppWindow* _get_window_by_glfw_window(GLFWwindow* ptr) {
        if (ptr == nullptr) return nullptr;

        auto idx_opt = _find_window_index_eq(ptr);
        if (idx_opt.has_value() == false) return nullptr;

        uint32_t idx = idx_opt.value();

        return &_windows[idx].win;
    }
    GLFWwindow* _get_glfw_window_by_handle(uint32_t handle) {
        if (handle >= GLFWpp_MAX_WINDOW_NUMBER) return nullptr;
        return _windows[handle].glfw_ptr;
    }
    _GLFWppWindow* _get_window_by_handle(uint32_t handle) {
        if (handle >= GLFWpp_MAX_WINDOW_NUMBER) return nullptr;
        if (_windows[handle].glfw_ptr == nullptr) return nullptr;
        return &_windows[handle].win;
    }

    /* ================================================================ */
    /* ================================================================ */
    /* ================================================================ */

    void GLFWppWindow::_char_callback(GLFWwindow* w, unsigned int unicode) {
        auto* wd = _get_window_by_glfw_window(w);
        if (wd == nullptr) return;

        wd->do_char_callback(unicode);
    }
    void GLFWppWindow::_cursor_enter_callback(GLFWwindow* w, int enter) {
        auto* wd = _get_window_by_glfw_window(w);
        if (wd == nullptr) return;

        wd->do_cursor_enter_callback(enter);
    }
    void GLFWppWindow::_cursor_pos_callback(GLFWwindow* w, double x, double y) {
        auto* wd = _get_window_by_glfw_window(w);
        if (wd == nullptr) return;

        wd->do_cursor_pos_callback(x, y);
    }
    void GLFWppWindow::_mouse_button_callback(GLFWwindow* w, int button, int action, int mods) {
        auto* wd = _get_window_by_glfw_window(w);
        if (wd == nullptr) return;

        wd->do_mouse_button_callback(button, action, mods);
    }
    void GLFWppWindow::_scroll_callback(GLFWwindow* w, double x_offset, double y_offset) {
        auto* wd = _get_window_by_glfw_window(w);
        if (wd == nullptr) return;

        wd->do_scroll_callback(x_offset, y_offset);
    }

    void GLFWppWindow::set_char_callback(CharFun&& callback) {
        GLFWwindow* _glfw_window = _get_glfw_window_by_handle(_glfw_handle);
        _GLFWppWindow* _window = _get_window_by_handle(_glfw_handle);

        if (_glfw_window == nullptr || _window == nullptr) return;

        glfwSetCharCallback(_glfw_window, &GLFWppWindow::_char_callback);
        _window->_char_callback = callback;
    }
    void GLFWppWindow::set_cursor_enter_callback(CursorEnterFun&& callback) {
        GLFWwindow* _glfw_window = _get_glfw_window_by_handle(_glfw_handle);
        _GLFWppWindow* _window = _get_window_by_handle(_glfw_handle);

        if (_glfw_window == nullptr || _window == nullptr) return;

        glfwSetCursorEnterCallback(_glfw_window, &GLFWppWindow::_cursor_enter_callback);
        _window->_cursor_enter_callback = callback;
    }
    void GLFWppWindow::set_cursor_pos_callback(CursorPosFun&& callback) {
        GLFWwindow* _glfw_window = _get_glfw_window_by_handle(_glfw_handle);
        _GLFWppWindow* _window = _get_window_by_handle(_glfw_handle);

        if (_glfw_window == nullptr || _window == nullptr) return;

        glfwSetCursorPosCallback(_glfw_window, &GLFWppWindow::_cursor_pos_callback);
        _window->_cursor_pos_callback = callback;
    }
    void GLFWppWindow::set_mouse_button_callback(MouseButtonFun&& callback) {
        GLFWwindow* _glfw_window = _get_glfw_window_by_handle(_glfw_handle);
        _GLFWppWindow* _window = _get_window_by_handle(_glfw_handle);

        if (_glfw_window == nullptr || _window == nullptr) return;

        glfwSetMouseButtonCallback(_glfw_window, &GLFWppWindow::_mouse_button_callback);
        _window->_mouse_button_callback = callback;
    }
    void GLFWppWindow::set_scroll_callback(ScrollFun&& callback) {
        GLFWwindow* _glfw_window = _get_glfw_window_by_handle(_glfw_handle);
        _GLFWppWindow* _window = _get_window_by_handle(_glfw_handle);

        if (_glfw_window == nullptr || _window == nullptr) return;

        glfwSetScrollCallback(_glfw_window, &GLFWppWindow::_scroll_callback);
        _window->_scroll_callback = callback;
    }
    /* ================================================================ */
    /* ================================================================ */
    /* ================================================================ */

    GLFWppWindow::GLFWppWindow(size_t w, size_t h, const char* title) {
        auto handle_opt = _create_window(w, h, title);
        if (handle_opt.has_value() == false)
            throw std::runtime_error("unable to create window.");

        _glfw_handle = handle_opt.value();
    }

    GLFWppWindow::GLFWppWindow(const GLFWppWindow& w) {
        _glfw_handle = w._glfw_handle;
        _inc_window_rc(_glfw_handle);
    }
    GLFWppWindow::GLFWppWindow(GLFWppWindow&& w) {
        _glfw_handle = w._glfw_handle;
        _inc_window_rc(_glfw_handle);
    }

    GLFWppWindow::~GLFWppWindow() {
        _dec_window_rc(_glfw_handle);
    }

    GLFWppWindow& GLFWppWindow::operator=(const GLFWppWindow& w) {
        this->~GLFWppWindow();
        _glfw_handle = w._glfw_handle;
        _inc_window_rc(_glfw_handle);
        return *this;
    }
    GLFWppWindow& GLFWppWindow::operator=(GLFWppWindow&& w) {
        this->~GLFWppWindow();
        _glfw_handle = w._glfw_handle;
        _inc_window_rc(_glfw_handle);
        return *this;
    }

    void GLFWppWindow::make_opengl_context() {
        auto* _glfw_window = _get_glfw_window_by_handle(_glfw_handle);

        glfwMakeContextCurrent(_glfw_window);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            throw std::runtime_error("glfw make OpenGL context error");
    }

    void GLFWppWindow::set_title(const char* title) {
        auto* _glfw_window = _get_glfw_window_by_handle(_glfw_handle);
        glfwSetWindowTitle(_glfw_window, title);
    }
    void GLFWppWindow::set_size(int w, int h) {
        auto* _glfw_window = _get_glfw_window_by_handle(_glfw_handle);
        glfwSetWindowSize(_glfw_window, w, h);
    }

    size_t GLFWppWindow::width() {
        auto* _glfw_window = _get_glfw_window_by_handle(_glfw_handle);

        int w, h;
        glfwGetWindowSize(_glfw_window, &w, &h);
        return w;
    }
    size_t GLFWppWindow::height() {
        auto* _glfw_window = _get_glfw_window_by_handle(_glfw_handle);

        int w, h;
        glfwGetWindowSize(_glfw_window, &w, &h);
        return h;
    }

    bool GLFWppWindow::should_close() {
        auto* _glfw_window = _get_glfw_window_by_handle(_glfw_handle);

        return glfwWindowShouldClose(_glfw_window) == GLFW_TRUE;
    }

    void GLFWppWindow::poll_events() { glfwPollEvents(); }
    void GLFWppWindow::wait_events() { glfwWaitEvents(); }

    void GLFWppWindow::hide() {
        auto* _glfw_window = _get_glfw_window_by_handle(_glfw_handle);

        glfwHideWindow(_glfw_window);
    }
    void GLFWppWindow::show() {
        auto* _glfw_window = _get_glfw_window_by_handle(_glfw_handle);

        glfwShowWindow(_glfw_window);
    }

    void GLFWppWindow::swap_buffer() {
        auto* _glfw_window = _get_glfw_window_by_handle(_glfw_handle);

        glfwSwapBuffers(_glfw_window);
    }

    void GLFWppWindow::close() {
        auto* _glfw_window = _get_glfw_window_by_handle(_glfw_handle);

        glfwSetWindowShouldClose(_glfw_window, GLFW_TRUE);
    }

    void GLFWppWindow::run() {
        while (should_close() == false) {

            poll_events();
            swap_buffer();
        }
    }

} // namespace RolUIBackend
