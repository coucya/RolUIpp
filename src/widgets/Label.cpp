
#include "RolUI/widgets/Label.hpp"
#include "RolUI/IEvent.hpp"
#include "RolUI/Size.hpp"
#include <cstdint>

namespace RolUI {
    namespace widget {

        void Label::_init_part(std::string&& text) noexcept {

            set_size(_text_widget.font_size() * (text.size() + 1) * 0.5, _text_widget.font_size() * 2);

            _rect_widget.set_pos(0, 0);
            _rect_widget.set_size(size());
            _rect_widget.set_background_color({255, 255, 255});

            _text_widget.set_text(std::move(text));

            add_part(&_rect_widget);
            add_part(&_text_widget);

            _text_widget.on_size_change.connect([this](Size text_size) {
                Size size = this->size();
                _rect_widget.set_size(size);

                int32_t tpx = ((int32_t)size.width - (int32_t)text_size.width) / 2;
                int32_t tpy = ((int32_t)size.height - (int32_t)text_size.height) / 2;
                _text_widget.set_pos(tpx, tpy);
                return true;
            });

            on_size_change.connect([&](Size size) {
                _rect_widget.set_size(size);

                Size text_size = _text_widget.size();
                int32_t tpx = ((int32_t)size.width - (int32_t)text_size.width) / 2;
                int32_t tpy = ((int32_t)size.height - (int32_t)text_size.height) / 2;
                _text_widget.set_pos(tpx, tpy);

                return true;
            });
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

        const std::string& Label::text() const noexcept { return _text_widget.text(); }

        const char* Label::font() const noexcept { return _text_widget.font(); }
        size_t Label::font_size() const noexcept { return _text_widget.font_size(); }

        Color Label::background_color() const noexcept { return _rect_widget.background_color(); }

        uint32_t Label::border_width() const noexcept { return _rect_widget.border_width(); }
        Color Label::border_color() const noexcept { return _rect_widget.border_color(); }

        uint32_t Label::round() const noexcept { return _rect_widget.round(); }

        void Label::set_text(std::string text) noexcept {
            _text_widget.set_text(std::move(text));
        }

        void Label::set_font(const char* name) noexcept {
            _text_widget.set_font(name);
        }
        void Label::set_font_size(size_t size) noexcept {
            _text_widget.set_font_size(size);
        }

        void Label::set_background_color(Color c) noexcept {
            _rect_widget.set_background_color(c);
        }
        void Label::set_border_width(uint32_t w) noexcept {
            _rect_widget.set_border_width(w);
        }
        void Label::set_border_color(Color c) noexcept {
            _rect_widget.set_border_color(c);
        }
        void Label::set_round(uint32_t r) noexcept {
            _rect_widget.set_round(r);
        }

    } // namespace widget
} // namespace RolUI
