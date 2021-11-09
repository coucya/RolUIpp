
#include "RolUI/Window.hpp"
#include "RolUI/widgets/Text.hpp"
#include "RolUI/events/Widget_event.hpp"

namespace RolUI {
    namespace widget {

        Text ::Text() noexcept
            : _font_size(15), _font_color(Color()) {

            add_listener(WindowChangeEvent::type(), [this](IEvent* e) {
                this->_update_size();
                return true;
            });
        }

        Text::~Text() {}

        void Text::_update_size() {
            Window* win = window();

            if (win && !_text.empty()) {
                Size s = win->painter()->text_size(_text.c_str(), _text.size());
                Widget::set_size(s);
            }
        }
        const std::string& Text::text() const noexcept { return _text; }
        const char* Text::font() const noexcept { return _font_name; }
        unsigned Text::font_size() const noexcept { return _font_size; }
        Color Text::font_color() const noexcept { return _font_color; }

        void Text::set_font(const char* name) noexcept {
            _font_name = name;
        }
        void Text::set_font_size(unsigned size) noexcept {
            _font_size = size;
        }
        void Text::set_font_color(Color c) noexcept {
            _font_color = c;
        }

        void Text::set_text(const char* text) noexcept {
            if (text == nullptr) return;
            set_text(text, strlen(text));
        }
        void Text::set_text(const char* text, unsigned len) noexcept {
            if (text == nullptr) return;
            _text = std::string(text, len);
            _update_size();
        }

        void Text::set_text(const std::string& text) noexcept {
            _text = text;
            _update_size();
        }

        void Text::on_draw(IPainter* painter) {
            painter->set_font_size(_font_size);
            painter->set_font_color(_font_color);

            if (_font_name)
                painter->set_font(_font_name);
            if (!_text.empty())
                painter->draw_text(pos(), _text.c_str(), _text.size());
        }

    } // namespace widget
} // namespace RolUI
