
#include "RolUI/Point.hpp"
#include "RolUI/Size.hpp"
#include "RolUI/IEvent.hpp"
#include "RolUI/IPainter.hpp"
#include "RolUI-widgets/container.hpp"

namespace RolUI {
    namespace widgets {

        BoxWidget::BoxWidget() noexcept {}

        void BoxWidget::perform_draw(IPainter* painter) noexcept {
            Rect rect = abs_rect();
            painter->set_fill_color(background_color);
            painter->fill_roundedrect(rect, round);

            if (border_width > 0) {
                painter->set_line_color(border_color);
                painter->set_line_width(border_width);
                painter->stroke_roundedrect(rect, round);
            }

            SingleChildWidget::perform_draw(painter);
        }
        Size BoxWidget::perform_layout(Constraint constraint) noexcept {
            if (child_count() == 0) return constraint.max();
            return layout_child(constraint, [](Size) { return Point{0, 0}; });
        }

        SizedWidget::SizedWidget() noexcept {}
        SizedWidget::SizedWidget(SizeUnit w, SizeUnit h) noexcept {
            width = w, height = h;
        }

        Size SizedWidget::perform_layout(Constraint constraint) noexcept {
            int w = width().type() == SizeUnitType::pixel
                      ? width().pixel()
                      : width().percentage() * constraint.max_width();
            int h = height().type() == SizeUnitType::pixel
                      ? height().pixel()
                      : height().percentage() * constraint.max_height();
            layout_child(Constraint::zero_to({w, h}), [](Size) { return Point{0, 0}; });
            return {w, h};
        }

        AlignWidget::AlignWidget() noexcept {}
        AlignWidget::AlignWidget(float x, float y) noexcept {
            align_x = x;
            align_y = y;
        }

        Size AlignWidget::perform_layout(Constraint constraint) noexcept {
            Size self_size = constraint.max();

            layout_child(constraint, [&](Size s) {
                int cx = float(self_size.width - s.width) * (align_x() + 1.0f) / 2.0f;
                int cy = float(self_size.height - s.height) * (align_y() + 1.0f) / 2.0f;
                return Point{cx, cy};
            });

            return self_size;
        }

        MarginWidget::MarginWidget() noexcept {}
        MarginWidget::MarginWidget(unsigned margin) noexcept
            : top(this, margin), bottom(this, margin), left(this, margin), right(this, margin) {}
        MarginWidget::MarginWidget(unsigned x, unsigned y) noexcept
            : top(this, y), bottom(this, y), left(this, x), right(this, x) {}
        MarginWidget::MarginWidget(unsigned top, unsigned bottom, unsigned left, unsigned right) noexcept
            : top(this, top), bottom(this, bottom), left(this, left), right(this, right) {}

        Size MarginWidget::perform_layout(Constraint constraint) noexcept {
            Size m = constraint.max();
            int cw = std::max(m.width - int(left() + right()), 0);
            int ch = std::max(m.height - int(top() + bottom()), 0);
            int cx = left();
            int cy = top();

            Size child_size = layout_child(Constraint::zero_to(cw, ch), [&](Size s) {
                return Point{cx, cy};
            });

            Size self_size;
            if (this->child())
                self_size = {int(left() + right() + child_size.width), int(top() + bottom() + child_size.height)};
            else
                self_size = {int(left() + right()), int(top() + bottom())};
            return self_size;
        }

        const ObjectType* BoxWidget::object_type() const noexcept { return object_type_of<BoxWidget>(); }
        const ObjectType* SizedWidget::object_type() const noexcept { return object_type_of<SizedWidget>(); }
        const ObjectType* AlignWidget::object_type() const noexcept { return object_type_of<AlignWidget>(); }
        const ObjectType* MarginWidget::object_type() const noexcept { return object_type_of<MarginWidget>(); }

    } // namespace widgets

    RolUI_impl_object_type_of_with_namespace(widgets, BoxWidget, SingleChildWidget);
    RolUI_impl_object_type_of_with_namespace(widgets, SizedWidget, SingleChildWidget);
    RolUI_impl_object_type_of_with_namespace(widgets, AlignWidget, SingleChildWidget);
    RolUI_impl_object_type_of_with_namespace(widgets, MarginWidget, SingleChildWidget);

} // namespace RolUI
