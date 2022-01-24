
#include "RolUI/utility/utf8ext.h"

#include "RolUI/Application.hpp"
#include "RolUI/Window.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/widgets/Text.hpp"
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
                _size = win->painter()->text_size(text->c_str(), text->size());
            } else {
                _size = Size(0, 0);
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
            return _size;
        }

        unsigned Text::line_height() const noexcept {
            return font_size.get();
        }
        unsigned Text::pos_to_index(Point pos) const noexcept {
            unsigned maybe_idx = float(pos.x) / float(size().width) * text->size();
            Point maybe_pos = _index_to_pos(maybe_idx);

            if (maybe_idx > text->size())
                maybe_idx = text->size();

            for (unsigned i = maybe_idx; i > 0; i--) {
                Point pos_t = _index_to_pos(i - 1);
                if (std::abs(pos_t.x - pos.x) <= std::abs(maybe_pos.x - pos.x)) {
                    maybe_pos = pos_t;
                    maybe_idx = i - 1;
                } else
                    break;
            }

            for (unsigned i = maybe_idx + 1; i < text->size(); i++) {
                Point pos_t = _index_to_pos(i);
                if (std::abs(pos_t.x - pos.x) <= std::abs(maybe_pos.x - pos.x)) {
                    maybe_pos = pos_t;
                    maybe_idx = i;
                } else
                    break;
            }

            return utf8codepointindex(text->c_str(), maybe_idx);
        }
        Point Text::index_to_pos(unsigned index) const noexcept {
            return _index_to_pos(index);
        }

        Point Text::_index_to_pos(unsigned index) const noexcept {
            if (index == 0) return {0, 0};

            unsigned utf8_idx = utf8utf8index(text->c_str(), index);
            IPainter* painter = Application::window()->painter();
            Size s = painter->text_size(text->c_str(), utf8_idx);
            return {s.width, 0};
        }

    } // namespace widget
} // namespace RolUI
