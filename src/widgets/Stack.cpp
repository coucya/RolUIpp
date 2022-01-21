
#include "RolUI/widgets/Stack.hpp"

namespace RolUI {
    namespace widget {

        Stack::Stack() noexcept {}
        Stack::Stack(float x, float y) noexcept {
            align_x = x;
            align_y = y;
        }

        Size Stack::perlayout(Constraint constraint) {
            int cw = constraint.max_width();
            int ch = constraint.max_height();

            int max_w = 0;
            int max_h = 0;

            for (int i = 0; i < child_count(); i++) {
                Widget* child = this->child(i);
                Constraint nc = Constraint::zero_to(cw, ch);
                Size s = child->perlayout(nc);
                RolUI::set_rect(child, {{0, 0}, s});

                max_w = std::max(max_w, s.width);
                max_h = std::max(max_h, s.height);
            }

            for (int i = 0; i < child_count(); i++) {
                Widget* child = this->child(i);
                Size cs = child->size();
                int cx = float(max_w - cs.width) * this->align_x;
                int cy = float(max_h - cs.height) * this->align_y;

                RolUI::set_rect(child, {{cx, cy}, cs});
            }

            return {max_w, max_h};
        }

    } // namespace widget
} // namespace RolUI