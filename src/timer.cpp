
#include <chrono>

#include "RolUI/timer.hpp"
#include "RolUI/Application.hpp"

namespace RolUI {

    // size_t TimerQueue::push(TimeoutCallback cb, unsigned long long target_time, void* arg) noexcept {
    //     using namespace std::chrono;

    //     long long current_time = duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
    //     TimerTask tt{(unsigned long long)current_time, target_time, cb, arg, _timer_handle++};

    //     priority_queue::push(tt);
    //     return tt.handle;
    // }

    size_t TimerQueue::push(TimeoutCallback cb, double interval, void* arg) noexcept {
        using namespace std::chrono;

        long long current_time = duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
        unsigned long long target_time = current_time + std::max(interval, 0.0) * 1000000.0;
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
        while (!empty()) {
            TimerTask tt = top();
            unsigned long long current_time = duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();

            if (tt.trigger_time > current_time) {
                timeout = (double)(tt.trigger_time - current_time) / 1000000.0;
                break;
            }

            double dur = (current_time - tt.start_time) / 1000000.0;
            if (tt.callback)
                tt.callback(dur, tt.arg);
            pop();
        }
        return std::max(0.0, timeout);
    }

    Timer::~Timer() { stop(); }

    void Timer::_timeout_cb(double interval, void* arg) noexcept {
        using namespace std::chrono;
        Timer* timer = (Timer*)arg;

        if (!timer->is_singleShot()) {
            timer->_handle = Application::set_timeout(_timeout_cb, timer->interval(), timer);
        } else {
            timer->_is_action = false;
        }
        timer->on_timeout.emit(interval);
    }

    void Timer::start(double interval, bool single_shot) noexcept {
        if (is_action()) stop();
        _interval = interval;
        _singleShot = single_shot;
        _is_action = true;
        Application::set_timeout(Timer::_timeout_cb, _interval, this);
    }
    void Timer::stop() noexcept {
        if (is_action()) {
            Application::remove_timeout(_handle);
        }
    }

} // namespace RolUI
