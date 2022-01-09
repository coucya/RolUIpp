
#include "RolUI/Application.hpp"
#include "RolUI/Window.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/WidgetState.hpp"
#include "RolUI/widgets/Text.hpp"
#include "RolUI/Style.hpp"
#include "RolUI/events/Widget_event.hpp"

namespace RolUI {
    namespace widget {

        Text ::Text(const std::string& str) noexcept {
            text = str;
            _update_size();
            font_size.on_change.connect([this](const unsigned&) { this->_update_size(); });
            font_color.on_change.connect([this](const Color&) { this->_update_size(); });
            font_name.on_change.connect([this](const std::string&) { this->_update_size(); });
            text.on_change.connect([this](const std::string&) { this->_update_size(); });
        }

        Text::~Text() {}

        void Text::_update_size() noexcept {
            Window* win = Application::window();
            if (!text->empty()) {
                win->painter()->set_font_size(font_size);
                if (!font_name->empty())
                    win->painter()->set_font(font_name->c_str());
                _text_size = win->painter()->text_size(text->c_str(), text->size());
            } else {
                _text_size = Size(0, 0);
            }
        }

        void Text::on_draw(IPainter* painter) {
            painter->set_font_size(font_size);
            painter->set_font_color(font_color);

            if (!font_name->empty())
                painter->set_font(font_name->c_str());
            else
                painter->set_font("default");

            if (!text->empty())
                painter->draw_text({0, 0}, text->c_str(), text->size());
        }

        Size Text::perlayout(Constraint constraint) {
            return _text_size;
        }

    } // namespace widget
} // namespace RolUI
