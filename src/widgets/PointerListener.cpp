
#include "RolUI/widgets/PointerListener.hpp"
#include "RolUI/events/MouseEvent.hpp"
#include "RolUI/IPainter.hpp"

namespace RolUI {
    namespace widgets {

        PointerListener::PointerListener() noexcept {}

        PointerListener::~PointerListener() {}

        bool PointerListener::handle_event(IEvent* e) noexcept {
            if (!e) return false;

            MouseEvent* mouseEvent = (MouseEvent*)e;
            const EventType* et = e->event_type();

            if (et == MouseEnterEvent_type()) {
                this->on_hover.emit(true);
                return true;
            } else if (et == MouseLeaveEvent_type()) {
                _is_press = false;
                this->on_hover.emit(false);
                return true;
            } else if (et == MousePressEvent_type()) {
                _is_press = true;
                this->on_down.emit(mouseEvent->pos());
                return true;
            } else if (et == MouseReleaseEvent_type()) {
                this->on_up.emit(mouseEvent->pos());
                if (_is_press)
                    this->on_click.emit(mouseEvent->pos());
                _is_press = false;
                return true;
            } else if (et == MousePosEvent_type()) {
                _is_press = false;
                this->on_move.emit(mouseEvent->offset());
                return true;
            }
            return false;
        }

        // void PointerListener::on_draw(IPainter* painter) {
        //     if (!painter) return;
        //     painter->set_stroke_width(2);
        //     painter->set_stroke_color({0, 0, 0});
        //     painter->draw_roundedrect(Rect{{0, 0}, this->size()}, 0);
        //     SingleChildWidget::on_draw(painter);
        // }

    } // namespace widgets
} // namespace RolUI