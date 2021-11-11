
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
            _default_propertys = this->Label::style_property();
            _default_propertys.font_color = {0, 0, 0, 255};
            _default_propertys.border_color = {50, 50, 50};
            _default_propertys.border_width = 1;
            _default_propertys.background_color = {240, 240, 240};

            _hover_propertys = _default_propertys;
            _press_propertys = _default_propertys;

            _hover_propertys.background_color = {220, 220, 220};
            _press_propertys.background_color = {250, 250, 250};

            Label::set_style_property(_default_propertys);
            Label::adjust_size();

            _init_event_bind();
        }

        void Button::_init_event_bind() noexcept {
            add_listener(MousePressEvent_type(), [this](IEvent* e) {
                this->on_state_change.emit(State::press);

                this->_is_press = true;
                return false;
            });
            add_listener(MousePosEvent_type(), [this](IEvent* e) {
                this->_is_press = false;
                return false;
            });
            add_listener(MouseEnterEvent_type(), [this](IEvent* e) {
                this->on_state_change.emit(State::hover);

                return false;
            });
            add_listener(MouseLeaveEvent_type(), [this](IEvent* e) {
                this->on_state_change.emit(State::default_);

                this->_is_press = false;
                return false;
            });
            add_listener(MouseReleaseEvent_type(), [this](IEvent* e) {
                this->on_state_change.emit(State::hover);

                if (this->_is_press) {
                    this->on_click();
                    if (this->on_click.slot_count() > 0)
                        return true;
                }
                return false;
            });
            on_state_change.connect([this](State s) {
                switch (s) {
                    case default_:
                        this->Label::set_style_property(_default_propertys);
                        break;
                    case hover:
                        this->Label::set_style_property(_hover_propertys);
                        break;
                    case press:
                        this->Label::set_style_property(_press_propertys);
                        break;
                }
            });
        }

        const Button::StyleProperty& Button::default_property() const noexcept { return _default_propertys; }
        const Button::StyleProperty& Button::hover_property() const noexcept { return _hover_propertys; }
        const Button::StyleProperty& Button::press_property() const noexcept { return _press_propertys; }

        void Button::set_default_property(const StyleProperty& property) noexcept {
            _default_propertys = property;
            if (_state == State::default_)
                Label::set_style_property(property);
        }
        void Button::set_hover_property(const StyleProperty& property) noexcept {
            _hover_propertys = property;
            if (_state == State::hover)
                Label::set_style_property(property);
        }
        void Button::set_press_property(const StyleProperty& property) noexcept {
            _press_propertys = property;
            if (_state == State::press)
                Label::set_style_property(property);
        }

    } // namespace widget
} // namespace RolUI
