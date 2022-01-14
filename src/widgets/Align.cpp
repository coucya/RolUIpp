#include <algorithm>

#include "RolUI/widgets/Align.hpp"

namespace RolUI {
    namespace widget {

        Align::Align() noexcept {}
        Align::Align(float x, float y) noexcept {
            align_x = x;
            align_y = y;
        }

        Size Align::perlayout(Constraint constraint) {
            Size self_size = constraint.max();

            if (child_count() == 0 || get_child(0) == nullptr)
                return constraint.max();

            Widget* child = get_child(0);
            Size cs = RolUI::perlayout(child, constraint);

            int cx = float(self_size.width - cs.width) * align_x.get();
            int cy = float(self_size.height - cs.height) * align_y.get();

            RolUI::set_rect(child, Rect{{cx, cy}, cs});
            return constraint.max();
        }

    } // namespace widget
} // namespace RolUI