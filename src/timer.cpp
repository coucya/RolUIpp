
#include <charconv>

#include "RolUI/timer.hpp"

namespace RolUI {

    size_t TimerQueue::push(TimeoutCallback cb, unsigned long long target_time, void* arg) noexcept {
        TimerTask tt{target_time, cb, arg, _timer_handle++};
        priority_queue::push(tt);
        return tt.handle;
    }

    size_t TimerQueue::push(TimeoutCallback cb, double duration, void* arg) noexcept {
        using namespace std::chrono;

        long long current_time = duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
        unsigned long long target_time = current_time + (duration <= 0.0 ? 0ull : (unsigned long long)duration * 1000000);
        return push(cb, target_time, arg);
    }

    void TimerQueue::remove(size_t handle) noexcept {
        auto it = std::find_if(c.begin(), c.end(), [&](const TimerTask& tt) {
            return tt.handle == handle;
        });
        c.erase(it);
        std::make_heap(c.begin(), c.end());
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

            if (tt.callback)
                tt.callback(tt.arg);
            pop();
        }
        return std::max(0.0, timeout - tolerance / 1000000.0);
    }

} // namespace RolUI
