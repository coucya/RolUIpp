
#include <algorithm>

#include "RolUI/widgets/Margin.hpp"

namespace RolUI {
    namespace widgets {

        Margin::Margin() noexcept {}
        Margin::Margin(unsigned margin) noexcept {
            top = margin;
            bottom = margin;
            left = margin;
            right = margin;
        }
        Margin::Margin(unsigned x, unsigned y) noexcept {
            top = y;
            bottom = y;
            left = x;
            right = x;
        }
        Margin::Margin(unsigned top, unsigned bottom, unsigned left, unsigned right) noexcept {
            top = top;
            bottom = bottom;
            left = left;
            right = right;
        }

        Size Margin::perform_layout(Constraint constraint) noexcept {
            Size m = constraint.max();
            int cw = std::max(m.width - int(left() + right()), 0);
            int ch = std::max(m.height - int(top() + bottom()), 0);
            int cx = left();
            int cy = top();

            Size child_size = layout_child(Constraint::zero_to(cw, ch), [&](Size s) {
                return Point{cx, cy};
            });

            Size self_size;
            if (this->child())
                self_size = {int(left() + right() + child_size.width), int(top() + bottom() + child_size.height)};
            else
                self_size = {int(left() + right()), int(top() + bottom())};
            return self_size;
        }

    } // namespace widgets
} // namespace RolUI