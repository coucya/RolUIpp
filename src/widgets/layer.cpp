
#include "RolUI/widgets/layer.hpp"
#include "RolUI/IPainter.hpp"

namespace RolUI {
    namespace widgets {

        StackWidget::StackWidget() noexcept {}
        StackWidget::StackWidget(float x, float y) noexcept {
            align_x = x;
            align_y = y;
        }

        Size StackWidget::perform_layout(Constraint constraint) noexcept {
            int cw = constraint.max_width();
            int ch = constraint.max_height();

            int max_w = 0;
            int max_h = 0;

            for (int i = 0; i < child_count(); i++) {
                Widget* child = this->child(i);
                Constraint nc = Constraint::zero_to(cw, ch);
                Size s = child->layout(nc);

                max_w = std::max(max_w, s.width);
                max_h = std::max(max_h, s.height);
            }

            for (int i = 0; i < child_count(); i++) {
                Widget* child = this->child(i);
                Size cs = child->size();
                int cx = float(max_w - cs.width) * this->align_x;
                int cy = float(max_h - cs.height) * this->align_y;

                RolUI::set_pos(child, {cx, cy});
            }

            return {max_w, max_h};
        }

        DeckWidget::DeckWidget() noexcept {}
        DeckWidget::DeckWidget(unsigned selected) noexcept {
            this->selected = selected;
        }

        Widget* DeckWidget::get_child_by_pos(Point pos) const noexcept {
            Widget* sw = child(selected.get());
            if (sw == nullptr) return nullptr;
            if (sw->hit_test(pos)) return sw;
            return nullptr;
        }

        Size DeckWidget::perform_layout(Constraint constraint) noexcept {
            if (selected.get() >= this->child_count()) return {0, 0};

            Widget* sw = this->child(selected.get());
            if (sw == nullptr) return {0, 0};

            Size s = sw->layout(constraint);
            return s;
        }

        void DeckWidget::draw(IPainter* painter) noexcept {
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