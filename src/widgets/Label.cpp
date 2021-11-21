
#include "RolUI/Color.hpp"
#include "RolUI/IEvent.hpp"
#include "RolUI/Size.hpp"
#include "RolUI/Widget.hpp"

#include "RolUI/widgets/Label.hpp"

namespace RolUI {
    namespace widget {

        void Label::_init_part(std::string&& text) noexcept {

            set_size(_text_widget.font_size * (text.size() + 1) * 0.5, _text_widget.font_size * 2);

            _rect_widget.set_pos(0, 0);
            _rect_widget.set_size_relative(RelativeTarget::parent, SizeMode::relative);
            _text_widget.set_pos(0, 0);
            _text_widget.set_pos_relative(RelativeTarget::parent, AnchorPoint::centre_middle, AnchorPoint::centre_middle);

            this->text.on_change.connect(&_text_widget.text);
            font_color.on_change.connect(&_text_widget.font_color);
            font_name.on_change.connect(&_text_widget.font_name);
            font_size.on_change.connect(&_text_widget.font_size);

            round.on_change.connect(&_rect_widget.round);
            border_color.on_change.connect(&_rect_widget.border_color);
            border_width.on_change.connect(&_rect_widget.border_width);
            background_color.on_change.connect(&_rect_widget.background_color);

            add_part(&_rect_widget);
            add_part(&_text_widget);

            this->text = std::move(text);
        }

        Label::Label() noexcept {
            _init_part("");
        }
        Label::Label(std::string text) noexcept {
            _init_part(std::move(text));
        }
        Label::Label(std::string text, Widget* parent) noexcept
            : Widget(parent) {
            _init_part(std::move(text));
        }
        Label::~Label() {}

        void Label::adjust_size() noexcept {
            Size content_size = _text_widget.size();
            set_size(content_size.width + padding.get().x * 2,
                     content_size.height + padding.get().y * 2);
        }

        void Label::set_style(const Style& style) {
            _rect_widget.set_style(style);
            _text_widget.set_style(style);
            padding = style.padding;
        }

    } // namespace widget
} // namespace RolUI
