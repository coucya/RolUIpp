#include "RolUI/Object.hpp"
#include "RolUI/IEvent.hpp"
#include "RolUI/Widget.hpp"

namespace RolUI {

    bool send_event(Widget* w, IEvent* e) {
        if (!e || !w || !w->mounted()) return false;
        e->_target = w;
        return w->handle_event(e);
    }

    RolUI_impl_object_type_of(IEvent, Object);

    IEvent::IEvent(Widget* target) noexcept : _target(target) {}
    IEvent::~IEvent() {}

    const ObjectType* IEvent::object_type() const noexcept { return object_type_of<IEvent>(); }

    Widget* IEvent::target() const noexcept { return _target; }

} // namespace RolUI
