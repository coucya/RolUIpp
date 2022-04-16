#pragma once

#include <cstddef>
#include <functional>

#include "./Point.hpp"
#include "./Widget.hpp"

namespace RolUI {

    class Window;
    class Widget;
    class IEvent;

    typedef std::function<void(double)> TimeoutCallback;

    class RootWidget : public MultiChildWidget {
      public:
        RootWidget() noexcept;

        Widget* content_widget() const noexcept;
        void set_content_widget(Widget* widget) noexcept;

        const ObjectType* object_type() const noexcept override;
        Size perform_layout(Constraint constraint) noexcept override;
    };

    RolUI_decl_object_type_of(RootWidget);

    class Application {
      public:
        Application(const Application&) = delete;
        Application(Application&&) = delete;
        Application& operator=(const Application&) = delete;
        Application& operator=(Application&&) = delete;

        static void init(Window* w) noexcept;

        static Window* window() noexcept;
        static RootWidget* root_widget() noexcept;

        static size_t set_timeout(double duration, TimeoutCallback cb);
        static size_t set_interval(double duration, TimeoutCallback cb);
        static void clear_timeout(size_t handle);
        static void clear_interval(size_t handle);

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
