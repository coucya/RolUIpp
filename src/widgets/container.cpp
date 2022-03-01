
#include "RolUI/Point.hpp"
#include "RolUI/Size.hpp"
#include "RolUI/IEvent.hpp"
#include "RolUI/IPainter.hpp"
#include "RolUI/widgets/container.hpp"

namespace RolUI {
    namespace widgets {

        Box::Box(unsigned round) noexcept { this->round = round; }
        Box::~Box() {}

        void Box::draw(IPainter* painter) noexcept {
            Rect rect = abs_rect();
            painter->set_fill_color(background_color);
            painter->fill_roundedrect(rect, round);

            if (border_width > 0) {
                painter->set_stroke_color(border_color);
                painter->set_stroke_width(border_width * 2);
                painter->draw_roundedrect(rect, round);
            }

            SingleChildWidget::draw(painter);
        }

        SizedBox::SizedBox() noexcept {}
        SizedBox::SizedBox(SizeUnit w, SizeUnit h) noexcept {
            width = w, height = h;
        }

        Size SizedBox::perform_layout(Constraint constraint) noexcept {
            int w = width().type() == SizeUnitType::pixel
                      ? width().pixel()
                      : width().percentage() * constraint.max_width();
            int h = height().type() == SizeUnitType::pixel
                      ? height().pixel()
                      : height().percentage() * constraint.max_height();
            layout_child(Constraint::zero_to({w, h}), [](Size) { return Point{0, 0}; });
            return {w, h};
        }

        Align::Align() noexcept {}
        Align::Align(float x, float y) noexcept {
            align_x = x;
            align_y = y;
        }

        Size Align::perform_layout(Constraint constraint) noexcept {
            Size self_size = constraint.max();

            layout_child(constraint, [&](Size s) {
                int cx = float(self_size.width - s.width) * align_x.get();
                int cy = float(self_size.height - s.height) * align_y.get();
                return Point{cx, cy};
            });

            return self_size;
        }

        Margin::Margin() noexcept {}
        Margin::Margin(unsigned margin) noexcept
            : top(this, margin), bottom(this, margin), left(this, margin), right(this, margin) {}
        Margin::Margin(unsigned x, unsigned y) noexcept
            : top(this, y), bottom(this, y), left(this, x), right(this, x) {}
        Margin::Margin(unsigned top, unsigned bottom, unsigned left, unsigned right) noexcept
            : top(this, top), bottom(this, bottom), left(this, left), right(this, right) {}

        Size Margin::perform_layout(Constraint constraint) noexcept {
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

    } // namespace widgets
} // namespace RolUI
