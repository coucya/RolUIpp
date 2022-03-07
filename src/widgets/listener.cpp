
#include "RolUI/IPainter.hpp"
#include "RolUI/Application.hpp"
#include "RolUI/widgets/listener.hpp"

#include "RolUI/events/MouseEvent.hpp"
#include "RolUI/events/CharEvent.hpp"
#include "RolUI/events/Widget_event.hpp"

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
                if (mouseEvent->button(MouseKey::left) == MouseKeyMode::press)
                    this->on_drag(mouseEvent->offset());
                return true;
            } else if (et == MouseScrollEvent_type()) {
                this->on_scroll.emit(((MouseScrollEvent*)e)->offset());
            }
            return false;
        }

        void PointerListenerWidget::draw(IPainter* painter) noexcept {
            SingleChildWidget::draw(painter);
            // painter->set_stroke_color({255, 0, 0});
            // painter->set_stroke_width(2);
            // painter->draw_rect(abs_rect());
        }
        FocusWidget::FocusWidget() noexcept {}

        void FocusWidget::focus() noexcept {
            Application::set_focus_widget(this);
        }

        void FocusWidget::unfocus() noexcept {
            if (Application::focus_widget() == static_cast<Widget*>(this))
                Application::set_focus_widget(nullptr);
        }

        bool FocusWidget::handle_event(IEvent* e) noexcept {
            if (e->event_type() == FocusChangeEvent::type()) {
                FocusChangeEvent* event = static_cast<FocusChangeEvent*>(e);
                on_focus.emit(event->current_value());
                return true;
            }
            return false;
        }

        CharInputWidget::CharInputWidget() noexcept {}

        bool CharInputWidget::handle_event(IEvent* e) noexcept {
            if (e->event_type() == CharEvent::type()) {
                CharEvent* ce = static_cast<CharEvent*>(e);
                uint32_t cp = ce->codepoint();
                on_input.emit(cp);
                return true;
            }
            return false;
        }

    } // namespace widgets
} // namespace RolUI