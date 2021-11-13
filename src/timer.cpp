
#include <charconv>

#include "RolUI/timer.hpp"
#include "RolUI/Application.hpp"

namespace RolUI {

    size_t TimerQueue::push(TimeoutCallback cb, unsigned long long target_time, void* arg) noexcept {
        using namespace std::chrono;

        long long current_time = duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
        TimerTask tt{(unsigned long long)current_time, target_time, cb, arg, _timer_handle++};

        priority_queue::push(tt);
        return tt.handle;
    }

    size_t TimerQueue::push(TimeoutCallback cb, double interval, void* arg) noexcept {
        using namespace std::chrono;

        long long current_time = duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
        unsigned long long target_time = current_time + (interval <= 0.0 ? 0ull : (unsigned long long)interval * 1000000);
        TimerTask tt{(unsigned long long)current_time, target_time, cb, arg, _timer_handle++};

        priority_queue::push(tt);
        return tt.handle;
    }

    void TimerQueue::remove(size_t handle) noexcept {
        auto it = std::find_if(c.begin(), c.end(), [&](const TimerTask& tt) {
            return tt.handle == handle;
        });
        if (it != c.end()) {
            c.erase(it);
            std::make_heap(c.begin(), c.end());
        }
    }

    double TimerQueue::do_timer() noexcept {
        using namespace std::chrono;

        double timeout = 60.0;
        unsigned long long tolerance = 10000; // 0.010s.
        while (!empty()) {
            TimerTask tt = top();
            unsigned long long current_time = duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();

            if (tt.trigger_time > current_time && current_time - tt.trigger_time > tolerance) {
                timeout = (double)(tt.trigger_time - current_time) / 1000000.0;
                break;
            }

            double dur = (current_time - tt.start_time) / 1000000.0;
            if (tt.callback)
                tt.callback(dur, tt.arg);
            pop();
        }
        return std::max(0.0, timeout - tolerance / 1000000.0);
    }

    Timer::~Timer() { stop(); }

    void Timer::_timeout_cb(double interval, void* arg) noexcept {
        using namespace std::chrono;
        Timer* timer = (Timer*)arg;

        if (timer->_app && !timer->is_singleShot())
            timer->_handle = timer->_app->set_timeout(_timeout_cb, timer->interval(), timer);

        timer->on_timeout.emit(interval);

        if (timer->_app && timer->is_singleShot())
            timer->_app = nullptr;
    }

    void Timer::start(Application* app) noexcept {
        stop();
        if (!(_app = app)) return;
        _handle = _app->set_timeout(Timer::_timeout_cb, _interval, this);
    }
    void Timer::start(Application* app, double interval, bool single_shot) noexcept {
        stop();
        if (!(_app = app)) return;
        set_interval(interval);
        set_singleShot(single_shot);
        _handle = _app->set_timeout(Timer::_timeout_cb, _interval, this);
    }
    void Timer::stop() noexcept {
        if (!_app) return;
        _app->remove_timeout(_handle);
        _app = nullptr;
    }

} // namespace RolUI
