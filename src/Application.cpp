
#include <chrono>
#include <cstddef>

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

    size_t Application::set_timeout(TimeoutCallback cb, double duration, void* arg) {
        using namespace std::chrono;

        long long current_time = duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
        unsigned long long target_time = current_time + (duration <= 0.0 ? 0ull : (unsigned long long)duration * 1000000);

        return _timer_queue.push(cb, target_time, arg);
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
        using namespace std::chrono;

        double timeout = 60.0;
        unsigned long long tolerance = 10000; // 0.010s.
        while (!_timer_queue.empty()) {
            TimerTask tt = _timer_queue.top();
            unsigned long long current_time = duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();

            if (tt.trigger_time > current_time && current_time - tt.trigger_time > tolerance) {
                timeout = (double)(tt.trigger_time - current_time) / 1000000.0;
                break;
            }

            if (tt.callback)
                tt.callback(tt.arg);
            _timer_queue.pop();
        }
        return std::max(0.0, timeout - tolerance / 1000000.0);
    }
} // namespace RolUI
