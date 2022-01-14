

#include "RolUI/widgets/Column.hpp"

namespace RolUI {
    namespace widget {

        Column::Column() noexcept {}

        Size Column::perlayout(Constraint constraint) {
            Size self_size = constraint.max();
            int cw = constraint.max_width();
            int ch = constraint.max_height();
            int cy = 0;

            for (int i = 0; i < child_count(); i++) {
                Widget* child = get_child(i);
                Constraint nc = Constraint::zero_to(cw, ch);
                Size s = RolUI::perlayout(child, nc);
                RolUI::set_rect(child, {{0, cy}, s});
                ch = std::max(0, ch - s.height);
                cy = cy + s.height;
            }

            return constraint.max();
        }

    } // namespace widget
} // namespace RolUI