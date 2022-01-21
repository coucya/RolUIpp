
#include "RolUI/widgets/PointerListener.hpp"
#include "RolUI/events/MouseEvent.hpp"
#include "RolUI/IPainter.hpp"

namespace RolUI {
    namespace widget {

        PointerListener::PointerListener() noexcept {}

        PointerListener::~PointerListener() {}

        bool PointerListener::handle_event(IEvent* e) {
            if (!e) return false;

            MouseEvent* mouseEvent = (MouseEvent*)e;
            const EventType* et = e->event_type();

            if (et == MouseEnterEvent_type()) {
                this->on_hover.emit(true);
                return true;
            } else if (et == MouseLeaveEvent_type()) {
                this->on_hover.emit(false);
                return true;
            } else if (et == MousePressEvent_type()) {
                this->on_down(mouseEvent->pos());
                return true;
            } else if (et == MouseReleaseEvent_type()) {
                this->on_up(mouseEvent->pos());
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

    } // namespace widget
} // namespace RolUI