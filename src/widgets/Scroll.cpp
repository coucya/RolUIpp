
#include <algorithm>

#include "RolUI/Point.hpp"
#include "RolUI/widgets/Scroll.hpp"
#include "RolUI/Widget.hpp"

namespace RolUI {
    namespace widget {

        Scroll::Scroll() noexcept {}
        Scroll::Scroll(Widget* parent) noexcept
            : Widget(parent) {}

        Widget* Scroll::widget() const noexcept { return _widget; }
        void Scroll::set_widget(Widget* widget) noexcept {
            if (_widget)
                remove_widget(_widget);

            _widget = widget;
            if (widget) {
                _widget->set_pos_relative(RelativeTarget::parent, AnchorPoint::left_top, AnchorPoint::left_top);
                _widget->set_pos(0, 0);
                add_child(widget);
            }
        }

        void Scroll::scroll_by_px(int dx, int dy) noexcept {
            scroll_x_by_px(dx);
            scroll_y_by_px(dy);
        }
        void Scroll::scroll_by_ratio(float x, float y) noexcept {
            scroll_x_by_ration(x);
            scroll_y_by_ration(y);
        }

        void Scroll::scroll_x_by_px(int dx) noexcept {
            if (!_widget) return;
            int self_w = size().width;
            int widget_w = _widget->size().width;
            Point widget_pos = _widget->pos();

            int dis = widget_w >= self_w ? widget_w - self_w : 0;

            int x = std::clamp(widget_pos.x + dx, -dis, 0);
            _widget->move(x - widget_pos.x, 0);
            // _widget->move(dx, 0);
        }
        void Scroll::scroll_y_by_px(int dy) noexcept {
            if (!_widget) return;
            int self_h = size().height;
            int widget_h = _widget->size().height;
            Point widget_pos = _widget->pos();

            int dis = widget_h >= self_h ? widget_h - self_h : 0;

            int y = std::clamp(widget_pos.y + dy, -dis, 0);
            _widget->move(0, y - widget_pos.y);
            // _widget->move(0, dy);
        }
        void Scroll::scroll_x_by_ration(float dx) noexcept {
            if (!_widget) return;
            int self_w = size().width;
            int widget_w = _widget->size().width;

            int dis = widget_w >= self_w ? widget_w - self_w : 0;
            scroll_x_by_px((int)(dis * dx));
        }
        void Scroll::scroll_y_by_ration(float dy) noexcept {
            if (!_widget) return;
            int self_h = size().height;
            int widget_h = _widget->size().height;

            int dis = widget_h >= self_h ? widget_h - self_h : 0;
            scroll_y_by_px((int)(dis * dy));
        }

    } // namespace widget
} // namespace RolUI
