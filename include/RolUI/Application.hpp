#pragma once

#include <cstddef>

#include "RolUI/timer.hpp"

namespace RolUI {

    class Window;
    class Widget;
    class IEvent;

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
