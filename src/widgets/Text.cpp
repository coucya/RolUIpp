
#include "RolUI/Application.hpp"
#include "RolUI/Window.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/WidgetState.hpp"
#include "RolUI/widgets/Text.hpp"
#include "RolUI/Style.hpp"
#include "RolUI/events/Widget_event.hpp"

namespace RolUI {
    namespace widget {

        Text ::Text() noexcept {
            _update_size();
            font_size.on_change.connect([this](const unsigned&) { this->_update_size(); });
            font_color.on_change.connect([this](const Color&) { this->_update_size(); });
            font_name.on_change.connect([this](const std::string&) { this->_update_size(); });
            text.on_change.connect([this](const std::string&) { this->_update_size(); });
        }

        Text::~Text() {}

        void Text::_update_size() {
            Window* win = Application::window();
            if (!text->empty()) {
                win->painter()->set_font_size(font_size);
                if (!font_name->empty())
                    win->painter()->set_font(font_name->c_str());
                Size s = win->painter()->text_size(text->c_str(), text->size());
                Widget::set_size(s);
            }
        }

        void Text::on_draw(IPainter* painter) {
            painter->set_font_size(font_size);
            painter->set_font_color(font_color);

            if (!font_name->empty())
                painter->set_font(font_name->c_str());

            if (!text->empty())
                painter->draw_text({0, 0}, text->c_str(), text->size());
        }

        void Text::set_style(const Style& style) {
            font_size = style.font_size;
            font_color = style.color;
            font_name = style.font_name;
        }

    } // namespace widget
} // namespace RolUI
