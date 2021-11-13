#pragma once

#include <queue>

namespace RolUI {

    class Window;
    class Widget;
    class IEvent;

    typedef void (*TimeoutCallback)(void*);

    struct TimerTask {
        unsigned long long trigger_time;
        TimeoutCallback callback;
        void* arg;

        bool operator<(const TimerTask& right) const noexcept {
            return trigger_time > right.trigger_time;
        }
        bool operator>(const TimerTask& right) const noexcept {
            return trigger_time < right.trigger_time;
        }
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

        void set_timeout(TimeoutCallback cb, double duration, void* arg = nullptr);

        void run() noexcept;
        void exit() noexcept;

      private:
        void _draw_window() noexcept;
        void _dispatch_event(double timeout) noexcept;

        double _do_timer() noexcept;

      private:
        bool _should_exit = false;
        Window* _window = nullptr;

        std::priority_queue<TimerTask> _timers;
    };

} // namespace RolUI
