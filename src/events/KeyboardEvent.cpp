
#include <cstring>
#include <functional>

#include "RolUI/IEvent.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/Window.hpp"
#include "RolUI/Application.hpp"
#include "RolUI/events/KeyboardEvent.hpp"

namespace RolUI {

    RolUI_impl_event_type_in_class(KeyboardEvent);

    KeyboardEvent::KeyboardEvent(Widget* target) noexcept
        : IEvent(type(), target) {}

    KeyboardKey KeyboardEvent::action() const noexcept { return _action; }
    KeyboardKeyMode KeyboardEvent::key_mode() const noexcept {
        return _key_modes[int(_action)];
    }
    KeyboardKeyMode KeyboardEvent::key_mode(KeyboardKey key) const noexcept {
        return _key_modes[int(key)];
    }

    KeyboardDispatcher::KeyboardDispatcher() noexcept {
        clear_change();
    }

    KeyboardKeyMode KeyboardDispatcher::key_mode(KeyboardKey key) const noexcept {
        return _key_modes[int(key)];
    }
    void KeyboardDispatcher::set_key_mode(KeyboardKey key, KeyboardKeyMode mode) noexcept {
        KeyboardKeyMode old_mode = _key_modes[int(key)];
        _key_modes[int(key)] = mode;
        if (old_mode != mode)
            _key_is_change[int(key)] = true;
    }

    void KeyboardDispatcher::clear_change() noexcept {
        for (int i = 0; i < KEYBORD_KEY_COUNT; i++)
            _key_is_change[i] = false;
    }

    void KeyboardDispatcher::dispatch(Window* w) noexcept {
        if (w == nullptr) return;
        RootWidget* rw = Application::root_widget();

        KeyboardEvent ke{nullptr};
        for (int i = 0; i < KEYBORD_KEY_COUNT; i++)
            ke._key_modes[i] = _key_modes[i];

        std::function<void(Widget*)> f;
        auto _f = [&](Widget* w) {
            send_event(w, &ke);
            w->visit_children(f);
        };
        f = _f;

        for (int i = 0; i < KEYBORD_KEY_COUNT; i++) {
            if (_key_is_change[i]) {
                ke._action = KeyboardKey(i);
                rw->visit_children(f);
            }
        }

        clear_change();
    }

} // namespace RolUI
