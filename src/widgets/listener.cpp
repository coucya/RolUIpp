
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

            MouseEvent* me = (MouseEvent*)e;
            const EventType* et = e->event_type();

            if (et == MouseEnterEvent_type()) {
                this->on_hover.emit(true);
                return true;
            } else if (et == MouseLeaveEvent_type()) {
                this->on_hover.emit(false);
                return true;
            } else if (et == MousePressEvent_type() && me->action() == MouseKey::left) {
                _is_press = true;
                this->on_down.emit(me->pos());
                return true;
            } else if (et == MouseReleaseEvent_type() && me->action() == MouseKey::left) {
                this->on_up.emit(me->pos());
                if (_is_press)
                    this->on_click.emit(me->pos());
                return true;
            } else if (et == MousePosEvent_type()) {
                _is_press = false;
                this->on_move.emit(me->offset());
                if (me->button(MouseKey::left) == MouseKeyMode::press)
                    this->on_drag.emit(me->offset());
                return true;
            }
            return false;
        }

        void PointerListenerWidget::draw(IPainter* painter) noexcept {
            SingleChildWidget::draw(painter);
            // painter->set_stroke_color({255, 0, 0});
            // painter->set_stroke_width(2);
            // painter->draw_rect(abs_rect());
        }

        MouseAreaWidget::MouseAreaWidget() noexcept {}
        MouseAreaWidget::~MouseAreaWidget() {}

        bool MouseAreaWidget::handle_event(IEvent* e) noexcept {
            if (!e) return false;

            MouseEvent* me = (MouseEvent*)e;
            const EventType* et = e->event_type();

            if (et == MouseEnterEvent_type()) {
                this->on_hover.emit(true);
                return true;
            } else if (et == MouseLeaveEvent_type()) {
                this->on_hover.emit(false);
                return true;
            } else if (et == MousePressEvent_type()) {
                _is_press[(int)me->action()] = true;
                this->on_down.emit(me->action(), me->pos());
                return true;
            } else if (et == MouseReleaseEvent_type()) {
                this->on_up.emit(me->action(), me->pos());
                if (_is_press[(int)me->action()])
                    this->on_click.emit(me->action(), me->pos());
                _is_press[(int)me->action()] = false;
                return true;
            } else if (et == MousePosEvent_type()) {
                for (int i = 0; i < sizeof(_is_press) / sizeof(bool); i++)
                    _is_press[i] = false;

                this->on_move.emit(me->offset());

                if (me->button(MouseKey::left) == MouseKeyMode::press)
                    this->on_drag.emit(MouseKey::left, me->offset());
                if (me->button(MouseKey::middle) == MouseKeyMode::press)
                    this->on_drag.emit(MouseKey::middle, me->offset());
                if (me->button(MouseKey::right) == MouseKeyMode::press)
                    this->on_drag.emit(MouseKey::right, me->offset());
                if (me->button(MouseKey::key4) == MouseKeyMode::press)
                    this->on_drag.emit(MouseKey::key4, me->offset());
                if (me->button(MouseKey::key5) == MouseKeyMode::press)
                    this->on_drag.emit(MouseKey::key5, me->offset());
                if (me->button(MouseKey::key6) == MouseKeyMode::press)
                    this->on_drag.emit(MouseKey::key6, me->offset());
                if (me->button(MouseKey::key7) == MouseKeyMode::press)
                    this->on_drag.emit(MouseKey::key7, me->offset());
                if (me->button(MouseKey::key8) == MouseKeyMode::press)
                    this->on_drag.emit(MouseKey::key8, me->offset());

                return true;
            } else if (et == MouseScrollEvent_type()) {
                MouseScrollEvent* me = (MouseScrollEvent*)e;
                this->on_wheel.emit(me->offset());
            }
            return false;
        }
        void MouseAreaWidget::draw(IPainter* painter) noexcept {
            SingleChildWidget::draw(painter);
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