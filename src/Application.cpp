
#include "RolUI/Window.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/Application.hpp"

namespace RolUI {

    Application::Application() {}
    Application::~Application() {}

    void Application::set_window(Window* w) noexcept {
        _window = w;
        _window->_application = this;
    }
    void Application::remove_window(Window* w) noexcept {
        if (_window == w) {
            _window = nullptr;
            w->_application = nullptr;
        }
    }

    void Application::exit() noexcept { _should_exit = true; }
    void Application::run() noexcept {
        _draw_window();
        while (!_should_exit) {
            _dispatch_event();
            _draw_window();
        }
    }

    void Application::_draw_window() noexcept {
        if (!_window) return;
        _window->draw();
    }
    void Application::_dispatch_event() noexcept {
        if (!_window) return;
        _window->dispatch_event();
    }

} // namespace RolUI
