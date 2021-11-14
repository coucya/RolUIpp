#pragma once

#include <cstddef>

#include "RolUI/Widget.hpp"

namespace RolUI {
    namespace widget {

        class Scroll : public Widget {
          public:
            Scroll() noexcept;
            Scroll(Widget* parent) noexcept;

            Widget* widget() const noexcept;
            void set_widget(Widget* widget) noexcept;

            Point widget_pos() const noexcept;
            int widget_x() const noexcept;
            int widget_y() const noexcept;
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

          private:
            Widget* _widget = nullptr;
        };

    } // namespace widget
} // namespace RolUI
