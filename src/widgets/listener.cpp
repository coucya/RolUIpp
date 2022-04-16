
#include "RolUI/IPainter.hpp"
#include "RolUI/Application.hpp"
#include "RolUI/widgets/listener.hpp"

#include "RolUI/events/MouseEvent.hpp"
#include "RolUI/events/KeyboardEvent.hpp"
#include "RolUI/events/CharEvent.hpp"
#include "RolUI/events/Widget_event.hpp"

namespace RolUI {
    namespace widgets {

        PointerListener::PointerListener() noexcept {}

        PointerListener::~PointerListener() {}

        bool PointerListener::handle_event(IEvent* e) noexcept {
            if (!e) return false;

            MouseEvent* me = (MouseEvent*)e;
            const ObjectType* et = e->object_type();

            if (et == object_type_of<MouseEnterEvent>()) {
                this->on_hover.emit(true);
                return true;
            } else if (et == object_type_of<MouseLeaveEvent>()) {
                this->on_hover.emit(false);
                return true;
            } else if (et == object_type_of<MousePressEvent>() && me->action() == MouseKey::left) {
                _is_press = true;
                this->on_down.emit(me->pos());
                return true;
            } else if (et == object_type_of<MouseReleaseEvent>() && me->action() == MouseKey::left) {
                this->on_up.emit(me->pos());
                if (_is_press)
                    this->on_click.emit(me->pos());
                return true;
            } else if (et == object_type_of<MouseMoveEvent>()) {
                _is_press = false;
                this->on_move.emit(me->offset());
                if (me->button(MouseKey::left) == MouseKeyMode::press)
                    this->on_drag.emit(me->offset());
                return true;
            }
            return false;
        }

        MouseListener::MouseListener() noexcept {}
        MouseListener::~MouseListener() {}

        bool MouseListener::handle_event(IEvent* e) noexcept {
            if (!e) return false;

            MouseEvent* me = (MouseEvent*)e;
            const ObjectType* et = e->object_type();

            if (et == object_type_of<MouseEnterEvent>()) {
                this->on_hover.emit(true);
                return true;
            } else if (et == object_type_of<MouseLeaveEvent>()) {
                this->on_hover.emit(false);
                return true;
            } else if (et == object_type_of<MousePressEvent>()) {
                _is_press[(int)me->action()] = true;
                this->on_down.emit(me->action(), me->pos());
                return true;
            } else if (et == object_type_of<MouseReleaseEvent>()) {
                this->on_up.emit(me->action(), me->pos());
                if (_is_press[(int)me->action()])
                    this->on_click.emit(me->action(), me->pos());
                _is_press[(int)me->action()] = false;
                return true;
            } else if (et == object_type_of<MouseMoveEvent>()) {
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
            } else if (et == object_type_of<MouseWheelEvent>()) {
                MouseWheelEvent* me = (MouseWheelEvent*)e;
                this->on_wheel.emit(me->offset());
            }
            return false;
        }

        KeyboardListener::KeyboardListener() noexcept {}
        KeyboardListener::~KeyboardListener() {}

        bool KeyboardListener::handle_event(IEvent* e) noexcept {
            if (e->object_type_is<KeyboardEvent>()) {
                KeyboardEvent* ke = static_cast<KeyboardEvent*>(e);
                on_key.emit(ke->action(), ke->key_mode());
                return true;
            }
            return false;
        }

        FocusListener::FocusListener() noexcept {}

        void FocusListener::focus() noexcept {
            Application::set_focus_widget(this);
        }

        void FocusListener::unfocus() noexcept {
            if (Application::focus_widget() == static_cast<Widget*>(this))
                Application::set_focus_widget(nullptr);
        }

        bool FocusListener::handle_event(IEvent* e) noexcept {
            if (e->object_type_is<FocusChangeEvent>()) {
                FocusChangeEvent* event = static_cast<FocusChangeEvent*>(e);
                on_focus.emit(event->current_value());
                return true;
            }
            return false;
        }

        CharInputListener::CharInputListener() noexcept {}

        bool CharInputListener::handle_event(IEvent* e) noexcept {
            if (e->object_type_is<CharEvent>()) {
                CharEvent* ce = static_cast<CharEvent*>(e);
                uint32_t cp = ce->codepoint();
                on_input.emit(cp);
                return true;
            }
            return false;
        }

        const ObjectType* PointerListener::object_type() const noexcept { return object_type_of<PointerListener>(); }
        const ObjectType* MouseListener::object_type() const noexcept { return object_type_of<MouseListener>(); }
        const ObjectType* KeyboardListener::object_type() const noexcept { return object_type_of<KeyboardListener>(); }
        const ObjectType* FocusListener::object_type() const noexcept { return object_type_of<FocusListener>(); }
        const ObjectType* CharInputListener::object_type() const noexcept { return object_type_of<CharInputListener>(); }

    } // namespace widgets

    RolUI_impl_object_type_of_with_namespace(widgets, PointerListener, SingleChildWidget);
    RolUI_impl_object_type_of_with_namespace(widgets, MouseListener, SingleChildWidget);
    RolUI_impl_object_type_of_with_namespace(widgets, KeyboardListener, SingleChildWidget);
    RolUI_impl_object_type_of_with_namespace(widgets, FocusListener, SingleChildWidget);
    RolUI_impl_object_type_of_with_namespace(widgets, CharInputListener, SingleChildWidget);

} // namespace RolUI