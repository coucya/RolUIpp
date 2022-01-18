
#include "RolUI/widgets/PointerListener.hpp"
#include "RolUI/events/MouseEvent.hpp"
#include "RolUI/IPainter.hpp"

namespace RolUI {
    namespace widget {

        PointerListener::PointerListener() noexcept {
            _init_event_bind();
        }

        PointerListener::~PointerListener() {}

        void PointerListener::_init_event_bind() noexcept {
            add_listener(MouseEnterEvent_type(), [this](IEvent* event) {
                this->on_hover.emit(true);
                return true;
            });
            add_listener(MouseLeaveEvent_type(), [this](IEvent* event) {
                this->on_hover.emit(false);
                return true;
            });
            add_listener(MousePressEvent_type(), [this](IEvent* event) {
                MouseEvent* mouseEvent = (MouseEvent*)event;
                this->on_down(mouseEvent->pos());
                return true;
            });
            add_listener(MouseReleaseEvent_type(), [this](IEvent* event) {
                MouseEvent* mouseEvent = (MouseEvent*)event;
                this->on_up(mouseEvent->pos());
                return true;
            });
        }

        void PointerListener::on_draw(IPainter* painter) {
            // if (!painter) return;
            // painter->set_stroke_width(2);
            // painter->set_stroke_color({0, 0, 0});
            // painter->draw_roundedrect(Rect{{0, 0}, this->size()}, 0);
        }
        Size PointerListener::perlayout(Constraint constraint) {
            if (child_count() == 0) return Size{0, 0};

            Widget* w = get_child(0);
            if (w == nullptr) return Size{0, 0};

            Size s = RolUI::perlayout(w, constraint);
            RolUI::set_rect(w, {{0, 0}, s});

            return s;
        }

    } // namespace widget
} // namespace RolUI