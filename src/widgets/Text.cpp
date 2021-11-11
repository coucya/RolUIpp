
#include "RolUI/Window.hpp"
#include "RolUI/widgets/Text.hpp"
#include "RolUI/events/Widget_event.hpp"

namespace RolUI {
    namespace widget {

        Text ::Text() noexcept {
            add_listener(WindowChangeEvent::type(), [this](IEvent* e) {
                this->_update_size();
                return true;
            });
        }

        Text::~Text() {}

        void Text::_update_size() {
            Window* win = window();

            if (win && !_text.empty()) {
                win->painter()->set_font_size(_propertys.font_size);
                if (_propertys.font_name)
                    win->painter()->set_font(_propertys.font_name);
                Size s = win->painter()->text_size(_text.c_str(), _text.size());
                Widget::set_size(s);
            }
        }

        const std::string& Text::text() const noexcept { return _text; }
        const char* Text::font() const noexcept { return _propertys.font_name; }
        unsigned Text::font_size() const noexcept { return _propertys.font_size; }
        Color Text::font_color() const noexcept { return _propertys.font_color; }

        const Text::StyleProperty& Text::style_property() const noexcept { return _propertys; }

        void Text::set_font(const char* name) noexcept {
            _propertys.font_name = name;
            _update_size();
        }
        void Text::set_font_size(unsigned size) noexcept {
            _propertys.font_size = size;
            _update_size();
        }
        void Text::set_font_color(Color c) noexcept {
            _propertys.font_color = c;
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

        void Text::set_style_property(const StyleProperty& property) noexcept {
            _propertys.font_size = property.font_size;
            _propertys.font_color = property.font_color;
            _propertys.font_name = property.font_name;
            _update_size();
        }

        void Text::on_draw(IPainter* painter) {
            painter->set_font_size(_propertys.font_size);
            painter->set_font_color(_propertys.font_color);

            if (_propertys.font_name)
                painter->set_font(_propertys.font_name);
            if (!_text.empty())
                painter->draw_text(pos(), _text.c_str(), _text.size());
        }

    } // namespace widget
} // namespace RolUI
