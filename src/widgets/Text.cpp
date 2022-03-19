
#include "RolUI/utility/utf8ext.h"

#include "RolUI/Application.hpp"
#include "RolUI/Window.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/widgets/Text.hpp"
#include "RolUI/events/Widget_event.hpp"
#include "RolUI/events/MouseEvent.hpp"
#include "RolUI/events/CharEvent.hpp"

namespace RolUI {
    namespace widgets {

        static unsigned _prev_byte_index(const char* str, unsigned index) noexcept {
            utf8_int32_t _tmp_;
            const char* end = utf8rcodepoint(str + index, &_tmp_);
            unsigned idx = end <= str ? 0 : end - str;
            return idx;
        }
        static unsigned _next_byte_index(const char* str, unsigned index) noexcept {
            utf8_int32_t _tmp_;
            const char* end = utf8codepoint(str + index, &_tmp_);
            unsigned idx = end <= str ? 0 : end - str;
            return idx;
        }

        static unsigned _byte_index_to_char_index(const char* str, unsigned index) noexcept {
            unsigned idx = utf8nlen(str, index);
            return idx;
        }
        static unsigned _char_index_to_byte_index(const char* str, unsigned index) noexcept {
            unsigned idx = utf8utf8index(str, index);
            return idx;
        }

        TextWidget ::TextWidget() noexcept {
            font_size.on_change.connect([this](const unsigned&) { this->_update_size(); });
            font_color.on_change.connect([this](const Color&) { this->_update_size(); });
            font_name.on_change.connect([this](const std::string&) { this->_update_size(); });
            text.on_change.connect([this](const std::string&) { this->_update_size(); });
        }
        TextWidget ::TextWidget(const std::string& str) noexcept
            : TextWidget() {
            text(str);
        }

        TextWidget::~TextWidget() {}

        void TextWidget::_update_size() noexcept {
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

        void TextWidget::draw(IPainter* painter) noexcept {
            painter->set_font_size(font_size);
            painter->set_font_color(font_color);

            if (!font_name->empty())
                painter->set_font(font_name->c_str());
            else
                painter->set_font("default");

            if (!text->empty())
                painter->draw_text(abs_pos(), text->c_str(), text->size());
        }

        Size TextWidget::perform_layout(Constraint constraint) noexcept {
            return _size;
        }

        unsigned TextWidget::line_height() const noexcept {
            return font_size.get();
        }
        unsigned TextWidget::pos_to_index(Point pos) const noexcept {
            const char* text_str = text->c_str();
            int text_size = text->size();

            unsigned maybe_idx = float(pos.x) / float(size().width) * text->size();
            Point maybe_pos = _byte_index_to_pos(maybe_idx);

            if (maybe_idx > text->size())
                maybe_idx = text->size();

            while (maybe_idx > 0) {
                unsigned t = _prev_byte_index(text_str, maybe_idx);
                Point pos_t = _byte_index_to_pos(t);
                if (std::abs(pos_t.x - pos.x) <= std::abs(maybe_pos.x - pos.x)) {
                    maybe_pos = pos_t;
                    maybe_idx = t;
                } else
                    break;
            }

            while (maybe_idx < text_size) {
                unsigned t = _next_byte_index(text_str, maybe_idx);
                Point pos_t = _byte_index_to_pos(t);
                if (std::abs(pos_t.x - pos.x) <= std::abs(maybe_pos.x - pos.x)) {
                    maybe_pos = pos_t;
                    maybe_idx = t;
                } else
                    break;
            }

            return _byte_index_to_char_index(text->c_str(), maybe_idx);
        }
        Point TextWidget::index_to_pos(unsigned index) const noexcept {
            return _char_index_to_pos(index);
        }

        Point TextWidget::_byte_index_to_pos(unsigned index) const noexcept {
            if (index == 0) return {0, 0};

            const char* text_str = text->c_str();

            IPainter* painter = Application::window()->painter();
            Size s = painter->text_size(text_str, index);
            return {s.width, 0};
        }
        Point TextWidget::_char_index_to_pos(unsigned index) const noexcept {
            if (index == 0) return {0, 0};

            unsigned byte_idx = _char_index_to_byte_index(text->c_str(), index);
            Point pos = _byte_index_to_pos(byte_idx);
            return pos;
        }

        EditableTextWidget::EditableTextWidget() noexcept {
            cursor_index.on_change.connect([this](unsigned int index) {
                this->_update_cursor_pos();
            });
            text.on_change.connect([this](const std::string& text) {
                this->_update_cursor_pos();
            });
        }
        EditableTextWidget::~EditableTextWidget() {}

        bool EditableTextWidget::is_blinking() const noexcept {
            return _is_blinking;
        }
        void EditableTextWidget::set_blink(bool blink) noexcept {
            if (blink && !is_blinking()) {
                _show_cursor = true;
                _is_blinking = true;
                _blink_timer_handle = Application::set_interval(0.5f, [this](double) {
                    this->_show_cursor = !this->_show_cursor;
                });
            } else if (!blink && _is_blinking) {
                _is_blinking = false;
                Application::clear_interval(_blink_timer_handle);
                _show_cursor = false;
            }
        }

        void EditableTextWidget::delete_front() noexcept {
            _delete_at_index(std::max(0, int(cursor_index.get()) - 1), 1);
            if (cursor_index > 0)
                cursor_index = cursor_index - 1;
        }
        void EditableTextWidget::delete_back() noexcept {
            _delete_at_index(cursor_index.get() + 1, 1);
        }

        void EditableTextWidget::insert_char(unsigned idx, uint32_t char_) noexcept {
            char str[7] = {0, 0, 0, 0, 0, 0, 0};
            utf8catcodepoint(str, char_, 6);
            insert_str(idx, str);
        }
        void EditableTextWidget::insert_str(unsigned idx, const char* str) noexcept {
            insert_str(idx, str, strlen(str));
        }
        void EditableTextWidget::insert_str(unsigned idx, const char* str, unsigned len) noexcept {
            int byte_idx = _char_index_to_byte_index(text().c_str(), idx);
            std::string ts = text();
            ts.insert(byte_idx, str, len);
            text = std::move(ts);
            cursor_index = cursor_index() + utf8nlen(str, len);
        }

        void EditableTextWidget::draw(IPainter* painter) noexcept {
            TextWidget::draw(painter);
            if (_show_cursor) {
                int ts = font_size.get();
                painter->set_stroke_width(2);
                painter->set_stroke_color({0, 0, 0});
                painter->draw_vline(abs_pos() + _cursor_pos, ts);
            }
        }

        void EditableTextWidget::_delete_at_index(unsigned idx, unsigned len) noexcept {
            int utf8_idx = utf8utf8index(text->c_str(), idx);
            int utf8_len = utf8_idx + utf8utf8index(text->c_str() + utf8_idx, len);
            std::string ts = text;
            ts.erase(utf8_idx, utf8_len);
            text = std::move(ts);
        }
        void EditableTextWidget::_update_cursor_pos() noexcept {
            _cursor_pos = index_to_pos(cursor_index.get());
            return;
        }

        TextBoxWidget::TextBoxWidget() noexcept {}
        TextBoxWidget::~TextBoxWidget() {}

        bool TextBoxWidget::handle_event(IEvent* e) noexcept {
            EditableTextWidget::handle_event(e);

            if (e->is(MousePressEvent_type()) && ((MouseEvent*)e)->action() == MouseKey::left) {
                MouseEvent* me = (MouseEvent*)e;
                this->cursor_index = this->pos_to_index(me->pos() - this->abs_pos());
                Application::set_focus_widget(this);
                return true;
            } else if (e->is(FocusChangeEvent::type())) {
                FocusChangeEvent* fce = (FocusChangeEvent*)e;
                if (fce->current_value())
                    this->set_blink(true);
                else
                    this->set_blink(false);
                return true;
            } else if (e->is(CharEvent::type())) {
                if (Application::focus_widget() == this) {
                    uint32_t cp = ((CharEvent*)e)->codepoint();
                    int idx = this->cursor_index();
                    this->insert_char(idx, cp);
                }
            }
            return false;
        }

    } // namespace widgets
} // namespace RolUI
