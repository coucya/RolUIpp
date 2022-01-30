#pragma once

#include <cstddef>

#include "./Point.hpp"

namespace RolUI {

    class Window;
    class Widget;
    class IEvent;

    typedef void (*TimeoutCallback)(double, void*);

    class Application {
      public:
        Application(const Application&) = delete;
        Application(Application&&) = delete;
        Application& operator=(const Application&) = delete;
        Application& operator=(Application&&) = delete;

        static void init(Window* w) noexcept;
        static Window* window() noexcept;

        static size_t set_timeout(TimeoutCallback cb, double duration, void* arg = nullptr);
        static void remove_timeout(size_t handle);

        static void set_content_widget(Widget* w) noexcept;
        static Widget* content_widget() noexcept;

        static Widget* get_widget_by_pos(Point pos) noexcept;

        static void layout() noexcept;

        static void run() noexcept;
        static void run(Widget* w) noexcept;

        static void exit() noexcept;

      private:
        static void _draw_window() noexcept;
        static void _dispatch_event(double timeout) noexcept;

        static double _do_timer() noexcept;
    };

} // namespace RolUI
