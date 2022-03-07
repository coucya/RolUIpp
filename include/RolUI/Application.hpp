#pragma once

#include <cstddef>
#include <functional>

#include "./Point.hpp"

namespace RolUI {

    class Window;
    class Widget;
    class IEvent;

    typedef std::function<void(double)> TimeoutCallback;

    class Application {
      public:
        Application(const Application&) = delete;
        Application(Application&&) = delete;
        Application& operator=(const Application&) = delete;
        Application& operator=(Application&&) = delete;

        static void init(Window* w) noexcept;
        static Window* window() noexcept;

        static size_t set_timeout(double duration, TimeoutCallback cb);
        static size_t set_interval(double duration, TimeoutCallback cb);
        static void clear_timeout(size_t handle);
        static void clear_interval(size_t handle);

        static void set_content_widget(Widget* w) noexcept;
        static Widget* content_widget() noexcept;

        static bool has_focus_widget(Widget* w) noexcept;
        static void set_focus_widget(Widget* w) noexcept;
        static Widget* focus_widget() noexcept;

        static Widget* get_widget_by_pos(Point pos) noexcept;

        static void flush_layout() noexcept;
        static void flush_draw() noexcept;
        static void flush_frame() noexcept;

        static void run() noexcept;
        static void run(Widget* w) noexcept;

        static void exit() noexcept;

      private:
        static void _dispatch_event(double timeout) noexcept;

        static double _do_timer() noexcept;
    };

} // namespace RolUI
