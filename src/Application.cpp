#include <cstddef>
#include <chrono>
#include <queue>

#include "RolUI/Window.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/IPainter.hpp"
#include "RolUI/Application.hpp"

#include "RolUI/events/Widget_event.hpp"

namespace RolUI {

    struct TimerTask {
        unsigned long long interval;
        unsigned long long start_time;
        unsigned long long trigger_time;
        TimeoutCallback callback;
        bool is_repeat;
        size_t handle;

        bool operator<(const TimerTask& right) const noexcept {
            return trigger_time > right.trigger_time;
        }
        bool operator>(const TimerTask& right) const noexcept {
            return trigger_time < right.trigger_time;
        }
    };

    class TimerQueue : public std::priority_queue<TimerTask> {
      private:
        typedef std::priority_queue<TimerTask> priority_queue;

      public:
        size_t add(TimeoutCallback cb, double interval, bool repeat = false) noexcept;
        void remove(size_t handle) noexcept;
        double do_timer() noexcept;

      private:
        size_t _add(TimerTask&& task) noexcept;

        size_t _timer_handle = 0;
    };

    static bool _should_exit = false;
    static Window* _window = nullptr;
    static Widget* _content_widget = nullptr;
    static Widget* _focus_widget = nullptr;

    static TimerQueue _timer_queue = {};

    void Application::init(Window* w) noexcept { _window = w; }

    Window* Application::window() noexcept { return _window; }

    void Application::set_content_widget(Widget* w) noexcept {
        _content_widget = w;
        if (_content_widget)
            _content_widget->_attach();
    }
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
        if (content_widget() == nullptr) return nullptr;

        Widget* widget = nullptr;
        Widget* w_it = content_widget();

        while (true) {
            Widget* w = w_it->get_child_by_pos(pos);
            if (w == nullptr) return w_it;
            w_it = w;
        }

        return widget->hit_test(pos) ? widget : nullptr;
    }

    size_t Application::set_timeout(double duration, TimeoutCallback cb) {
        return _timer_queue.add(cb, duration, false);
    }
    size_t Application::set_interval(double duration, TimeoutCallback cb) {
        return _timer_queue.add(cb, duration, true);
    }
    void Application::clear_timeout(size_t handle) { _timer_queue.remove(handle); }
    void Application::clear_interval(size_t handle) { _timer_queue.remove(handle); }

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
        Widget* root_widget = content_widget();
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

    static unsigned long long now_microseconds() {
        using namespace std::chrono;
        long long current_time = duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
        return (unsigned long long)current_time;
    }

    size_t TimerQueue::_add(TimerTask&& task) noexcept {
        size_t handle = task.handle;
        push(std::move(task));
        return handle;
    }

    size_t TimerQueue::add(TimeoutCallback cb, double interval, bool repeat) noexcept {
        unsigned long long interval_ = std::max(interval, 0.0) * 1000000.0;
        unsigned long long current_time = now_microseconds();

        TimerTask tt;
        tt.interval = interval_;
        tt.start_time = (unsigned long long)current_time;
        tt.trigger_time = current_time + interval_;
        tt.callback = std::move(cb);
        tt.is_repeat = repeat;
        tt.handle = _timer_handle++;

        return _add(std::move(tt));
    }

    void TimerQueue::remove(size_t handle) noexcept {
        auto it = std::find_if(c.begin(), c.end(), [=](const TimerTask& tt) {
            return tt.handle == handle;
        });
        if (it != c.end()) {
            c.erase(it);
            std::make_heap(c.begin(), c.end());
        }
    }

    double TimerQueue::do_timer() noexcept {
        using namespace std::chrono;

        double timeout = 10.0;

        while (!empty()) {
            const TimerTask& tt = top();

            unsigned long long current_time = now_microseconds();

            if (tt.trigger_time > current_time) {
                timeout = (double)(tt.trigger_time - current_time) / 1000000.0;
                break;
            }

            double dur = (current_time - tt.start_time) / 1000000.0;
            if (tt.callback) tt.callback(dur);

            if (tt.is_repeat) {
                TimerTask ntt{std::move(top())};
                pop();

                ntt.start_time = current_time;
                ntt.trigger_time = current_time + ntt.interval;
                _add(std::move(ntt));
            } else {
                pop();
            }
        }
        return timeout;
    }
} // namespace RolUI
