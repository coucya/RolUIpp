#pragma once

#include <algorithm>
#include <cstddef>
#include <queue>

namespace RolUI {

    class Window;
    class Widget;
    class IEvent;

    typedef void (*TimeoutCallback)(void*);

    struct TimerTask {
        unsigned long long trigger_time;
        TimeoutCallback callback;
        void* arg = nullptr;
        bool is_action = true;
        size_t handle;

        bool operator<(const TimerTask& right) const noexcept {
            return trigger_time > right.trigger_time;
        }
        bool operator>(const TimerTask& right) const noexcept {
            return trigger_time < right.trigger_time;
        }
    };

    class TimerQueue : public std::priority_queue<TimerTask> {
      public:
        typedef std::priority_queue<TimerTask> priority_queue;

      public:
        size_t push(TimeoutCallback cb, unsigned long long target_time, void* arg = nullptr) noexcept {
            TimerTask tt{target_time, cb, arg, true, _timer_handle++};
            priority_queue::push(tt);
            return tt.handle;
        }
        void remove(size_t handle) noexcept {
            auto it = std::find_if(c.begin(), c.end(), [&](const TimerTask& tt) {
                return tt.handle == handle;
            });
            c.erase(it);
            std::make_heap(c.begin(), c.end());
        }

      private:
        size_t _timer_handle = 0;
    };

    class Application {
      public:
        Application();

        Application(const Application&) = delete;
        Application(Application&&) = delete;

        ~Application();

        Application& operator=(const Application&) = delete;
        Application& operator=(Application&&) = delete;

        void set_window(Window* w) noexcept;
        void remove_window(Window* w) noexcept;

        size_t set_timeout(TimeoutCallback cb, double duration, void* arg = nullptr);
        void remove_timeout(size_t handle);

        void run() noexcept;
        void exit() noexcept;

      private:
        void _draw_window() noexcept;
        void _dispatch_event(double timeout) noexcept;

        double _do_timer() noexcept;

      private:
        bool _should_exit = false;
        Window* _window = nullptr;

        TimerQueue _timer_queue;
    };

} // namespace RolUI
