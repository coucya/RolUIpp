#include "RolUI/widgets/flow.hpp"

namespace RolUI {
    namespace widgets {

        ColumnWidget::ColumnWidget() noexcept {}

        Size ColumnWidget::perform_layout(Constraint constraint) noexcept {
            int cw = constraint.max_width();
            int ch = constraint.max_height();

            for (int i = 0; i < child_count(); i++) {
                Constraint nc = Constraint::zero_to(cw, ch);
                Size s = child(i)->layout(nc);
                ch = std::max(ch - s.height - gap(), 0);
            }

            int max_w = 0;
            int total_h = 0;
            for (int i = 0; i < child_count(); i++) {
                Widget* child = this->child(i);
                if (child->size().width > max_w)
                    max_w = child->size().width;
                total_h = total_h + child->size().height;
            }
            total_h += std::max(child_count() - 1, 0) * gap();

            int cy = 0;
            for (int i = 0; i < child_count(); i++) {
                Widget* child = this->child(i);
                float r = (cross_axis_alignment() + 1.0f) / 2.0f;
                int cx = float(max_w - child->size().width) * r;
                RolUI::set_pos(child, {cx, cy});

                cy = cy + child->size().height + gap();
            }
            Size self_size = {max_w, total_h};
            return self_size;
        }

        RowWidget::RowWidget() noexcept {}

        Size RowWidget::perform_layout(Constraint constraint) noexcept {
            int cw = constraint.max_width();
            int ch = constraint.max_height();

            for (int i = 0; i < child_count(); i++) {
                Constraint nc = Constraint::zero_to(cw, ch);
                Size s = child(i)->layout(nc);
                cw = std::max(cw - s.width - gap(), 0);
            }

            int max_h = 0;
            int total_w = 0;
            for (int i = 0; i < child_count(); i++) {
                Size child_size = this->child(i)->size();
                if (child_size.height > max_h)
                    max_h = child_size.height;
                total_w = total_w + child_size.width;
            }
            total_w += std::max(child_count() - 1, 0) * gap();

            int cx = 0;
            for (int i = 0; i < child_count(); i++) {
                Widget* child = this->child(i);
                float r = (cross_axis_alignment() + 1.0f) / 2.0f;
                int cy = float(max_h - child->size().height) * r;
                RolUI::set_pos(child, {cx, cy});

                cx = cx + child->size().width + gap();
            }

            Size self_size = {total_w, max_h};
            return self_size;
        }

        namespace _details {

            FlowGridWidgetBase::FlowGridWidgetBase() noexcept {}

            float FlowGridWidgetBase::flex_of(int index) const noexcept {
                if (index < 0 || index >= _flexs.size()) return 0.0f;
                return _flexs[index];
            }

            float FlowGridWidgetBase::flex_sum() const noexcept {
                float sum = 0.0f;
                for (float flex : _flexs) sum += flex;
                return sum;
            }

            FlowGridWidgetBase* FlowGridWidgetBase::add_child(Widget* child, float flex) noexcept {
                if (!child) return this;
                MultiChildWidget::add_child(child);
                _flexs.push_back(flex);
                return this;
            }
            FlowGridWidgetBase* FlowGridWidgetBase::set_child(int index, Widget* child, float flex) noexcept {
                if (!child) return this;
                if (index < 0 || index >= child_count()) return this;
                MultiChildWidget::set_child(index, child);
                _flexs[index] = flex;
                return this;
            }

            void FlowGridWidgetBase::remove_child(Widget* child) noexcept {
                if (!child) return;
                int child_index = -1;
                for (int i = 0; i < child_count(); i++) {
                    if (this->child(i) == child) {
                        child_index = i;
                        break;
                    }
                }
                if (child_index != -1)
                    remove_child(child_index);
            }
            void FlowGridWidgetBase::remove_child(int index) noexcept {
                if (index < 0 || index >= child_count()) return;
                MultiChildWidget::remove_child(index);
                _flexs.erase(_flexs.begin() + index);
            }

        } // namespace _details

        ColumnGridWidget::ColumnGridWidget() noexcept {}

        Size ColumnGridWidget::perform_layout(Constraint constraint) noexcept {
            int cw = constraint.max_width();
            int ch = constraint.max_height();
            float fsum = flex_sum();
            float current_hpos = 0.0f;

            for (int i = 0; i < child_count(); i++) {
                int nh = ch * (flex_of(i) / fsum);
                Constraint nc = Constraint::zero_to(cw, nh);
                child(i)->layout(nc);
                RolUI::set_pos(child(i), Point{0, int(current_hpos / fsum * ch)});
                current_hpos += flex_of(i);
            }
            return constraint.max();
        }

        RowGridWidget::RowGridWidget() noexcept {}
        Size RowGridWidget::perform_layout(Constraint constraint) noexcept {
            int cw = constraint.max_width();
            int ch = constraint.max_height();
            float fsum = flex_sum();
            float current_wpos = 0.0f;

            for (int i = 0; i < child_count(); i++) {
                int nw = cw * (flex_of(i) / fsum);
                Constraint nc = Constraint::zero_to(nw, ch);
                child(i)->layout(nc);
                RolUI::set_pos(child(i), Point{int(current_wpos / fsum * cw), 0});
                current_wpos += flex_of(i);
            }
            return constraint.max();
        }

        FlexWidget::FlexWidget() noexcept {}

        void FlexWidget::_layout_children(Constraint constraint) noexcept {
            int cons_width = constraint.max_width();
            int cons_height = constraint.max_height();
            bool is_row = direction() == Direction::row || direction() == Direction::row_reverse;
            bool is_reverse = direction() == Direction::row_reverse || direction() == Direction::column_reverse;
            int cons_main_size = is_row ? cons_width : cons_height;

            int main_size_it = cons_main_size;
            for (int i = 0; i < child_count(); i++) {
                Widget* child = is_reverse ? this->child(child_count() - 1 - i) : this->child(i);
                Point new_cons_size = is_row ? Point{main_size_it, cons_height} : Point{cons_width, main_size_it};
                Constraint new_cons = Constraint::zero_to(new_cons_size);
                Size child_size = child->layout(new_cons);
                main_size_it -= is_row ? child_size.width : child_size.height;
                if (main_size_it <= 0)
                    main_size_it = cons_main_size;
            }
        }
        Size FlexWidget::perform_layout(Constraint constraint) noexcept {
            int cons_width = constraint.max_width();
            int cons_height = constraint.max_height();
            bool is_row = direction() == Direction::row || direction() == Direction::row_reverse;
            bool is_reverse = direction() == Direction::row_reverse || direction() == Direction::column_reverse;
            int cons_main_size = is_row ? cons_width : cons_height;
            int cons_cross_size = !is_row ? cons_width : cons_height;

            int main_size_it = cons_main_size;

            _layout_children(constraint);

            int last_main_pos = 0;
            int last_cross_pos = 0;
            int start = 0;
            int end = 0;
            int current_cross_size_max = 0;

            while (start < this->child_count()) {
                current_cross_size_max = 0;
                main_size_it = 0;
                last_main_pos = 0;
                while (main_size_it < cons_main_size && end < this->child_count()) {
                    Widget* child = is_reverse ? this->child(child_count() - 1 - end) : this->child(end);
                    Size child_size = child->size();
                    main_size_it += is_row ? child_size.width : child_size.height;
                    if (main_size_it > cons_main_size && start < end)
                        break;

                    int t = !is_row ? child_size.width : child_size.height;
                    if (t > current_cross_size_max)
                        current_cross_size_max = t;
                    end++;
                }
                for (int i = start; i < end; i++) {
                    Widget* child = is_reverse ? this->child(child_count() - 1 - i) : this->child(i);
                    int child_main_size = is_row ? child->size().width : child->size().height;
                    int child_cross_size = is_row ? child->size().height : child->size().width;
                    int main_pos = last_main_pos;
                    int cross_pos = last_cross_pos + (cross_axis_alignment() + 1.0f) / 2.0f * (current_cross_size_max - child_cross_size);
                    Point pos = is_row ? Point{main_pos, cross_pos} : Point{cross_pos, main_pos};
                    RolUI::set_pos(child, pos);
                    last_main_pos += child_main_size;
                }
                last_cross_pos += current_cross_size_max;
                start = end;
            }

            return {constraint.max_width(), last_cross_pos};
        }

    } // namespace widgets
} // namespace RolUI