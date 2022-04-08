#pragma once

#include "../IEvent.hpp"
#include "../Point.hpp"
#include "../Size.hpp"

namespace RolUI {

    class Window;
    class Widget;

    namespace _details {

        template <typename T>
        class PropertyChangeEvent : public IEvent {
          public:
            PropertyChangeEvent(Widget* target, T current, T old) noexcept
                : IEvent(target), _current(current), _old(old) {}

            T current_value() const noexcept { return _current; }
            T old_value() const noexcept { return _old; }

          private:
            T _current;
            T _old;
        };

    } // namespace _details

#define RulUI_define_property_change_event(name, property_type)                 \
    class name : public _details::PropertyChangeEvent<property_type> {          \
      public:                                                                   \
        name(Widget* target, property_type current, property_type old) noexcept \
            : PropertyChangeEvent<property_type>(target, current, old) {}       \
        const ObjectType* object_type() const noexcept override;                \
    };                                                                          \
    RolUI_decl_object_type_of(name)

    RulUI_define_property_change_event(ParentChangeEvent, Widget*);

    RulUI_define_property_change_event(PosChangeEvent, Point);

    RulUI_define_property_change_event(SizeChangeEvent, Size);

    RulUI_define_property_change_event(FocusChangeEvent, bool);

#undef RulUI_define_property_change_event

} // namespace RolUI
