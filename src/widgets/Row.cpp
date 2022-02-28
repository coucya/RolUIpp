

#include "RolUI/widgets/Row.hpp"

namespace RolUI {
    namespace widget {

        Row::Row() noexcept {}

        Size Row::layout(Constraint constraint) noexcept {
            Size self_size = constraint.max();
            int cw = constraint.max_width();
            int ch = constraint.max_height();

            for (int i = 0; i < child_count(); i++) {
                Widget* child = this->child(i);
                Constraint nc = Constraint::zero_to(cw, ch);
                Size s = child->layout(nc);
                RolUI::set_rect(child, {{0, 0}, s});
                cw = std::max(cw - s.width, 0);
            }

            int max_h = 0;
            int total_w = 0;
            for (int i = 0; i < child_count(); i++) {
                Widget* child = this->child(i);
                if (child->size().height > max_h)
                    max_h = child->size().height;
                total_w = total_w + child->size().width;
            }

            int cx = 0;
            for (int i = 0; i < child_count(); i++) {
                Widget* child = this->child(i);
                float r = 0;
                if (cross_axis_alignment.get() == top)
                    r = 0.0f;
                else if (cross_axis_alignment.get() == center)
                    r = 0.5f;
                else if (cross_axis_alignment.get() == bottom)
                    r = 1.0f;

                int cy = float(max_h - child->size().height) * r;
                RolUI::set_rect(child, {{cx, cy}, child->size()});

                cx = cx + child->size().width;
            }

            return {total_w, max_h};
        }

    } // namespace widget
} // namespace RolUI