#pragma once

#include <queue>

#include "../IEvent.hpp"

namespace RolUI {

    class Window;

    class CharEvent : public IEvent {
      public:
        CharEvent(Widget* target) noexcept;
        CharEvent(Widget* target, unsigned int cp) noexcept;
        ~CharEvent();

        const ObjectType* object_type() const noexcept override;

        unsigned int codepoint() const noexcept;
        const char* c_char() const noexcept;

      private:
        unsigned int _codepoint = 0;
        char _c_str[8]{0, 0, 0, 0, 0, 0, 0, 0};
    };

    RolUI_decl_object_type_of(CharEvent);

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
