#include <algorithm>

#include "RolUI/widgets/Align.hpp"

namespace RolUI {
    namespace widget {

        Align::Align() noexcept {}
        Align::Align(float x, float y) noexcept {
            align_x = x;
            align_y = y;
        }

        Size Align::layout(Constraint constraint) noexcept {
            Size self_size = constraint.max();

            Widget* cw = child();
            if (cw == nullptr)
                return constraint.max();

            Size cs = cw->layout(constraint);

            int cx = float(self_size.width - cs.width) * align_x.get();
            int cy = float(self_size.height - cs.height) * align_y.get();

            RolUI::set_rect(cw, Rect{{cx, cy}, cs});
            return constraint.max();
        }

    } // namespace widget
} // namespace RolUI