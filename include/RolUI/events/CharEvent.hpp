#pragma once

#include <queue>

#include "../IEvent.hpp"

namespace RolUI {

    class Window;

    class CharEvent : public IEvent {
      public:
        RolUI_decl_event_type_in_class(CharEvent);

      public:
        CharEvent(Widget* target, unsigned int cp) noexcept;
        ~CharEvent();

        unsigned int codepoint() const noexcept;
        const char* c_char() const noexcept;

      private:
        unsigned int _codepoint = 0;
        char _c_str[8] = {0};
    };

    class CharEventDispatcher {
      public:
        void push_char(unsigned int c) noexcept;
        unsigned int pop_char() noexcept;
        unsigned int top_char() const noexcept;

        unsigned char_count() const noexcept;
        bool empty() const noexcept;

        void dispatch(Window* w) noexcept;

      private:
        std::queue<unsigned int> _char_queue;
    };

} // namespace RolUI
