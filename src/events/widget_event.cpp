
#include "RolUI/IEvent.hpp"
#include "RolUI/events/Widget_event.hpp"

namespace RolUI {

    RolUI_impl_object_type_of(ParentChangeEvent, IEvent);
    RolUI_impl_object_type_of(PosChangeEvent, IEvent);
    RolUI_impl_object_type_of(SizeChangeEvent, IEvent);
    RolUI_impl_object_type_of(FocusChangeEvent, IEvent);

    const ObjectType* ParentChangeEvent::object_type() const noexcept { return object_type_of<ParentChangeEvent>(); }
    const ObjectType* PosChangeEvent::object_type() const noexcept { return object_type_of<PosChangeEvent>(); }
    const ObjectType* SizeChangeEvent::object_type() const noexcept { return object_type_of<SizeChangeEvent>(); }
    const ObjectType* FocusChangeEvent::object_type() const noexcept { return object_type_of<FocusChangeEvent>(); }

} // namespace RolUI
