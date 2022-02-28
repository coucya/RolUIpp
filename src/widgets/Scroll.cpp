#include <algorithm>
#include <cmath>
#include <limits>

#include "RolUI/Point.hpp"
#include "RolUI/widgets/Rect.hpp"
#include "RolUI/widgets/Scroll.hpp"

namespace RolUI {
    namespace widget {

        Scroll::Scroll() noexcept {}

        Point Scroll::widget_pos() const noexcept { return child() ? child()->pos() : Point{0, 0}; }
        int Scroll::widget_x() const noexcept { return child() ? child()->pos().x : 0; }
        int Scroll::widget_y() const noexcept { return child() ? child()->pos().y : 0; }

        float Scroll::widget_x_ratio() const noexcept {
            Size self_size = size();
            Size widget_size = child()->size();

            int w_dis = std::max(widget_size.width - self_size.width, self_size.width - widget_size.width);

            int wx = widget_x();
            if (w_dis == 0 && wx == 0)
                return 0.0f;
            else if (w_dis == 0 && wx < 0)
                return -std::numeric_limits<float>::infinity();
            else if (w_dis == 0 && wx > 0)
                return +std::numeric_limits<float>::infinity();
            return float(wx) / float(w_dis);
        }
        float Scroll::widget_y_ratio() const noexcept {
            Size self_size = size();
            Size widget_size = child()->size();

            int h_dis = std::max(widget_size.height - self_size.height, self_size.height - widget_size.height);

            int wy = widget_y();
            if (h_dis == 0 && wy == 0)
                return 0.0f;
            else if (h_dis == 0 && wy < 0)
                return -std::numeric_limits<float>::infinity();
            else if (h_dis == 0 && wy > 0)
                return +std::numeric_limits<float>::infinity();
            return float(wy) / float(h_dis);
        }

        void Scroll::scroll_by_px(int dx, int dy) noexcept {
            if (!child()) return;
            offset = offset.get() + Point{dx, dy};
        }
        void Scroll::scroll_by_ratio(float x, float y) noexcept {
            if (!child()) return;
            Size self_size = size();
            Size widget_size = child()->size();

            int w_dis = std::max(widget_size.width - self_size.width, self_size.width - widget_size.width);
            int h_dis = std::max(widget_size.height - self_size.height, self_size.height - widget_size.height);

            scroll_by_px(x * w_dis, y * h_dis);
        }

        void Scroll::scroll_x_by_px(int dx) noexcept {
            scroll_by_px(dx, 0);
        }

        void Scroll::scroll_y_by_px(int dy) noexcept {
            scroll_by_px(0, dy);
        }
        void Scroll::scroll_x_by_ratio(float x) noexcept {
            scroll_by_ratio(x, 0);
        }
        void Scroll::scroll_y_by_ratio(float y) noexcept {
            scroll_by_ratio(0, y);
        }

        void Scroll::scroll_to_px(int x, int y) noexcept {
            if (!child()) return;
            offset = {x, y};
        }
        void Scroll::scroll_to_ratio(float x, float y) noexcept {
            if (!child()) return;
            Size self_size = size();
            Size widget_size = child()->size();

            int w_dis = std::max(widget_size.width - self_size.width, self_size.width - widget_size.width);
            int h_dis = std::max(widget_size.height - self_size.height, self_size.height - widget_size.height);

            scroll_to_px(w_dis * x, h_dis * y);
        }
        void Scroll::scroll_x_to_px(int x) noexcept { scroll_to_px(x, child()->pos().y); }
        void Scroll::scroll_y_to_px(int y) noexcept { scroll_to_px(child()->pos().x, y); }
        void Scroll::scroll_x_to_ratio(float x) noexcept {
            if (!child()) return;
            Size self_size = size();
            Size widget_size = child()->size();
            int w_dis = std::max(widget_size.width - self_size.width, self_size.width - widget_size.width);
            scroll_to_px(w_dis * x, child()->pos().y);
        }
        void Scroll::scroll_y_to_ratio(float y) noexcept {
            if (!child()) return;
            Size self_size = size();
            Size widget_size = child()->size();
            int h_dis = std::max(widget_size.height - self_size.height, self_size.height - widget_size.height);
            scroll_to_ratio(child()->pos().x, h_dis * y);
        }

        Size Scroll::layout(Constraint constraint) noexcept {
            if (child()) {
                Widget* widget = child();
                int n = std::numeric_limits<int>::max();
                Size s = widget->layout(Constraint{{0, 0}, {n, n}});
                RolUI::set_rect(widget, {offset.get(), s});
            }
            return constraint.max();
        }
    } // namespace widget
} // namespace RolUI
