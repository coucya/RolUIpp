
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
            set_border_color({0, 0, 0});
            set_border_width(1);
            set_background_color({240, 240, 240});

            _init_event_bind();
        }

        void Button::_init_event_bind() noexcept {
            add_listener(MousePressEvent_type(), [this](IEvent* e) {
                this->_is_press = true;
                return false;
            });
            add_listener(MousePosEvent_type(), [this](IEvent* e) {
                this->_is_press = false;
                return false;
            });
            add_listener(MouseLeaveEvent_type(), [this](IEvent* e) {
                this->_is_press = false;
                return false;
            });
            add_listener(MouseReleaseEvent_type(), [this](IEvent* e) {
                if (this->_is_press) {
                    this->on_click();
                    if (this->on_click.slot_count() > 0)
                        return true;
                }
                return false;
            });
        }

    } // namespace widget
} // namespace RolUI
