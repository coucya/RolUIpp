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

        FlexableWidget::FlexableWidget() noexcept {}

        Size FlexableWidget::perform_layout(Constraint constraint) noexcept {
            return layout_child(constraint, [](Size) { return Point{0, 0}; });
        }

        ColumnGridWidget::ColumnGridWidget() noexcept {}

        Size ColumnGridWidget::perform_layout(Constraint constraint) noexcept {
            int cw = constraint.max_width();
            int ch = constraint.max_height();
            int current_pos = 0;
            int used_size = 0;

            int fixed_need_size = 0;
            int flex_need_size = 0;
            int expand_need_size = 0;
            int expand_child_count = 0;
            float flex_sum = 0.0f;

            for (int i = 0; i < child_count(); i++) {
                Widget* child = this->child(i);
                FlexableWidget* flexable = object_try_cast<FlexableWidget>(child);
                if (flexable) {
                    switch (flexable->fit()) {
                        case FlexFit::fixed: fixed_need_size += flexable->fixed(); break;
                        case FlexFit::percentage: fixed_need_size += flexable->percentage() * ch; break;
                        case FlexFit::expand: expand_child_count++; break;
                        case FlexFit::flex: flex_sum += flexable->flex(); break;
                    }
                } else {
                    flex_sum += 1.0f;
                }
            }

            flex_need_size = ch > fixed_need_size ? ch - fixed_need_size : 0;

            for (int i = 0; i < child_count(); i++) {
                Widget* child = this->child(i);
                FlexableWidget* flexable = object_try_cast<FlexableWidget>(child);
                Constraint nc;
                if (flexable && flexable->fit() == FlexFit::fixed) {
                    nc = Constraint::zero_to(cw, flexable->fixed());
                } else if (flexable && flexable->fit() == FlexFit::percentage) {
                    nc = Constraint::zero_to(cw, int(ch * flexable->percentage()));
                } else if (flexable && flexable->fit() == FlexFit::flex) {
                    nc = Constraint::zero_to(cw, int(flex_need_size * (flexable->flex() / flex_sum)));
                } else if (flexable && flexable->fit() == FlexFit::expand) {
                    continue;
                } else {
                    nc = Constraint::zero_to(cw, int(flex_need_size * (1.0f / flex_sum)));
                }
                child->layout(nc);
                used_size += child->size().height;
            }

            expand_need_size = ch > used_size ? ch - used_size : 0;
            for (int i = 0; i < child_count(); i++) {
                Widget* child = this->child(i);
                FlexableWidget* flexable = object_try_cast<FlexableWidget>(child);
                if (flexable && flexable->fit() == FlexFit::expand) {
                    int nh = (float(expand_need_size) / float(expand_child_count));
                    Constraint nc = Constraint::zero_to(Size{cw, nh});
                    child->layout(nc);
                }
            }

            for (int i = 0; i < child_count(); i++) {
                Widget* child = this->child(i);
                RolUI::set_pos(child, Point{0, current_pos});
                current_pos += child->size().height;
            }

            return constraint.max();
        }

        RowGridWidget::RowGridWidget() noexcept {}

        Size RowGridWidget::perform_layout(Constraint constraint) noexcept {
            int cw = constraint.max_width();
            int ch = constraint.max_height();
            int current_pos = 0;
            int used_size = 0;

            int fixed_need_size = 0;
            int flex_need_size = 0;
            int expand_need_size = 0;
            int expand_child_count = 0;
            float flex_sum = 0.0f;

            for (int i = 0; i < child_count(); i++) {
                Widget* child = this->child(i);
                FlexableWidget* flexable = object_try_cast<FlexableWidget>(child);
                if (flexable) {
                    switch (flexable->fit()) {
                        case FlexFit::fixed: fixed_need_size += flexable->fixed(); break;
                        case FlexFit::percentage: fixed_need_size += flexable->percentage() * ch; break;
                        case FlexFit::expand: expand_child_count++; break;
                        case FlexFit::flex: flex_sum += flexable->flex(); break;
                    }
                } else {
                    flex_sum += 1.0f;
                }
            }

            flex_need_size = cw > fixed_need_size ? cw - fixed_need_size : 0;

            for (int i = 0; i < child_count(); i++) {
                Widget* child = this->child(i);
                FlexableWidget* flexable = object_try_cast<FlexableWidget>(child);
                Constraint nc;
                if (flexable && flexable->fit() == FlexFit::fixed) {
                    nc = Constraint::zero_to(flexable->fixed(), ch);
                } else if (flexable && flexable->fit() == FlexFit::percentage) {
                    nc = Constraint::zero_to(int(cw * flexable->percentage()), ch);
                } else if (flexable && flexable->fit() == FlexFit::flex) {
                    nc = Constraint::zero_to(int(flex_need_size * (flexable->flex() / flex_sum)), ch);
                } else if (flexable && flexable->fit() == FlexFit::expand) {
                    continue;
                } else {
                    nc = Constraint::zero_to(int(flex_need_size * (1.0f / flex_sum)), ch);
                }
                child->layout(nc);
                used_size += child->size().width;
            }

            expand_need_size = cw > used_size ? cw - used_size : 0;
            for (int i = 0; i < child_count(); i++) {
                Widget* child = this->child(i);
                FlexableWidget* flexable = object_try_cast<FlexableWidget>(child);
                if (flexable && flexable->fit() == FlexFit::expand) {
                    int nw = (float(expand_need_size) / float(expand_child_count));
                    Constraint nc = Constraint::zero_to(Size{nw, ch});
                    child->layout(nc);
                }
            }

            for (int i = 0; i < child_count(); i++) {
                Widget* child = this->child(i);
                RolUI::set_pos(child, Point{current_pos, 0});
                current_pos += child->size().width;
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

        const ObjectType* ColumnWidget::object_type() const noexcept { return object_type_of<ColumnWidget>(); }
        const ObjectType* RowWidget::object_type() const noexcept { return object_type_of<RowWidget>(); }
        const ObjectType* FlexableWidget::object_type() const noexcept { return object_type_of<FlexableWidget>(); }
        const ObjectType* ColumnGridWidget::object_type() const noexcept { return object_type_of<ColumnGridWidget>(); }
        const ObjectType* RowGridWidget::object_type() const noexcept { return object_type_of<RowGridWidget>(); }
        const ObjectType* FlexWidget::object_type() const noexcept { return object_type_of<FlexWidget>(); }

    } // namespace widgets

    RolUI_impl_object_type_of_with_namespace(widgets, ColumnWidget, MultiChildWidget);
    RolUI_impl_object_type_of_with_namespace(widgets, RowWidget, MultiChildWidget);
    RolUI_impl_object_type_of_with_namespace(widgets, FlexableWidget, SingleChildWidget);
    RolUI_impl_object_type_of_with_namespace(widgets, ColumnGridWidget, MultiChildWidget);
    RolUI_impl_object_type_of_with_namespace(widgets, RowGridWidget, MultiChildWidget);
    RolUI_impl_object_type_of_with_namespace(widgets, FlexWidget, MultiChildWidget);

} // namespace RolUI