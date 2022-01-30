
#include <cstddef>

#include "RolUI/timer.hpp"
#include "RolUI/Window.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/IPainter.hpp"
#include "RolUI/Application.hpp"

namespace RolUI {

    // Application::Application() {}
    // Application::~Application() {}

    static bool _should_exit = false;
    static Window* _window = nullptr;
    static Widget* _content_widget = nullptr;
    static Widget* _focus_widget = nullptr;

    static TimerQueue _timer_queue = {};

    void Application::init(Window* w) noexcept { _window = w; }

    Window* Application::window() noexcept { return _window; }

    void Application::set_content_widget(Widget* w) noexcept { _content_widget = w; }
    Widget* Application::content_widget() noexcept { return _content_widget; }

    Widget* Application::get_widget_by_pos(Point pos) noexcept {
        if (_content_widget == nullptr) return nullptr;

        Widget* widget = nullptr;
        Widget* w_it = _content_widget;
        Point pos_it = pos - _content_widget->pos();

        if (w_it->hit_test(pos_it) == false) return nullptr;

        while (true) {
            Widget* w = w_it->get_child_by_pos(pos_it);
            if (w == nullptr) return w_it;
            w_it = w;
            pos_it = pos_it - w_it->pos();
        }
        return widget;
    }

    size_t Application::set_timeout(TimeoutCallback cb, double duration, void* arg) {
        return _timer_queue.push(cb, duration, arg);
    }
    void Application::remove_timeout(size_t handle) {
        _timer_queue.remove(handle);
    }

    void Application::exit() noexcept { _should_exit = true; }
    void Application::run() noexcept {

        double timeout = 0.0;

        layout();
        _draw_window();

        while (!_should_exit) {
            _dispatch_event(timeout);
            timeout = _do_timer();
            layout();
            _draw_window();
        }
    }

    void Application::layout() noexcept {
        if (content_widget()) {
            Size s = content_widget()->perlayout({{0, 0}, _window->size()});
            RolUI::set_rect(content_widget(), Rect{{0, 0}, s});
        }
    }

    void Application::run(Widget* w) noexcept {
        set_content_widget(w);
        run();
    }

    void Application::_draw_window() noexcept {
        if (!_window) return;
        Widget* root_widget = _content_widget;
        IPainter* painter = _window->painter();

        if (root_widget == nullptr) return;
        if (painter == nullptr) return;

        _window->begin_draw();
        painter->scissor(Rect{Point(), _window->size()});
        root_widget->on_draw(painter);
        _window->end_draw();
    }
    void Application::_dispatch_event(double timeout) noexcept {
        if (!_window) return;
        _window->dispatch_event(timeout);
    }

    double Application::_do_timer() noexcept {
        double timeout = 0.0;
        do {
            timeout = _timer_queue.do_timer();
        } while (timeout == 0.0);
        return timeout;
    }
} // namespace RolUI
