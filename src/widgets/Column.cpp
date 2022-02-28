
#include "RolUI/widgets/Column.hpp"

namespace RolUI {
    namespace widget {

        Column::Column() noexcept {}

        Size Column::layout(Constraint constraint) noexcept {
            Size self_size = constraint.max();
            int cw = constraint.max_width();
            int ch = constraint.max_height();

            for (int i = 0; i < child_count(); i++) {
                Widget* child = this->child(i);
                Constraint nc = Constraint::zero_to(cw, ch);
                Size s = child->layout(nc);
                RolUI::set_rect(child, {{0, 0}, s});
                ch = std::max(ch - s.height, 0);
            }

            int max_w = 0;
            int total_h = 0;
            for (int i = 0; i < child_count(); i++) {
                Widget* child = this->child(i);
                if (child->size().width > max_w)
                    max_w = child->size().width;
                total_h = total_h + child->size().height;
            }

            int cy = 0;
            for (int i = 0; i < child_count(); i++) {
                Widget* child = this->child(i);
                float r = 0;
                if (cross_axis_alignment.get() == left)
                    r = 0.0f;
                else if (cross_axis_alignment.get() == center)
                    r = 0.5f;
                else if (cross_axis_alignment.get() == right)
                    r = 1.0f;

                int cx = float(max_w - child->size().width) * r;
                RolUI::set_rect(child, {{cx, cy}, child->size()});

                cy = cy + child->size().height;
            }

            return {max_w, total_h};
        }

    } // namespace widget
} // namespace RolUI