
#include "RolUI/Application.hpp"
#include "RolUI/events/Widget_event.hpp"
#include "RolUI/widgets/Focus.hpp"

namespace RolUI {
    namespace widget {

        Focus::Focus() noexcept {}

        void Focus::focus() noexcept {
            Application::set_focus_widget(this);
        }
        void Focus::unfocus() noexcept {
            if (Application::focus_widget() == static_cast<Widget*>(this))
                Application::set_focus_widget(nullptr);
        }

        bool Focus::handle_event(IEvent* e) {
            if (e->event_type() == FocusChangeEvent::type()) {
                FocusChangeEvent* event = static_cast<FocusChangeEvent*>(e);
                on_focus.emit(event->current_value());
                return true;
            }
            return false;
        }

    } // namespace widget
} // namespace RolUI