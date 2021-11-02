
#include "RolUI/events/Widget_event.hpp"
#include "RolUI/IEvent.hpp"

namespace RolUI {

    RolUI_impl_event_type_in_class(WindowChangeEvent);
    Window* WindowChangeEvent::current_window() const noexcept { return _current_window; }
    Window* WindowChangeEvent::old_window() const noexcept { return _old_window; }

    RolUI_impl_event_type_in_class(ParentChangeEvent);
    Widget* ParentChangeEvent::current_parent() const noexcept { return _current_parent; }
    Widget* ParentChangeEvent::old_parent() const noexcept { return _old_parent; }

} // namespace RolUI
