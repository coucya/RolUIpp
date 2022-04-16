
#include "RolUI/IEvent.hpp"
#include "RolUI/Window.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/Application.hpp"
#include "RolUI/events/CharEvent.hpp"

#include "utf8.h"

namespace RolUI {

    RolUI_impl_object_type_of(CharEvent, IEvent);

    CharEvent::CharEvent(Widget* target) noexcept : IEvent(target), _codepoint(0) {}
    CharEvent::CharEvent(Widget* target, unsigned int cp) noexcept
        : IEvent(target), _codepoint(cp) {
        utf8catcodepoint(_c_str, cp, sizeof(_c_str) / sizeof(char));
    }
    CharEvent::~CharEvent() {}

    const ObjectType* CharEvent::object_type() const noexcept { return object_type_of<CharEvent>(); }

    unsigned int CharEvent::codepoint() const noexcept { return _codepoint; }
    const char* CharEvent::c_char() const noexcept { return _c_str; }

    void CharEventDispatcher::push_char(unsigned int c) noexcept {
        _char_queue.push(c);
    }
    unsigned int CharEventDispatcher::pop_char() noexcept {
        unsigned int c = _char_queue.front();
        _char_queue.pop();
        return c;
    }
    unsigned int CharEventDispatcher::top_char() const noexcept {
        return _char_queue.front();
    }

    unsigned CharEventDispatcher::char_count() const noexcept {
        return _char_queue.size();
    }
    bool CharEventDispatcher::empty() const noexcept { return _char_queue.empty(); }

    void CharEventDispatcher::dispatch(Window* w) noexcept {
        if (w == nullptr) return;
        RootWidget* rw = Application::root_widget();

        CharEvent ce{nullptr};

        while (!empty()) {
            ce = CharEvent{nullptr, pop_char()};
            visit_tree(rw, [&](Widget* w) {
                send_event(w, &ce);
            });
        }
    }

} // namespace RolUI
