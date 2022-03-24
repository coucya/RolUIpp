#include <algorithm>
#include <cmath>
#include <limits>

#include "RolUI/Point.hpp"
#include "RolUI/IPainter.hpp"
#include "RolUI/widgets/Scroll.hpp"
#include "RolUI/events/MouseEvent.hpp"

namespace RolUI {
    namespace widgets {

        ScrollWidget::ScrollWidget() noexcept {}

        float ScrollWidget::widget_x_ratio() const noexcept {
            if (!child()) return 0.0;

            Size self_size = size();
            Size widget_size = child()->size();

            int w_dis = self_size.width - widget_size.width;

            int wx = offset().x;
            if (w_dis == 0 && wx == 0)
                return 0.0f;
            else if (w_dis == 0 && wx < 0)
                return -std::numeric_limits<float>::infinity();
            else if (w_dis == 0 && wx > 0)
                return +std::numeric_limits<float>::infinity();
            return float(wx) / float(w_dis);
        }
        float ScrollWidget::widget_y_ratio() const noexcept {
            if (!child()) return 0.0;

            Size self_size = size();
            Size widget_size = child()->size();

            int h_dis = self_size.height - widget_size.height;

            int wy = offset().y;
            if (h_dis == 0 && wy == 0)
                return 0.0f;
            else if (h_dis == 0 && wy < 0)
                return -std::numeric_limits<float>::infinity();
            else if (h_dis == 0 && wy > 0)
                return +std::numeric_limits<float>::infinity();
            return float(wy) / float(h_dis);
        }

        void ScrollWidget::scroll_by_px(int dx, int dy) noexcept {
            if (!child()) return;
            offset = offset.get() + Point{dx, dy};
        }
        void ScrollWidget::scroll_by_ratio(float x, float y) noexcept {
            if (!child()) return;
            Size self_size = size();
            Size widget_size = child()->size();

            int w_dis = self_size.width - widget_size.width;
            int h_dis = self_size.height - widget_size.height;

            scroll_by_px(x * w_dis, y * h_dis);
        }

        void ScrollWidget::scroll_x_by_px(int dx) noexcept {
            scroll_by_px(dx, 0);
        }

        void ScrollWidget::scroll_y_by_px(int dy) noexcept {
            scroll_by_px(0, dy);
        }
        void ScrollWidget::scroll_x_by_ratio(float x) noexcept {
            scroll_by_ratio(x, 0);
        }
        void ScrollWidget::scroll_y_by_ratio(float y) noexcept {
            scroll_by_ratio(0, y);
        }

        void ScrollWidget::scroll_to_px(int x, int y) noexcept {
            if (!child()) return;
            offset = {x, y};
        }
        void ScrollWidget::scroll_to_ratio(float x, float y) noexcept {
            if (!child()) return;
            Size self_size = size();
            Size widget_size = child()->size();

            int w_dis = self_size.width - widget_size.width;
            int h_dis = self_size.height - widget_size.height;

            scroll_to_px(w_dis * x, h_dis * y);
        }
        void ScrollWidget::scroll_x_to_px(int x) noexcept { scroll_to_px(x, offset().y); }
        void ScrollWidget::scroll_y_to_px(int y) noexcept { scroll_to_px(offset().x, y); }
        void ScrollWidget::scroll_x_to_ratio(float x) noexcept {
            if (!child()) return;
            Size self_size = size();
            Size widget_size = child()->size();
            int w_dis = self_size.width - widget_size.width;
            scroll_to_px(w_dis * x, offset().y);
        }
        void ScrollWidget::scroll_y_to_ratio(float y) noexcept {
            if (!child()) return;
            Size self_size = size();
            Size widget_size = child()->size();
            int h_dis = self_size.height - widget_size.height;
            scroll_to_px(offset().x, h_dis * y);
        }

        Size ScrollWidget::perform_layout(Constraint constraint) noexcept {
            int n = std::numeric_limits<int>::max();
            layout_child(Constraint::zero_to({n, n}), [this](Size) {
                return offset();
            });
            return constraint.max();
        }
        void ScrollWidget::draw(IPainter* painter) noexcept {
            if (!child()) return;

            RolUI::Rect ar = abs_rect();
            RolUI::Rect current_scissor = painter->get_scissor();
            painter->scissor(
                current_scissor
                    .intersected(ar)
                    .value_or(RolUI::Rect{ar.pos(), Size{0, 0}}));
            child()->draw(painter);
            painter->scissor(current_scissor);
        }

        VScrollView::VScrollView() noexcept {}

        Size VScrollView::perform_layout(Constraint constraint) noexcept {
            int n = std::numeric_limits<int>::max();
            layout_child(Constraint::zero_to({constraint.max_width(), n}), [this](Size) {
                return offset();
            });
            return constraint.max();
        }
        bool VScrollView::handle_event(IEvent* e) noexcept {
            if (e->is(MouseWheelEvent_type())) {
                MouseWheelEvent* mwe = static_cast<MouseWheelEvent*>(e);
                int dy = float(mwe->offset().y) * scroll_step();
                scroll_y_by_px(dy);
                float yr = widget_y_ratio();
                if (yr <= 0.0)
                    scroll_y_to_ratio(0.0);
                if (yr >= 1.0)
                    scroll_y_to_ratio(1.0);
                return true;
            }
            return false;
        }

        HScrollView::HScrollView() noexcept {}

        Size HScrollView::perform_layout(Constraint constraint) noexcept {
            int n = std::numeric_limits<int>::max();
            layout_child(Constraint::zero_to({n, constraint.max_height()}), [this](Size) {
                return offset();
            });
            return constraint.max();
        }
        bool HScrollView::handle_event(IEvent* e) noexcept {
            if (e->is(MouseWheelEvent_type())) {
                MouseWheelEvent* mwe = static_cast<MouseWheelEvent*>(e);
                int dy = float(mwe->offset().y) * scroll_step();
                scroll_x_by_px(dy);
                float xr = widget_x_ratio();
                if (xr <= 0.0)
                    scroll_x_to_ratio(0.0);
                if (xr >= 1.0)
                    scroll_x_to_ratio(1.0);
                return true;
            }
            return false;
        }

    } // namespace widgets
} // namespace RolUI
