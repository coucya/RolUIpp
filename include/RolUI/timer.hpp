#pragma once

#include <cstddef>
#include <queue>

#include "RolUI/sigslot/Signal.hpp"
#include "RolUI/sigslot/Slot.hpp"

namespace RolUI {

    typedef void (*TimeoutCallback)(double, void*);

    namespace _details {

        struct TimerTask {
            unsigned long long start_time;
            unsigned long long trigger_time;
            TimeoutCallback callback;
            void* arg = nullptr;
            size_t handle;

            bool operator<(const TimerTask& right) const noexcept {
                return trigger_time > right.trigger_time;
            }
            bool operator>(const TimerTask& right) const noexcept {
                return trigger_time < right.trigger_time;
            }
        };

    } // namespace _details

    class TimerQueue : public std::priority_queue<_details::TimerTask> {
      private:
        typedef _details::TimerTask TimerTask;

      public:
        typedef std::priority_queue<TimerTask> priority_queue;

      public:
        size_t push(TimeoutCallback cb, unsigned long long target_time, void* arg = nullptr) noexcept;
        size_t push(TimeoutCallback cb, double interval, void* arg = nullptr) noexcept;

        void remove(size_t handle) noexcept;

        double do_timer() noexcept;

      private:
        size_t _timer_handle = 0;
    };

    class Application;

    class Timer {
      public:
        ~Timer();

        bool is_action() const noexcept { return _app != nullptr; }
        bool is_singleShot() const noexcept { return _singleShot; }

        double interval() const noexcept { return _interval; }

        void set_interval(double interval) noexcept { _interval = interval; }
        void set_singleShot(bool singleShot) noexcept { _singleShot = singleShot; }

        void start(Application* app) noexcept;
        void start(Application* app, double interval, bool single_shot = true) noexcept;
        void stop() noexcept;

      public:
        Signal<double> on_timeout;

      private:
        static void _timeout_cb(double interval, void* arg) noexcept;

      private:
        double _interval = 0.0;
        bool _singleShot = true;

        double _start_timepoint;

        Application* _app = nullptr;
        size_t _handle;
    };

} // namespace RolUI
