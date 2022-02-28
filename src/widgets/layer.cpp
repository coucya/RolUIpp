
#include "RolUI/widgets/layer.hpp"
#include "RolUI/IPainter.hpp"

namespace RolUI {
    namespace widgets {

        Stack::Stack() noexcept {}
        Stack::Stack(float x, float y) noexcept {
            align_x = x;
            align_y = y;
        }

        Size Stack::layout(Constraint constraint) noexcept {
            int cw = constraint.max_width();
            int ch = constraint.max_height();

            int max_w = 0;
            int max_h = 0;

            for (int i = 0; i < child_count(); i++) {
                Widget* child = this->child(i);
                Constraint nc = Constraint::zero_to(cw, ch);
                Size s = child->layout(nc);
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

        Deck::Deck() noexcept {}
        Deck::Deck(unsigned selected) noexcept {
            this->selected = selected;
        }

        Widget* Deck::get_child_by_pos(Point pos) const noexcept {
            Widget* sw = child(selected.get());
            if (sw == nullptr) return nullptr;
            if (sw->hit_test(pos)) return sw;
            return nullptr;
        }

        Size Deck::layout(Constraint constraint) noexcept {
            if (selected.get() >= this->child_count()) return {0, 0};

            Widget* sw = this->child(selected.get());
            if (sw == nullptr) return {0, 0};

            Size s = sw->layout(constraint);
            RolUI::set_rect(sw, {0, 0, s.width, s.height});
            return s;
        }

        void Deck::draw(IPainter* painter) noexcept {
            if (selected.get() >= this->child_count()) return;

            Widget* sw = this->child(selected.get());
            if (sw == nullptr) return;

            RolUI::Rect ar = abs_rect();
            RolUI::Rect current_scissor = painter->get_scissor();
            painter->scissor(
                current_scissor
                    .intersected(ar)
                    .value_or(RolUI::Rect{ar.pos(), Size{0, 0}}));
            // painter->set_base_pos(ar.pos() + sw->pos());
            sw->draw(painter);
            painter->scissor(current_scissor);
        }

    } // namespace widgets
} // namespace RolUI