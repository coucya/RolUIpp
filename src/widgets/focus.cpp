
#include "RolUI/Application.hpp"
#include "RolUI/events/Widget_event.hpp"
#include "RolUI/widgets/focus.hpp"

namespace RolUI {
    namespace widgets {

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

    } // namespace widgets
} // namespace RolUI