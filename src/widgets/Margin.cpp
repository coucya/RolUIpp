
#include <algorithm>

#include "RolUI/widgets/Margin.hpp"

namespace RolUI {
    namespace widget {

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

        Size Margin::perlayout(Constraint constraint) {
            Size m = constraint.max;
            int cw = m.width - int(left.get() + right.get());
            int ch = m.height - int(top.get() + bottom.get());
            cw = std::max(cw, 0);
            ch = std::max(ch, 0);
            int cx = left.get();
            int cy = top.get();

            if (child_count() == 0 || get_child(0) == nullptr)
                return {int(left.get() + right.get()), int(top.get() + bottom.get())};

            Widget* child = get_child(0);
            Size s = RolUI::perlayout(child, {{0, 0}, {cw, ch}});
            RolUI::set_rect(child, Rect{{cx, cy}, s});
            return {int(left.get() + right.get() + s.width), int(top.get() + bottom.get() + s.height)};
        }

    } // namespace widget
} // namespace RolUI