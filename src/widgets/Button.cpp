
#include <cstddef>

#include "RolUI/IEvent.hpp"
#include "RolUI/Style.hpp"
#include "RolUI/events/MouseEvent.hpp"
#include "RolUI/WidgetState.hpp"
#include "RolUI/widgets/Label.hpp"
#include "RolUI/widgets/Button.hpp"

namespace RolUI {
    namespace widget {

        Button::Button(Widget* parent) noexcept
            : Label(parent) { _init(); }
        Button::Button(Widget* parent, std::string text) noexcept
            : Label(parent, std::move(text)) { _init(); }

        Button::~Button() {}

        void Button::_init() noexcept {
            Style default_, press, hover;
            default_.round = 0;
            default_.border_color = {100, 100, 100, 255};
            default_.border_width = 1;
            default_.background_color = {240, 240, 240};
            default_.color = {16, 16, 16};
            default_.font_name = "default";
            default_.font_size = 16;
            default_.padding = {20, 10};

            hover.background_color = {220, 220, 220};
            press.background_color = {230, 230, 230};

            StyleSheet ss;
            ss.set_style(default_, WIDGET_STATE_DEFAULT);
            ss.set_style(press, WIDGET_STATE_PRESS);
            ss.set_style(hover, WIDGET_STATE_HOVER);

            set_style(default_);
            set_style_sheet(ss);

            Label::adjust_size();

            _init_event_bind();
        }

        void Button::_init_event_bind() noexcept {
            add_listener(MousePressEvent_type(), [this](IEvent*) {
                this->_is_press = true;
                return false;
            });
            add_listener(MousePosEvent_type(), [this](IEvent*) {
                this->_is_press = false;
                return false;
            });
            add_listener(MouseLeaveEvent_type(), [this](IEvent*) {
                this->_is_press = false;
                return false;
            });
            add_listener(MouseReleaseEvent_type(), [this](IEvent*) {
                this->_is_press = false;
                this->on_click.emit();
                return false;
            });
        }

    } // namespace widget
} // namespace RolUI
