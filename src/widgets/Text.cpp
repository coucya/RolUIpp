
#include "RolUI/utility/utf8ext.h"

#include "RolUI/Application.hpp"
#include "RolUI/Window.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/widgets/Text.hpp"
#include "RolUI/events/Widget_event.hpp"

namespace RolUI {
    namespace widgets {

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

        void Text::draw(IPainter* painter) noexcept {
            painter->set_font_size(font_size);
            painter->set_font_color(font_color);

            if (!font_name->empty())
                painter->set_font(font_name->c_str());
            else
                painter->set_font("default");

            if (!text->empty())
                painter->draw_text(abs_pos(), text->c_str(), text->size());
        }

        Size Text::perform_layout(Constraint constraint) noexcept {
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

        EditableText::EditableText() noexcept : Text("") {
            _blink_timer.on_timeout.connect([this](double timeout) {
                this->_show_cursor = !this->_show_cursor;
            });
            cursor_index.on_change.connect([this](unsigned int index) {
                this->_update_cursor_pos();
            });
            text.on_change.connect([this](const std::string& text) {
                this->_update_cursor_pos();
            });
        }
        EditableText::~EditableText() {}

        bool EditableText::cursor_blinks() const noexcept {
            return _blink_timer.is_action();
        }
        void EditableText::set_cursor_blinks(bool blink) noexcept {
            if (blink && !cursor_blinks()) {
                _blink_timer.start(0.5, false);
            } else if (!blink)
                _blink_timer.stop();
        }

        void EditableText::delete_front() noexcept {
            _delete_at_index(std::max(0, int(cursor_index.get()) - 1), 1);
            if (cursor_index > 0)
                cursor_index = cursor_index - 1;
        }
        void EditableText::delete_back() noexcept {
            _delete_at_index(cursor_index.get() + 1, 1);
        }

        void EditableText::insert_char(unsigned idx, uint32_t char_) noexcept {
            char str[7] = {0, 0, 0, 0, 0, 0, 0};
            utf8catcodepoint(str, char_, 6);
            insert_str(idx, str);
        }
        void EditableText::insert_str(unsigned idx, const char* str) noexcept {
            insert_str(idx, str, strlen(str));
        }
        void EditableText::insert_str(unsigned idx, const char* str, unsigned len) noexcept {
            std::string ts = text;
            ts.insert(idx, str, len);
            text = std::move(ts);
        }

        void EditableText::draw(IPainter* painter) noexcept {
            Text::draw(painter);
            if (_show_cursor) {
                int ts = font_size.get();
                painter->set_stroke_width(2);
                painter->set_stroke_color({0, 0, 0});
                painter->draw_vline(_cursor_pos, ts);
            }
        }

        void EditableText::_delete_at_index(unsigned idx, unsigned len) noexcept {
            int utf8_idx = utf8utf8index(text->c_str(), idx);
            int utf8_len = utf8_idx + utf8utf8index(text->c_str() + utf8_idx, len);
            std::string ts = text;
            ts.erase(utf8_idx, utf8_len);
            text = std::move(ts);
        }
        void EditableText::_update_cursor_pos() noexcept {
            _cursor_pos = index_to_pos(cursor_index.get());
            return;
        }

    } // namespace widgets
} // namespace RolUI
