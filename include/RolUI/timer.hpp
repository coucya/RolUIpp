#pragma once

#include <cstddef>
#include <queue>

namespace RolUI {

    typedef void (*TimeoutCallback)(void*);

    namespace _details {

        struct TimerTask {
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
        size_t push(TimeoutCallback cb, double duration, void* arg = nullptr) noexcept;

        void remove(size_t handle) noexcept;

        double do_timer() noexcept;

      private:
        size_t _timer_handle = 0;
    };

} // namespace RolUI
