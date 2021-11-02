#pragma once

#include "RolUI/IEvent.hpp"

namespace RolUI {

    class Window;
    class Widget;

    class WindowChangeEvent : public IEvent {
      public:
        RolUI_decl_event_type_in_class(WindowChangeEvent);

      public:
        WindowChangeEvent(Widget* target, Window* current, Window* old) noexcept
            : IEvent(type(), target), _current_window(current), _old_window(old) {}

        Window* current_window() const noexcept;
        Window* old_window() const noexcept;

      private:
        Window* _current_window;
        Window* _old_window;
    };

    class ParentChangeEvent : public IEvent {
      public:
        RolUI_decl_event_type_in_class(ParentChangeEvent);

      public:
        ParentChangeEvent(Widget* target, Widget* current, Widget* old) noexcept
            : IEvent(type(), target), _current_parent(current), _old_parent(old) {}

        Widget* current_parent() const noexcept;
        Widget* old_parent() const noexcept;

      private:
        Widget* _current_parent;
        Widget* _old_parent;
    };

} // namespace RolUI
