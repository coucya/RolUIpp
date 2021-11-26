
#include <cstddef>

#include "RolUI/timer.hpp"
#include "RolUI/Window.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/Application.hpp"

namespace RolUI {

    // Application::Application() {}
    // Application::~Application() {}

    static bool _should_exit = false;
    static Window* _window = nullptr;

    static TimerQueue _timer_queue = {};

    void Application::init(Window* w) noexcept { _window = w; }

    Window* Application::window() noexcept { return _window; }

    size_t Application::set_timeout(TimeoutCallback cb, double duration, void* arg) {
        return _timer_queue.push(cb, duration, arg);
    }
    void Application::remove_timeout(size_t handle) {
        _timer_queue.remove(handle);
    }

    void Application::exit() noexcept { _should_exit = true; }
    void Application::run() noexcept {
        _draw_window();
        double timeout = 0.0;
        while (!_should_exit) {
            _dispatch_event(timeout);
            do {
                timeout = _do_timer();
            } while (timeout == 0.0);

            _draw_window();
        }
    }

    void Application::_draw_window() noexcept {
        if (!_window) return;
        _window->draw();
    }
    void Application::_dispatch_event(double timeout) noexcept {
        if (!_window) return;
        _window->dispatch_event(timeout);
    }

    double Application::_do_timer() noexcept {
        return _timer_queue.do_timer();
    }
} // namespace RolUI
