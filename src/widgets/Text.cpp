
#include "RolUI/utility/utf8ext.h"

#include "RolUI/Object.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/Window.hpp"
#include "RolUI/Application.hpp"
#include "RolUI/widgets/Text.hpp"
#include "RolUI/events/Widget_event.hpp"
#include "RolUI/events/MouseEvent.hpp"
#include "RolUI/events/CharEvent.hpp"
#include "RolUI/events/KeyboardEvent.hpp"

namespace RolUI {
    namespace widgets {

        TextSpanWidget ::TextSpanWidget() noexcept {
            font_size.on_change.connect([this](const unsigned&) { this->_did_text_change(); });
            font_color.on_change.connect([this](const Color&) { this->_did_text_change(); });
            font_name.on_change.connect([this](const std::string&) { this->_did_text_change(); });
            text.on_change.connect([this](const std::string&) { this->_did_text_change(); });
        }
        TextSpanWidget ::TextSpanWidget(const std::string& str) noexcept
            : TextSpanWidget() {
            text(str);
        }

        TextSpanWidget::~TextSpanWidget() {}

        void TextSpanWidget::_update_size() noexcept {
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

        void TextSpanWidget::_did_text_change() noexcept {
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

        void TextSpanWidget::draw(IPainter* painter) noexcept {
            painter->set_font_size(font_size);
            painter->set_font_color(font_color);

            if (!font_name->empty())
                painter->set_font(font_name->c_str());
            else
                painter->set_font("default");

            if (!text->empty())
                painter->draw_text(abs_pos(), text->c_str(), text->size());
        }

        Size TextSpanWidget::perform_layout(Constraint constraint) noexcept {
            return _text_size;
        }

        unsigned TextSpanWidget::line_height() const noexcept {
            return _text_size.height;
        }
        unsigned TextSpanWidget::pos_to_index(Point pos) const noexcept {
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
        Point TextSpanWidget::index_to_pos(unsigned index) const noexcept {
            return _char_index_to_pos(index);
        }

        unsigned TextSpanWidget::char_count() const noexcept { return _chars.size(); }
        unsigned TextSpanWidget::char_index_to_byte_index(unsigned idx) const noexcept {
            if (_chars.size() == 0) return 0;
            if (idx >= _chars.size())
                return _chars[_chars.size() - 1].utf8_str_end_idx;
            return _chars[idx].utf8_str_beg_idx;
        }
        unsigned TextSpanWidget::char_byte_size(unsigned idx) const noexcept {
            if (_chars.size() == 0) return 0;
            if (idx >= _chars.size()) return 0;
            return _chars[idx].utf8_str_end_idx - _chars[idx].utf8_str_beg_idx;
        }
        unsigned TextSpanWidget::byte_index_to_char_index(unsigned idx) const noexcept {
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

        Point TextSpanWidget::_char_index_to_pos(unsigned index) const noexcept {
            if (index == 0) return {0, 0};
            if (index >= _chars.size()) return {int(_chars[_chars.size() - 1].max_x), 0};
            return {int(_chars[index].min_x), 0};
        }

        RichTextLineWidget::RichTextLineWidget() noexcept {}
        RichTextLineWidget::~RichTextLineWidget() {}

        unsigned RichTextLineWidget::pos_to_index(Point pos) const noexcept {
            int char_count_ = 0;
            for (int i = 0; i < child_count(); i++) {
                Widget* child = this->child(i);
                ITextSpan* span = object_try_cast<ITextSpan>(child);
                if (!span) continue;

                if (child->rect().contain(pos)) {
                    Point tp = pos - child->pos();
                    return char_count_ + span->pos_to_index(tp);
                }
                char_count_ += span->char_count();
            }
            return pos.x < 0 ? 0 : char_count();
        }
        Point RichTextLineWidget::index_to_pos(unsigned index) const noexcept {
            int char_count_ = 0;
            for (int i = 0; i < child_count(); i++) {
                Widget* child = this->child(i);
                ITextSpan* span = object_try_cast<ITextSpan>(child);
                if (!span) continue;

                unsigned span_char_count = span->char_count();
                if (char_count_ + span_char_count > index) {
                    unsigned span_char_index = index - char_count_;
                    return span->index_to_pos(span_char_index) + child->pos();
                }
                char_count_ += span_char_count;
            }
            return {0, 0};
        }
        unsigned RichTextLineWidget::char_count() const noexcept {
            int char_count_ = 0;
            for (int i = 0; i < child_count(); i++) {
                Widget* child = this->child(i);
                ITextSpan* span = object_try_cast<ITextSpan>(child);
                char_count_ += span ? span->char_count() : 0;
            }
            return char_count_;
        }
        unsigned RichTextLineWidget::line_height() const noexcept {
            unsigned max_line_height = 0;
            for (int i = 0; i < child_count(); i++) {
                Widget* child = this->child(i);
                ITextSpan* span = object_try_cast<ITextSpan>(child);
                max_line_height = span && span->line_height() > max_line_height ? span->line_height() : max_line_height;
            }
            return max_line_height;
        }

        RichTextLineWidget* RichTextLineWidget::set_child(Widget* child, int index) noexcept {
            if (child && child->object_type()->is_superclass<ITextSpan>())
                FlexWidget::set_child(child, index);
            return this;
        }
        RichTextLineWidget* RichTextLineWidget::add_child(Widget* child) noexcept {
            if (child && child->object_type()->is_superclass<ITextSpan>())
                FlexWidget::add_child(child);
            return this;
        }
        RichTextLineWidget* RichTextLineWidget::insert_child(int index, Widget* child) noexcept {
            if (child && child->object_type()->is_superclass<ITextSpan>())
                FlexWidget::insert_child(index, child);
            return this;
        }

        RichTextWidget::RichTextWidget() noexcept {}
        RichTextWidget::~RichTextWidget() {}

        unsigned RichTextWidget::pos_to_index(Point pos) const noexcept {
            int char_count_ = 0;
            for (int i = 0; i < child_count(); i++) {
                RichTextLineWidget* child = object_try_cast<RichTextLineWidget>(this->child(i));
                if (!child) continue;

                Rect child_rect = child->rect();
                if (child_rect.contain(pos)) {
                    Point tp = pos - child->pos();
                    return char_count_ + child->pos_to_index(tp);
                }
                char_count_ += child->char_count();
            }
            return pos.x < 0 ? 0 : char_count();
        }
        Point RichTextWidget::index_to_pos(unsigned index) const noexcept {
            int char_count_ = 0;
            for (int i = 0; i < child_count(); i++) {
                RichTextLineWidget* child = object_try_cast<RichTextLineWidget>(this->child(i));
                if (!child) continue;

                unsigned span_char_count = child->char_count();
                if (char_count_ + span_char_count > index) {
                    unsigned span_char_index = index - char_count_;
                    return child->index_to_pos(span_char_index) + child->pos();
                }
                char_count_ += span_char_count;
            }
            return {0, 0};
        }
        unsigned RichTextWidget::char_count() const noexcept {
            int char_count_ = 0;
            for (int i = 0; i < child_count(); i++) {
                RichTextLineWidget* child = object_try_cast<RichTextLineWidget>(this->child(i));
                if (!child) continue;
                char_count_ += child->char_count();
            }
            return char_count_;
        }

        RichTextWidget* RichTextWidget::set_child(Widget* child, int index) noexcept {
            if (child && child->object_type()->is_superclass<RichTextLineWidget>())
                ColumnWidget::set_child(child, index);
            return this;
        }
        RichTextWidget* RichTextWidget::add_child(Widget* child) noexcept {
            if (child && child->object_type()->is_superclass<RichTextLineWidget>())
                ColumnWidget::add_child(child);
            return this;
        }
        RichTextWidget* RichTextWidget::insert_child(int index, Widget* child) noexcept {
            if (child && child->object_type()->is_superclass<RichTextLineWidget>())
                ColumnWidget::insert_child(index, child);
            return this;
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

        void EditableTextWidget::draw(IPainter* painter) noexcept {
            TextSpanWidget::draw(painter);
            if (_show_cursor) {
                int ts = line_height();
                painter->set_stroke_width(2);
                painter->set_stroke_color({0, 0, 0});
                painter->draw_vline(abs_pos() + _cursor_pos, ts);
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

        const ObjectType* TextSpanWidget::object_type() const noexcept { return object_type_of<TextSpanWidget>(); }
        const ObjectType* RichTextLineWidget::object_type() const noexcept { return object_type_of<RichTextLineWidget>(); }
        const ObjectType* RichTextWidget::object_type() const noexcept { return object_type_of<RichTextWidget>(); }
        const ObjectType* EditableTextWidget::object_type() const noexcept { return object_type_of<EditableTextWidget>(); }
        const ObjectType* TextBoxWidget::object_type() const noexcept { return object_type_of<TextBoxWidget>(); }

    } // namespace widgets

    RolUI_impl_object_type_of_with_namespace(widgets, TextSpanWidget, widgets::ITextSpan, Widget);
    RolUI_impl_object_type_of_with_namespace(widgets, RichTextLineWidget, widgets::ITextSpan, widgets::FlexWidget);
    RolUI_impl_object_type_of_with_namespace(widgets, RichTextWidget, widgets::ColumnWidget);
    RolUI_impl_object_type_of_with_namespace(widgets, EditableTextWidget, widgets::TextSpanWidget);
    RolUI_impl_object_type_of_with_namespace(widgets, TextBoxWidget, widgets::EditableTextWidget);

} // namespace RolUI
