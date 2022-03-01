#include <algorithm>

#include "RolUI/widgets/Align.hpp"

namespace RolUI {
    namespace widgets {

        Align::Align() noexcept {}
        Align::Align(float x, float y) noexcept {
            align_x = x;
            align_y = y;
        }

        Size Align::perform_layout(Constraint constraint) noexcept {
            Size self_size = constraint.max();

            layout_child(constraint, [&](Size s) {
                int cx = float(self_size.width - s.width) * align_x.get();
                int cy = float(self_size.height - s.height) * align_y.get();
                return Point{cx, cy};
            });

            return self_size;
        }

    } // namespace widgets
} // namespace RolUI