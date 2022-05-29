
#include <cstdint>
#include <cstring>

#include "utf8.h"

#include "RolUI/Object.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/Window.hpp"
#include "RolUI/Application.hpp"
#include "RolUI/Widget_event.hpp"
#include "RolUI-widgets/Text.hpp"
#include "RolUI-events/MouseEvent.hpp"
#include "RolUI-events/CharEvent.hpp"
#include "RolUI-events/KeyboardEvent.hpp"

namespace RolUI {
    namespace widgets {

        TextWidget ::TextWidget() noexcept {
            font.on_change.connect([this](int32_t font) { this->_did_text_change(); });
            text_size.on_change.connect([this](const unsigned&) { this->_did_text_change(); });
            text_color.on_change.connect([this](const Color&) { this->_did_text_change(); });
            text.on_change.connect([this](const std::string&) { this->_did_text_change(); });
        }
        TextWidget ::TextWidget(const std::string& str) noexcept
            : TextWidget() {
            text(str);
        }

        TextWidget::~TextWidget() {}

        void TextWidget::_update_size() noexcept {
            Window* win = Application::window();
            win->painter()->set_font_size(text_size);
            win->painter()->set_font(font());
            if (!text->empty()) {
                _text_size = win->painter()->text_size(text->c_str(), text->size());
            } else {
                _text_size = win->painter()->text_size(" ", text->size());
                _text_size.width = 0;
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
                _chars.push_back(CharInfo{cp, uint32_t(it - beg), uint32_t(next_it - beg)});
                it = next_it;
            }

            constexpr uint32_t T_MAX = 1024;
            uint32_t T_POS[T_MAX * 2];
            uint32_t* buff = nullptr;
            uint32_t* mins = &T_POS[0];
            uint32_t* maxs = &T_POS[0] + T_MAX;

            if (_chars.size() > T_MAX) {
                buff = new uint32_t[_chars.size()];
                mins = buff;
                maxs = buff + T_MAX;
            }

            IPainter* painter = Application::window()->painter();
            int n = painter->text_glyph_pos(beg, end - beg, mins, maxs, _chars.size());
            int i = 0;
            for (int i = 0; i < n; i++) {
                _chars[i].min_x = mins[i];
                _chars[i].max_x = maxs[i];
            }

            uint32_t max_x = n == 0 ? 0 : maxs[n - 1];
            _text_size.width = int(max_x);

            if (_chars.size() > T_MAX && buff != nullptr) delete[] buff;
        }

        void TextWidget::perform_draw(IPainter* painter) noexcept {
            painter->set_font_size(text_size);
            painter->set_font_color(text_color);
            painter->set_font(font());
            if (!text->empty())
                painter->fill_text(abs_pos(), text->c_str(), text->size());
        }

        Size TextWidget::perform_layout(Constraint constraint) noexcept {
            return _text_size;
        }

        unsigned TextWidget::line_height() const noexcept {
            return _text_size.height;
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
        unsigned TextWidget::char_index_to_byte_index(unsigned idx) const noexcept {
            if (_chars.size() == 0) return 0;
            if (idx >= _chars.size())
                return _chars[_chars.size() - 1].utf8_str_end_idx;
            return _chars[idx].utf8_str_beg_idx;
        }
        unsigned TextWidget::char_byte_size(unsigned idx) const noexcept {
            if (_chars.size() == 0) return 0;
            if (idx >= _chars.size()) return 0;
            return _chars[idx].utf8_str_end_idx - _chars[idx].utf8_str_beg_idx;
        }
        unsigned TextWidget::byte_index_to_char_index(unsigned idx) const noexcept {
            if (_chars.size() == 0) return 0;
            if (idx >= _chars[_chars.size() - 1].utf8_str_end_idx)
                return _chars.size();

            unsigned beg_it = 0, end_it = _chars.size();
            do {
                unsigned mid = beg_it + (end_it - beg_it) / 2;
                if (idx >= _chars[mid].utf8_str_beg_idx && idx < _chars[mid].utf8_str_end_idx) {
                    return mid;
                } else if (idx < _chars[mid].utf8_str_beg_idx) {
                    end_it = mid;
                } else if (idx >= _chars[mid].utf8_str_end_idx) {
                    beg_it = mid;
                }
            } while (beg_it != end_it);
            return beg_it;
        }

        Point TextWidget::_char_index_to_pos(unsigned index) const noexcept {
            if (index == 0) return {0, 0};
            if (index >= _chars.size()) return {int(_chars[_chars.size() - 1].max_x), 0};
            return {int(_chars[index].min_x), 0};
        }

        EditableTextWidget::EditableTextWidget() noexcept {
            cursor_index.on_change.connect([this](unsigned int index) {
                this->_update_cursor_pos();
            });
            text.on_change.connect([this](const std::string& text) {
                this->_update_cursor_pos();
            });
        }
        EditableTextWidget::~EditableTextWidget() {
            if (_is_blinking) {
                _is_blinking = false;
                _show_cursor = false;
                Application::clear_interval(_blink_timer_handle);
            }
        }

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
            int byte_idx = char_index_to_byte_index(idx);
            std::string ts = text();
            ts.insert(byte_idx, str, len);
            text = std::move(ts);
            cursor_index = cursor_index() + utf8nlen(str, len);
        }

        void EditableTextWidget::perform_draw(IPainter* painter) noexcept {
            TextWidget::perform_draw(painter);
            if (_show_cursor) {
                int ts = line_height();
                Point pos = abs_pos() + _cursor_pos;
                painter->set_line_width(2);
                painter->set_line_color({0, 0, 0});
                painter->draw_line(pos, Point{pos.x, pos.y + ts});
            }
        }

        void EditableTextWidget::_delete_at_index(unsigned idx, unsigned len) noexcept {
            int byte_beg_idx = char_index_to_byte_index(idx);
            int byte_end_idx = char_index_to_byte_index(idx + len);
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

            if (e->object_type_is<MousePressEvent>() && ((MouseEvent*)e)->action() == MouseKey::left) {
                MouseEvent* me = (MouseEvent*)e;
                this->cursor_index = this->pos_to_index(me->pos() - this->abs_pos());
                Application::set_focus_widget(this);
                return true;
            } else if (e->object_type_is<FocusChangeEvent>()) {
                FocusChangeEvent* fce = (FocusChangeEvent*)e;
                if (fce->current_value())
                    this->set_blink(true);
                else
                    this->set_blink(false);
                return true;
            } else if (e->object_type_is<CharEvent>() && Application::focus_widget() == this) {
                uint32_t cp = ((CharEvent*)e)->codepoint();
                int idx = this->cursor_index();
                this->insert_char(idx, cp);
                return true;
            } else if (e->object_type_is<KeyboardEvent>() && Application::focus_widget() == this) {
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

        const ObjectType* TextWidget::object_type() const noexcept { return object_type_of<TextWidget>(); }
        const ObjectType* EditableTextWidget::object_type() const noexcept { return object_type_of<EditableTextWidget>(); }
        const ObjectType* TextBoxWidget::object_type() const noexcept { return object_type_of<TextBoxWidget>(); }

    } // namespace widgets

    RolUI_impl_object_type_of_with_namespace(widgets, TextWidget, Widget);
    RolUI_impl_object_type_of_with_namespace(widgets, EditableTextWidget, widgets::TextWidget);
    RolUI_impl_object_type_of_with_namespace(widgets, TextBoxWidget, widgets::EditableTextWidget);

} // namespace RolUI
