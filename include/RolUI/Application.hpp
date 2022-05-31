#pragma once

#include <cmath>
#include <cstddef>
#include <functional>

#include "./Point.hpp"
#include "./Widget.hpp"

namespace RolUI {

    class Window;
    class Widget;
    class IEvent;

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

        static bool has_focus_widget(Widget* w) noexcept;
        static void set_focus_widget(Widget* w) noexcept;
        static Widget* focus_widget() noexcept;

        static Widget* get_widget_by_pos(Point pos) noexcept;

        static void flush_animation() noexcept;
        static void flush_layout() noexcept;
        static void flush_draw() noexcept;

        static void flush_frame(double time) noexcept;

        static void exit() noexcept;
    };

} // namespace RolUI
