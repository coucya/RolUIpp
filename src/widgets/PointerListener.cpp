
#include "RolUI/widgets/PointerListener.hpp"
#include "RolUI/events/MouseEvent.hpp"
#include "RolUI/IPainter.hpp"

namespace RolUI {
    namespace widgets {

        PointerListenerWidget::PointerListenerWidget() noexcept {}

        PointerListenerWidget::~PointerListenerWidget() {}

        bool PointerListenerWidget::handle_event(IEvent* e) noexcept {
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

    } // namespace widgets
} // namespace RolUI