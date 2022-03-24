#pragma once

#include <cstddef>

#include "../Property.hpp"
#include "../Widget.hpp"

namespace RolUI {
    namespace widgets {

        class ScrollView : public SingleChildWidget {
          public:
            Property<ScrollView, Point> offset{this};

          public:
            ScrollView() noexcept;

            float widget_x_ratio() const noexcept;
            float widget_y_ratio() const noexcept;

            void scroll_by_px(int dx, int dy) noexcept;
            void scroll_by_ratio(float x, float y) noexcept;
            void scroll_x_by_px(int dx) noexcept;
            void scroll_y_by_px(int dy) noexcept;
            void scroll_x_by_ratio(float x) noexcept;
            void scroll_y_by_ratio(float y) noexcept;

            void scroll_to_px(int x, int y) noexcept;
            void scroll_to_ratio(float x, float y) noexcept;
            void scroll_x_to_px(int x) noexcept;
            void scroll_y_to_px(int y) noexcept;
            void scroll_x_to_ratio(float x) noexcept;
            void scroll_y_to_ratio(float y) noexcept;

          protected:
            Size perform_layout(Constraint constraint) noexcept override;
            void draw(IPainter* painter) noexcept override;
        };

        class VScrollView : public ScrollView {
          public:
            Property<VScrollView, float> scroll_step{this, 10.0};

          public:
            VScrollView() noexcept;

          protected:
            Size perform_layout(Constraint constraint) noexcept override;
            bool handle_event(IEvent* e) noexcept override;
        };

        class HScrollView : public ScrollView {
          public:
            Property<HScrollView, float> scroll_step{this, 10.0};

          public:
            HScrollView() noexcept;

          protected:
            Size perform_layout(Constraint constraint) noexcept override;
            bool handle_event(IEvent* e) noexcept override;
        };

    } // namespace widgets
} // namespace RolUI
