#include "RolUI/widgets/flow.hpp"

namespace RolUI {
    namespace widgets {

        Column::Column() noexcept {}

        Size Column::perform_layout(Constraint constraint) noexcept {
            int cw = constraint.max_width();
            int ch = constraint.max_height();

            for (int i = 0; i < child_count(); i++) {
                Constraint nc = Constraint::zero_to(cw, ch);
                Size s = child(i)->layout(nc);
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
                if (cross_axis_alignment.get() == CrossAxisAlignment::start)
                    r = 0.0f;
                else if (cross_axis_alignment.get() == CrossAxisAlignment::center)
                    r = 0.5f;
                else if (cross_axis_alignment.get() == CrossAxisAlignment::end)
                    r = 1.0f;

                int cx = float(max_w - child->size().width) * r;
                RolUI::set_pos(child, {cx, cy});

                cy = cy + child->size().height;
            }
            Size self_size = {max_w, total_h};
            return self_size;
        }

        Row::Row() noexcept {}

        Size Row::perform_layout(Constraint constraint) noexcept {
            int cw = constraint.max_width();
            int ch = constraint.max_height();

            for (int i = 0; i < child_count(); i++) {
                Constraint nc = Constraint::zero_to(cw, ch);
                Size s = child(i)->layout(nc);
                cw = std::max(cw - s.width, 0);
            }

            int max_h = 0;
            int total_w = 0;
            for (int i = 0; i < child_count(); i++) {
                Size child_size = this->child(i)->size();
                if (child_size.height > max_h)
                    max_h = child_size.height;
                total_w = total_w + child_size.width;
            }

            int cx = 0;
            for (int i = 0; i < child_count(); i++) {
                Widget* child = this->child(i);
                float r = 0;
                if (cross_axis_alignment.get() == CrossAxisAlignment::start)
                    r = 0.0f;
                else if (cross_axis_alignment.get() == CrossAxisAlignment::center)
                    r = 0.5f;
                else if (cross_axis_alignment.get() == CrossAxisAlignment::end)
                    r = 1.0f;

                int cy = float(max_h - child->size().height) * r;
                RolUI::set_pos(child, {cx, cy});

                cx = cx + child->size().width;
            }

            Size self_size = {total_w, max_h};
            return self_size;
        }

    } // namespace widgets
} // namespace RolUI