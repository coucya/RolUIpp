
#include "RolUI/utility/utf8ext.h"

#include "RolUI/Application.hpp"
#include "RolUI/Window.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/widgets/Text.hpp"
#include "RolUI/events/Widget_event.hpp"
#include "RolUI/events/MouseEvent.hpp"
#include "RolUI/events/CharEvent.hpp"
#include "RolUI/events/KeyboardEvent.hpp"

namespace RolUI {
    namespace widgets {

        TextWidget ::TextWidget() noexcept {
            font_size.on_change.connect([this](const unsigned&) { this->_did_text_change(); });
            font_color.on_change.connect([this](const Color&) { this->_did_text_change(); });
            font_name.on_change.connect([this](const std::string&) { this->_did_text_change(); });
            text.on_change.connect([this](const std::string&) { this->_did_text_change(); });
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
                _text_size = win->painter()->text_size(text->c_str(), text->size());
            } else {
                _text_size = Size(0, 0);
            }
        }

        void TextWidget::_did_text_change() noexcept {
            _update_size();
            const char* beg = text().c_str();
            const char* end = beg + text().size();
            const char* it = beg;
            utf8_int32_t cp;
            _chars.clear();
            while (it < end) {
                const char* next_it = utf8codepoint(it, &cp);
                _chars.push_back(Char{cp, uint32_t(it - beg), uint32_t(next_it - beg)});
                it = next_it;
                if (it >= end) break;
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
            return _text_size;
        }

        unsigned TextWidget::line_height() const noexcept {
            return font_size.get();
        }
        unsigned TextWidget::pos_to_index(Point pos) const noexcept {
            const char* text_str = text().c_str();
            unsigned byte_count = text().size();
            unsigned char_count = _chars.size();

            auto hdis = [](Point a, Point b) { return std::abs(a.x - b.x); };

            unsigned maybe_idx = float(pos.x) / float(_text_size.width) * char_count;
            maybe_idx = std::clamp<unsigned>(maybe_idx, 0, char_count + 1);
            Point maybe_pos = _char_index_to_pos(maybe_idx);

            if (maybe_idx > 0) {
                Point i_pos = _char_index_to_pos(maybe_idx - 1);
                while (hdis(i_pos, pos) < hdis(maybe_pos, pos)) {
                    maybe_pos = i_pos;
                    maybe_idx--;
                    if (maybe_idx == 0) break;
                    i_pos = _char_index_to_pos(maybe_idx - 1);
                }
            }
            if (maybe_idx < char_count) {
                Point i_pos = _char_index_to_pos(maybe_idx + 1);
                while (hdis(i_pos, pos) < hdis(maybe_pos, pos)) {
                    maybe_pos = i_pos;
                    maybe_idx++;
                    if (maybe_idx >= char_count) break;
                    i_pos = _char_index_to_pos(maybe_idx + 1);
                }
            }

            return maybe_idx;
        }
        Point TextWidget::index_to_pos(unsigned index) const noexcept {
            return _char_index_to_pos(index);
        }

        unsigned TextWidget::char_count() const noexcept { return _chars.size(); }
        unsigned TextWidget::char_index_to_byte_beg_index(unsigned idx) const noexcept {
            if (_chars.size() == 0) return 0;
            if (idx >= _chars.size())
                return _chars[_chars.size() - 1].utf8_str_end_idx;
            return _chars[idx].utf8_str_beg_idx;
        }
        unsigned TextWidget::char_index_to_byte_end_index(unsigned idx) const noexcept {
            if (_chars.size() == 0) return 0;
            if (idx >= _chars.size())
                return _chars[_chars.size() - 1].utf8_str_end_idx;
            return _chars[idx].utf8_str_end_idx;
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
            unsigned byte_idx = char_index_to_byte_beg_index(index);
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
            if (cursor_index() == 0) return;
            _delete_at_index(cursor_index() - 1, 1);
            cursor_index(cursor_index() - 1);
        }
        void EditableTextWidget::delete_back() noexcept {
            _delete_at_index(cursor_index(), 1);
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
            int byte_idx = char_index_to_byte_beg_index(idx);
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
            int byte_beg_idx = char_index_to_byte_beg_index(idx);
            int byte_end_idx = char_index_to_byte_beg_index(idx + len);
            std::string ts = text;
            ts.erase(byte_beg_idx, byte_end_idx - byte_beg_idx);
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
            } else if (e->is(CharEvent::type()) && Application::focus_widget() == this) {
                uint32_t cp = ((CharEvent*)e)->codepoint();
                int idx = this->cursor_index();
                this->insert_char(idx, cp);
                return true;
            } else if (e->is(KeyboardEvent::type()) && Application::focus_widget() == this) {
                KeyboardEvent* ke = (KeyboardEvent*)e;
                if (ke->action() == KeyboardKey::backspace && ke->key_mode() == KeyboardKeyMode::press)
                    this->delete_front();
                else if (ke->action() == KeyboardKey::delete_ && ke->key_mode() == KeyboardKeyMode::press)
                    this->delete_back();
                else if (ke->action() == KeyboardKey::left && ke->key_mode() == KeyboardKeyMode::press) {
                    if (cursor_index() > 0)
                        cursor_index(cursor_index() - 1);
                } else if (ke->action() == KeyboardKey::right && ke->key_mode() == KeyboardKeyMode::press) {
                    if (cursor_index() < char_count())
                        cursor_index(cursor_index() + 1);
                } else
                    return false;
                return true;
            }
            return false;
        }

        Size TextBoxWidget::perform_layout(Constraint constraint) noexcept {
            return constraint.max();
        }

    } // namespace widgets
} // namespace RolUI
