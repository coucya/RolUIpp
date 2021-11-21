
#include "RolUI/widgets/Button.hpp"
#include "RolUI/IEvent.hpp"
#include "RolUI/events/MouseEvent.hpp"
#include "RolUI/widgets/Label.hpp"
#include <cstddef>

namespace RolUI {
    namespace widget {

        Button::Button() noexcept { _init(); }
        Button::Button(std::string text) noexcept
            : Label(std::move(text)) { _init(); }
        Button::Button(std::string text, Widget* parent) noexcept
            : Label(std::move(text), parent) { _init(); }

        Button::~Button() {}

        void Button::_init() noexcept {
            Label::adjust_size();
            _init_event_bind();
        }

        void Button::_init_event_bind() noexcept {
        }

    } // namespace widget
} // namespace RolUI
