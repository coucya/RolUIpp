#pragma once

#include "RolUI/IPainter.hpp"
#include "RolUI/Widget.hpp"
#include <cstddef>

namespace RolUI {
    namespace widget {

        class Scroll : public Widget {
          public:
            Scroll() noexcept;
            Scroll(Widget* parent) noexcept;

            Widget* widget() const noexcept;
            void set_widget(Widget* widget) noexcept;

            void scroll_by_px(int dx, int dy) noexcept;
            void scroll_by_ratio(float x, float y) noexcept;
            void scroll_x_by_px(int dx) noexcept;
            void scroll_y_by_px(int dy) noexcept;
            void scroll_x_by_ration(float dx) noexcept;
            void scroll_y_by_ration(float dy) noexcept;

            void on_draw(IPainter* p) override {
                p->set_fill_color({155, 240, 240});
                // p->fill_rect({0, 0, size().width, size().height});
                p->fill_rect({pos(), size()});
            }

          private:
            Widget* _widget = nullptr;
        };

    } // namespace widget
} // namespace RolUI
