
#include <cstring>
#include <functional>

#include "RolUI/IEvent.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/Window.hpp"
#include "RolUI/Application.hpp"
#include "RolUI-events/KeyboardEvent.hpp"

namespace RolUI {

    RolUI_impl_object_type_of(KeyboardEvent, IEvent);

    KeyboardEvent::KeyboardEvent(Widget* target) noexcept : IEvent(target) {}

    const ObjectType* KeyboardEvent::object_type() const noexcept { return object_type_of<KeyboardEvent>(); }

    KeyboardKey KeyboardEvent::action() const noexcept { return _action; }
    KeyboardKeyMode KeyboardEvent::key_mode() const noexcept {
        return _key_modes[int(_action)];
    }
    KeyboardKeyMode KeyboardEvent::key_mode(KeyboardKey key) const noexcept {
        return _key_modes[int(key)];
    }

    KeyboardDispatcher::KeyboardDispatcher() noexcept {
        clear_change();
        for (int i = 0; i < KEYBORD_KEY_COUNT; i++)
            _key_modes[i] = KeyboardKeyMode::release;
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

        for (int i = 0; i < KEYBORD_KEY_COUNT; i++) {
            if (_key_is_change[i]) {
                ke._action = KeyboardKey(i);
                visit_tree(rw, [&](Widget* w) {
                    send_event(w, &ke);
                });
            }
        }

        clear_change();
    }

} // namespace RolUI
