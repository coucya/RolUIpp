
#include <cstddef>

#include "RolUI/timer.hpp"
#include "RolUI/Window.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/IPainter.hpp"
#include "RolUI/Application.hpp"

#include "RolUI/events/Widget_event.hpp"

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

    bool Application::has_focus_widget(Widget* w) noexcept { return _focus_widget != nullptr; }
    Widget* Application::focus_widget() noexcept { return _focus_widget; }
    void Application::set_focus_widget(Widget* w) noexcept {
        if (_focus_widget) {
            FocusChangeEvent fce{_focus_widget, false, true};
            send_event(_focus_widget, &fce);
        }

        _focus_widget = w;
        if (_focus_widget) {
            FocusChangeEvent fce{_focus_widget, true, false};
            send_event(_focus_widget, &fce);
        }
    }

    Widget* Application::get_widget_by_pos(Point pos) noexcept {
        if (_content_widget == nullptr) return nullptr;

        Widget* widget = nullptr;
        Widget* w_it = _content_widget;

        while (true) {
            Widget* w = w_it->get_child_by_pos(pos);
            if (w == nullptr) return w_it;
            w_it = w;
        }

        return widget->hit_test(pos) ? widget : nullptr;
    }

    size_t Application::set_timeout(TimeoutCallback cb, double duration, void* arg) {
        return _timer_queue.push(cb, duration, arg);
    }
    void Application::remove_timeout(size_t handle) {
        _timer_queue.remove(handle);
    }

    void Application::exit() noexcept { _should_exit = true; }

    void Application::run(Widget* w) noexcept {
        set_content_widget(w);
        run();
    }
    void Application::run() noexcept {

        double timeout = 0.0;

        flush_frame();

        while (!_should_exit) {
            _dispatch_event(timeout);
            timeout = _do_timer();
            flush_frame();
        }
    }

    void Application::flush_frame() noexcept {
        flush_layout();
        flush_draw();
    }
    void Application::flush_layout() noexcept {
        if (content_widget()) {
            content_widget()->layout({{0, 0}, _window->size()});
            RolUI::set_pos(content_widget(), {0, 0});
        }
        if (content_widget()) {
            content_widget()->update_pos();
        }
    }
    void Application::flush_draw() noexcept {
        if (!_window) return;
        Widget* root_widget = _content_widget;
        IPainter* painter = _window->painter();

        if (root_widget == nullptr) return;
        if (painter == nullptr) return;

        _window->begin_draw();
        painter->scissor(Rect{Point(), _window->size()});
        root_widget->draw(painter);
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
