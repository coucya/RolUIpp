#pragma once

#include "RolUI/IEvent.hpp"
#include "RolUI/Point.hpp"
#include "RolUI/Size.hpp"
#include "RolUI/events/Widget_event.hpp"
#include <optional>

namespace RolUI {

    class Window;
    class Widget;

    namespace _details {

        template <typename T>
        class PropertyChangeEvent : public IEvent {
          public:
            PropertyChangeEvent(const EventType* et, Widget* target, T current, T old) noexcept
                : IEvent(et, target), _current(current), _old(old) {}

            T current_value() const noexcept { return _current; }
            T old_value() const noexcept { return _old; }

          private:
            T _current;
            T _old;
        };

    } // namespace _details

#define RulUI_define_property_change_event(name, property_type)                   \
    class name : public _details::PropertyChangeEvent<property_type> {            \
      public:                                                                     \
        RolUI_decl_event_type_in_class(name);                                     \
                                                                                  \
        name(Widget* target, property_type current, property_type old) noexcept   \
            : PropertyChangeEvent<property_type>(type(), target, current, old) {} \
    }

    RulUI_define_property_change_event(WindowChangeEvent, Window*);

    RulUI_define_property_change_event(ParentChangeEvent, Widget*);

    RulUI_define_property_change_event(PosChangeEvent, Point);

    RulUI_define_property_change_event(SizeChangeEvent, Size);

    RulUI_define_property_change_event(FocusChangeEvent, bool);

#undef RulUI_define_property_change_event

} // namespace RolUI
