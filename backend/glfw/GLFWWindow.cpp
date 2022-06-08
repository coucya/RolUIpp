
#include <cstdint>
#include <chrono>

#include "glad/glad.h"

#include "nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg_gl.h"

#include "stb_image.h"

#include "GLFWWindow.h"

#include "RolUI/Application.hpp"

namespace RolUIGLFW {

    static double now() noexcept {
        using namespace std;
        auto d = chrono::steady_clock::now().time_since_epoch();
        return chrono::duration_cast<chrono::duration<double>>(d).count();
    }

    static RolUI::MouseKey glfw_map_to_rolui_mouse_key(int glfw_mouse_key) {
        RolUI::MouseKey key = RolUI::MouseKey::unkown;
        switch (glfw_mouse_key) {
            case GLFW_MOUSE_BUTTON_1: key = RolUI::MouseKey::key1; break;
            case GLFW_MOUSE_BUTTON_2: key = RolUI::MouseKey::key2; break;
            case GLFW_MOUSE_BUTTON_3: key = RolUI::MouseKey::key3; break;
            case GLFW_MOUSE_BUTTON_4: key = RolUI::MouseKey::key4; break;
            case GLFW_MOUSE_BUTTON_5: key = RolUI::MouseKey::key5; break;
            case GLFW_MOUSE_BUTTON_6: key = RolUI::MouseKey::key6; break;
            case GLFW_MOUSE_BUTTON_7: key = RolUI::MouseKey::key7; break;
            case GLFW_MOUSE_BUTTON_8: key = RolUI::MouseKey::key8; break;
        };
        return key;
    }
    static RolUI::KeyboardKey glfw_map_to_rolui_keyboard_key(int glfw_keyboard_key) {
        RolUI::KeyboardKey key = RolUI::KeyboardKey::unknown;

        switch (glfw_keyboard_key) {
            case GLFW_KEY_SPACE: key = RolUI::KeyboardKey::space; break;
            case GLFW_KEY_APOSTROPHE: key = RolUI::KeyboardKey::apostrophe; break;
            case GLFW_KEY_COMMA: key = RolUI::KeyboardKey::comma; break;
            case GLFW_KEY_MINUS: key = RolUI::KeyboardKey::minus; break;
            case GLFW_KEY_PERIOD: key = RolUI::KeyboardKey::period; break;
            case GLFW_KEY_SLASH: key = RolUI::KeyboardKey::slash; break;
            case GLFW_KEY_0: key = RolUI::KeyboardKey::_0; break;
            case GLFW_KEY_1: key = RolUI::KeyboardKey::_1; break;
            case GLFW_KEY_2: key = RolUI::KeyboardKey::_2; break;
            case GLFW_KEY_3: key = RolUI::KeyboardKey::_3; break;
            case GLFW_KEY_4: key = RolUI::KeyboardKey::_4; break;
            case GLFW_KEY_5: key = RolUI::KeyboardKey::_5; break;
            case GLFW_KEY_6: key = RolUI::KeyboardKey::_6; break;
            case GLFW_KEY_7: key = RolUI::KeyboardKey::_7; break;
            case GLFW_KEY_8: key = RolUI::KeyboardKey::_8; break;
            case GLFW_KEY_9: key = RolUI::KeyboardKey::_9; break;
            case GLFW_KEY_SEMICOLON: key = RolUI::KeyboardKey::semicolon; break;
            case GLFW_KEY_EQUAL: key = RolUI::KeyboardKey::equal; break;
            case GLFW_KEY_A: key = RolUI::KeyboardKey::a; break;
            case GLFW_KEY_B: key = RolUI::KeyboardKey::b; break;
            case GLFW_KEY_C: key = RolUI::KeyboardKey::c; break;
            case GLFW_KEY_D: key = RolUI::KeyboardKey::d; break;
            case GLFW_KEY_E: key = RolUI::KeyboardKey::e; break;
            case GLFW_KEY_F: key = RolUI::KeyboardKey::f; break;
            case GLFW_KEY_G: key = RolUI::KeyboardKey::g; break;
            case GLFW_KEY_H: key = RolUI::KeyboardKey::h; break;
            case GLFW_KEY_I: key = RolUI::KeyboardKey::i; break;
            case GLFW_KEY_J: key = RolUI::KeyboardKey::j; break;
            case GLFW_KEY_K: key = RolUI::KeyboardKey::k; break;
            case GLFW_KEY_L: key = RolUI::KeyboardKey::l; break;
            case GLFW_KEY_M: key = RolUI::KeyboardKey::m; break;
            case GLFW_KEY_N: key = RolUI::KeyboardKey::n; break;
            case GLFW_KEY_O: key = RolUI::KeyboardKey::o; break;
            case GLFW_KEY_P: key = RolUI::KeyboardKey::p; break;
            case GLFW_KEY_Q: key = RolUI::KeyboardKey::q; break;
            case GLFW_KEY_R: key = RolUI::KeyboardKey::r; break;
            case GLFW_KEY_S: key = RolUI::KeyboardKey::s; break;
            case GLFW_KEY_T: key = RolUI::KeyboardKey::t; break;
            case GLFW_KEY_U: key = RolUI::KeyboardKey::u; break;
            case GLFW_KEY_V: key = RolUI::KeyboardKey::v; break;
            case GLFW_KEY_W: key = RolUI::KeyboardKey::w; break;
            case GLFW_KEY_X: key = RolUI::KeyboardKey::x; break;
            case GLFW_KEY_Y: key = RolUI::KeyboardKey::y; break;
            case GLFW_KEY_Z: key = RolUI::KeyboardKey::z; break;
            case GLFW_KEY_LEFT_BRACKET: key = RolUI::KeyboardKey::left_bracket; break;
            case GLFW_KEY_BACKSLASH: key = RolUI::KeyboardKey::backslash; break;
            case GLFW_KEY_RIGHT_BRACKET: key = RolUI::KeyboardKey::right_bracket; break;
            case GLFW_KEY_GRAVE_ACCENT: key = RolUI::KeyboardKey::grave_accent; break;
            case GLFW_KEY_ESCAPE: key = RolUI::KeyboardKey::escape; break;
            case GLFW_KEY_ENTER: key = RolUI::KeyboardKey::enter; break;
            case GLFW_KEY_TAB: key = RolUI::KeyboardKey::tab; break;
            case GLFW_KEY_BACKSPACE: key = RolUI::KeyboardKey::backspace; break;
            case GLFW_KEY_INSERT: key = RolUI::KeyboardKey::insert; break;
            case GLFW_KEY_DELETE: key = RolUI::KeyboardKey::delete_; break;
            case GLFW_KEY_RIGHT: key = RolUI::KeyboardKey::right; break;
            case GLFW_KEY_LEFT: key = RolUI::KeyboardKey::left; break;
            case GLFW_KEY_DOWN: key = RolUI::KeyboardKey::down; break;
            case GLFW_KEY_UP: key = RolUI::KeyboardKey::up; break;
            case GLFW_KEY_PAGE_UP: key = RolUI::KeyboardKey::page_up; break;
            case GLFW_KEY_PAGE_DOWN: key = RolUI::KeyboardKey::page_down; break;
            case GLFW_KEY_HOME: key = RolUI::KeyboardKey::home; break;
            case GLFW_KEY_END: key = RolUI::KeyboardKey::end; break;
            case GLFW_KEY_CAPS_LOCK: key = RolUI::KeyboardKey::caps_lock; break;
            case GLFW_KEY_SCROLL_LOCK: key = RolUI::KeyboardKey::scroll_lock; break;
            case GLFW_KEY_NUM_LOCK: key = RolUI::KeyboardKey::num_lock; break;
            case GLFW_KEY_PRINT_SCREEN: key = RolUI::KeyboardKey::print_screen; break;
            case GLFW_KEY_PAUSE: key = RolUI::KeyboardKey::pause; break;
            case GLFW_KEY_F1: key = RolUI::KeyboardKey::f1; break;
            case GLFW_KEY_F2: key = RolUI::KeyboardKey::f2; break;
            case GLFW_KEY_F3: key = RolUI::KeyboardKey::f3; break;
            case GLFW_KEY_F4: key = RolUI::KeyboardKey::f4; break;
            case GLFW_KEY_F5: key = RolUI::KeyboardKey::f5; break;
            case GLFW_KEY_F6: key = RolUI::KeyboardKey::f6; break;
            case GLFW_KEY_F7: key = RolUI::KeyboardKey::f7; break;
            case GLFW_KEY_F8: key = RolUI::KeyboardKey::f8; break;
            case GLFW_KEY_F9: key = RolUI::KeyboardKey::f9; break;
            case GLFW_KEY_F10: key = RolUI::KeyboardKey::f10; break;
            case GLFW_KEY_F11: key = RolUI::KeyboardKey::f11; break;
            case GLFW_KEY_F12: key = RolUI::KeyboardKey::f12; break;
            case GLFW_KEY_F13: key = RolUI::KeyboardKey::f13; break;
            case GLFW_KEY_F14: key = RolUI::KeyboardKey::f14; break;
            case GLFW_KEY_F15: key = RolUI::KeyboardKey::f15; break;
            case GLFW_KEY_F16: key = RolUI::KeyboardKey::f16; break;
            case GLFW_KEY_F17: key = RolUI::KeyboardKey::f17; break;
            case GLFW_KEY_F18: key = RolUI::KeyboardKey::f18; break;
            case GLFW_KEY_F19: key = RolUI::KeyboardKey::f19; break;
            case GLFW_KEY_F20: key = RolUI::KeyboardKey::f20; break;
            case GLFW_KEY_F21: key = RolUI::KeyboardKey::f21; break;
            case GLFW_KEY_F22: key = RolUI::KeyboardKey::f22; break;
            case GLFW_KEY_F23: key = RolUI::KeyboardKey::f23; break;
            case GLFW_KEY_F24: key = RolUI::KeyboardKey::f24; break;
            case GLFW_KEY_F25: key = RolUI::KeyboardKey::f25; break;
            case GLFW_KEY_KP_0: key = RolUI::KeyboardKey::kp_0; break;
            case GLFW_KEY_KP_1: key = RolUI::KeyboardKey::kp_1; break;
            case GLFW_KEY_KP_2: key = RolUI::KeyboardKey::kp_2; break;
            case GLFW_KEY_KP_3: key = RolUI::KeyboardKey::kp_3; break;
            case GLFW_KEY_KP_4: key = RolUI::KeyboardKey::kp_4; break;
            case GLFW_KEY_KP_5: key = RolUI::KeyboardKey::kp_5; break;
            case GLFW_KEY_KP_6: key = RolUI::KeyboardKey::kp_6; break;
            case GLFW_KEY_KP_7: key = RolUI::KeyboardKey::kp_7; break;
            case GLFW_KEY_KP_8: key = RolUI::KeyboardKey::kp_8; break;
            case GLFW_KEY_KP_9: key = RolUI::KeyboardKey::kp_9; break;
            case GLFW_KEY_KP_DECIMAL: key = RolUI::KeyboardKey::kp_decimal; break;
            case GLFW_KEY_KP_DIVIDE: key = RolUI::KeyboardKey::kp_divide; break;
            case GLFW_KEY_KP_MULTIPLY: key = RolUI::KeyboardKey::kp_multiply; break;
            case GLFW_KEY_KP_SUBTRACT: key = RolUI::KeyboardKey::kp_subtract; break;
            case GLFW_KEY_KP_ADD: key = RolUI::KeyboardKey::kp_add; break;
            case GLFW_KEY_KP_ENTER: key = RolUI::KeyboardKey::kp_enter; break;
            case GLFW_KEY_KP_EQUAL: key = RolUI::KeyboardKey::kp_equal; break;
            case GLFW_KEY_LEFT_SHIFT: key = RolUI::KeyboardKey::left_shift; break;
            case GLFW_KEY_LEFT_CONTROL: key = RolUI::KeyboardKey::left_control; break;
            case GLFW_KEY_LEFT_ALT: key = RolUI::KeyboardKey::left_alt; break;
            case GLFW_KEY_LEFT_SUPER: key = RolUI::KeyboardKey::left_super; break;
            case GLFW_KEY_RIGHT_SHIFT: key = RolUI::KeyboardKey::right_shift; break;
            case GLFW_KEY_RIGHT_CONTROL: key = RolUI::KeyboardKey::right_control; break;
            case GLFW_KEY_RIGHT_ALT: key = RolUI::KeyboardKey::right_alt; break;
            case GLFW_KEY_RIGHT_SUPER: key = RolUI::KeyboardKey::right_super; break;
            case GLFW_KEY_MENU: key = RolUI::KeyboardKey::menu; break;
        }

        return key;
    }

    GLFWWindow::GLFWWindow(uint32_t w, uint32_t h, const char* title)
        : GLFWppWindow(w, h, title), _painter(nullptr) {
        make_opengl_context();
        _init_nanovg();
        _init_event_callback();
    }

    GLFWWindow::~GLFWWindow() {}

    void GLFWWindow::_init_nanovg() {
        _nvg_context = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
        _painter = GLFWPainter(_nvg_context);
    }
    void GLFWWindow::_init_event_callback() {
        on_cursor_enter = [this]() {
            auto [mx, my] = this->cursor_pos();
            this->_mouse_dispatcher.set_pos({(int)mx, (int)my});
            this->_mouse_dispatcher.set_last_pos({(int)mx, (int)my});
            this->_mouse_dispatcher.enter();
        };
        on_cursor_leave = [this]() {
            auto [mx, my] = this->cursor_pos();
            this->_mouse_dispatcher.set_pos({(int)mx, (int)my});
            this->_mouse_dispatcher.leave();
        };
        on_cursor_pos = [this](double x, double y) {
            this->_mouse_dispatcher.set_pos({(int)x, (int)y});
        };
        on_mouse_button = [this](int button, int action, int mods) {
            RolUI::MouseKey key = glfw_map_to_rolui_mouse_key(button);
            RolUI::MouseKeyMode md;
            switch (action) {
                case GLFW_PRESS: md = RolUI::MouseKeyMode::press; break;
                case GLFW_REPEAT: md = RolUI::MouseKeyMode::press; break;
                case GLFW_RELEASE: md = RolUI::MouseKeyMode::release; break;
            };
            this->_mouse_dispatcher.set_key_mode(key, md);
        };

        on_size = [this](int w, int h) {
            glViewport(0, 0, w, h);
            on_size_change.emit({w, h});
        };

        on_char = [this](unsigned int c) {
            _char_dispatcher.push_char(c);
        };

        on_scroll = [this](double x, double y) {
            this->_mouse_dispatcher.set_wheel_offset({int(x), int(y)});
        };

        on_key = [this](int key, int scancode, int action, int mods) {
            RolUI::KeyboardKey rkey = glfw_map_to_rolui_keyboard_key(key);
            RolUI::KeyboardKeyMode md;
            switch (action) {
                case GLFW_PRESS: md = RolUI::KeyboardKeyMode::press; break;
                case GLFW_REPEAT: md = RolUI::KeyboardKeyMode::press; break;
                case GLFW_RELEASE: md = RolUI::KeyboardKeyMode::release; break;
            };
            _key_dispatcher.set_key_mode(rkey, md);
        };
    }

    RolUI::Point GLFWWindow::pos() const {
        auto [x, y] = window_pos();
        return {x, y};
    }
    RolUI::Size GLFWWindow::size() const {
        auto [w, h] = window_size();
        return {(int)w, (int)h};
    }

    RolUI::IPainter* GLFWWindow::painter() { return &_painter; }

    void GLFWWindow::begin_draw() {
        NVGcontext* vg = (NVGcontext*)_nvg_context;
        if (vg == nullptr) return;

        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        nvgBeginFrame(vg, width(), height(), 1);
    }
    void GLFWWindow::end_draw() {
        NVGcontext* vg = (NVGcontext*)_nvg_context;
        if (vg == nullptr) return;
        nvgEndFrame(vg);
        swap_buffer();
    }

    void GLFWWindow::dispatch_event() {
        this->poll_events();

        _mouse_dispatcher.dispatch(this);
        _key_dispatcher.dispatch(this);
        _char_dispatcher.dispatch(this);
    }

    void GLFWWindow::run() {
        double now_time = now();
        RolUI::Application::flush_frame(now_time);

        while (!Application::should_exit()) {
            dispatch_event();
            now_time = now();
            RolUI::Application::flush_frame(now_time);
        }
    }

    int GLFWWindow::load_image(const char* filename) noexcept {
        int w, h, n;
        unsigned char* data = stbi_load(filename, &w, &h, &n, 4);
        int handle = painter()->create_image(data, w, h);
        stbi_image_free(data);
        return handle;
    }
} // namespace RolUIGLFW
