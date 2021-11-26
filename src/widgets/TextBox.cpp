
#include <algorithm>

#include "RolUI/utility/utf8.h"
#include "RolUI/utility/utf8ext.h"
#include "RolUI/IEvent.hpp"
#include "RolUI/IEventListener.hpp"
#include "RolUI/Point.hpp"
#include "RolUI/Size.hpp"
#include "RolUI/Vector.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/IPainter.hpp"
#include "RolUI/WidgetState.hpp"
#include "RolUI/Window.hpp"
#include "RolUI/events/CharEvent.hpp"
#include "RolUI/events/MouseEvent.hpp"
#include "RolUI/events/Widget_event.hpp"
#include "RolUI/widgets/TextBox.hpp"

namespace RolUI {
    namespace widget {

        TextBox::TextBox(Widget* parent) noexcept
            : Widget(parent) {
            _init();
        }

        void TextBox::_init() noexcept {
            _init_part();
            _init_event_bind();

            _cursor_timer.on_timeout.connect(this, &TextBox::_cursor_timer_func);
            _cursor_pos = {_padding_widget.pos().x, 0};

            set_focusable(true);

            Style default_, focus;
            default_.round = 0;
            default_.border_color = {150, 150, 150, 255};
            default_.border_width = 1;
            default_.background_color = {255, 255, 255};
            default_.color = {16, 16, 16};
            default_.font_name = "default";
            default_.font_size = 16;

            focus.border_color = {0, 0, 0};
            focus.border_width = 1;
            StyleSheet ss;
            ss.set_style(default_, WIDGET_STATE_DEFAULT);
            ss.set_style(focus, WIDGET_STATE_FOCUS);

            set_style(default_);
            set_style_sheet(ss);
        }
        void TextBox::_init_event_bind() noexcept {
            padding.on_change.connect([&](Vec2i p) {
                _padding_widget.set_size(-(p.x * 2), -(p.y * 2));
            });
            cursor_width.on_change.connect([&](unsigned w) {
                _cursor_widget.set_size(w, _cursor_widget.size().height);
            });
            text.on_change.connect([this](const std::string& s) {
                this->_codepoint_count = utf8len(s.c_str());
                this->cursor_to_index(this->cursor_index());
            });

            _text_widget.add_listener(SizeChangeEvent::type(), [&](IEvent* e) {
                SizeChangeEvent* sce = (SizeChangeEvent*)e;
                Size s = sce->current_value();
                _cursor_widget.set_size(1, s.height + s.height * 0.2);
                return false;
            });

            add_listener(SizeChangeEvent::type(), [this](IEvent* e) {
                this->_update_text_pos();
                return false;
            });
            add_listener(FocusChangeEvent::type(), [this](IEvent* e) {
                FocusChangeEvent* fce = (FocusChangeEvent*)e;
                if (fce->current_value() == true) {
                    cursor_to_index(this->_codepoint_count);
                    _cursor_timer.start(0.5, false);
                } else {
                    _cursor_widget.set_pos(-10, 0);
                    _cursor_timer.stop();
                }

                return true;
            });

            add_listener(MousePressEvent_type(), [this](IEvent* e) {
                MouseEvent* me = (MouseEvent*)e;
                Point pos = me->pos() - this->abs_pos();

                this->set_focus();
                this->cursor_to_index(this->pos_to_index(pos));
                return true;
            });

            add_listener(CharEvent::type(), [this](IEvent* e) {
                CharEvent* ce = (CharEvent*)e;
                this->insert_at_index(this->cursor_index(), ce->c_char(), strlen(ce->c_char()));
                return true;
            });
        }
        void TextBox::_init_part() noexcept {
            round.on_change.connect(&_rect_widget.round);
            border_color.on_change.connect(&_rect_widget.border_color);
            border_width.on_change.connect(&_rect_widget.border_width);
            background_color.on_change.connect(&_rect_widget.background_color);
            font_name.on_change.connect(&_text_widget.font_name);
            font_size.on_change.connect(&_text_widget.font_size);
            color.on_change.connect(&_text_widget.font_color);
            text.on_change.connect(&_text_widget.text);

            _rect_widget.set_size_relative(RelativeTarget::parent, SizeMode::relative);

            _padding_widget.set_size_relative(RelativeTarget::parent, SizeMode::relative);
            _padding_widget.set_pos_relative(RelativeTarget::parent, AnchorPoint::centre_middle, AnchorPoint::centre_middle);

            _text_widget.set_pos_relative(RelativeTarget::parent, AnchorPoint::left_middle, AnchorPoint::left_middle);

            _cursor_widget.set_pos(-10, 0);
            _cursor_widget.set_size(1, _text_widget.size().height);
            _cursor_widget.background_color = {0, 0, 0};
            _cursor_widget.set_pos_relative(RelativeTarget::parent, AnchorPoint::left_middle, AnchorPoint::centre_middle);

            _padding_widget.add_part(&_text_widget);

            add_part(&_rect_widget);
            add_part(&_padding_widget);
            add_part(&_cursor_widget);
        }

        void TextBox::_cursor_timer_func(double interval) noexcept {
            if (_cursor_widget.pos().x >= 0) {
                _cursor_widget.set_pos(-10, 0);
            } else
                _cursor_widget.set_pos(_cursor_pos.x, 0);
        }

        unsigned TextBox::pos_to_index(Point pos) const noexcept {
            return _pos_to_index(pos);
        }
        Point TextBox::index_to_pos(unsigned idx) const noexcept {
            return _index_to_pos(idx);
        }

        unsigned TextBox::cursor_index() const noexcept { return _cursor_index; }

        void TextBox::cursor_to_prev_char() noexcept {
            unsigned i = std::max<int>(0, (int)cursor_index() - 1);
            cursor_to_index(i);
        }
        void TextBox::cursor_to_next_char() noexcept {
            cursor_to_index(cursor_index() + 1);
        }

        void TextBox::cursor_to_index(unsigned idx) noexcept {
            _cursor_to_index(idx);
            _update_text_pos();
        }

        void TextBox::insert_at_index(unsigned idx, const char* str, unsigned len) noexcept {
            if (_insert_at_index(idx, str, len))
                cursor_to_index(cursor_index() + utf8nlen(str, len));
        }
        void TextBox::delete_at_index(unsigned idx) noexcept {
            if (_delete_at_index(idx, 1)) {
                if (idx < cursor_index())
                    cursor_to_index(cursor_index() - 1);
                else
                    cursor_to_index(cursor_index());
            }
        }
        void TextBox::delete_cursor_prev() noexcept {
            if (cursor_index() == 0) return;
            delete_at_index(cursor_index() - 1);
        }
        void TextBox::delete_cursor_next() noexcept {
            delete_at_index(cursor_index());
        }

        void TextBox::adjust_size() noexcept {
            Size text_size = _text_widget.size();
            Size s = text_size + padding.get() * 2;
            set_size(s);
        }

        void TextBox::set_style(const Style& style) {
            round = style.round;
            border_width = style.border_width;
            border_color = style.border_color;
            background_color = style.background_color;

            font_name = style.font_name;
            font_size = style.font_size;

            color = style.color;
        }

        bool TextBox::_insert_at_index(unsigned idx, const char* str, unsigned len) noexcept {
            unsigned idx_u = utf8utf8index(text.get().c_str(), idx);
            std::string ts = text.get();
            ts.insert(idx_u, str, len);
            if (utf8valid(ts.c_str()) == nullptr) {
                _codepoint_count = utf8len(ts.c_str());
                text = std::move(ts);
                return true;
            }
            return false;
        }
        bool TextBox::_delete_at_index(unsigned idx, unsigned len) noexcept {
            const char* str_beg = text.get().c_str();
            const char* str_end = text.get().c_str() + text.get().size();
            unsigned del_beg = utf8utf8index(str_beg, idx);
            unsigned del_len = utf8nutf8index(str_beg + del_beg, len, (str_end - str_beg) - del_beg);

            std::string ts = text.get();
            ts.erase(del_beg, del_len);
            if (utf8valid(ts.c_str()) == nullptr) {
                _codepoint_count = utf8len(ts.c_str());
                text = std::move(ts);
                return true;
            }
            return false;
        }

        unsigned TextBox::_pos_to_index(Point pos) const noexcept {
            Point text_pos = _text_widget.pos() + _padding_widget.pos();
            Point p = pos - text_pos;
            return _pos_to_index__text_widget(p);
        }
        Point TextBox::_index_to_pos(unsigned idx) const noexcept {
            Point text_pos = _text_widget.pos() + _padding_widget.pos();
            Point idx_pos = _index_to_pos__text_widget(idx);
            return idx_pos + text_pos;
        }

        unsigned TextBox::_pos_to_index__text_widget(Point pos) const noexcept {
            unsigned maybe_idx = ((float)pos.x / (float)_text_widget.size().width) * (float)text.get().size();
            Point maybe_pos = _index_to_pos__text_widget__utf8(maybe_idx);

            unsigned text_size = text.get().size();
            if (maybe_idx > text_size)
                maybe_idx = text_size;

            for (unsigned i = maybe_idx; i > 0; i--) {
                Point pos_t = _index_to_pos__text_widget__utf8(i - 1);
                if (std::abs(pos_t.x - pos.x) <= std::abs(maybe_pos.x - pos.x)) {
                    maybe_pos = pos_t;
                    maybe_idx = i - 1;
                } else
                    break;
            }

            for (unsigned i = maybe_idx + 1; i < text.get().size(); i++) {
                Point pos_t = _index_to_pos__text_widget__utf8(i);
                if (std::abs(pos_t.x - pos.x) <= std::abs(maybe_pos.x - pos.x)) {
                    maybe_pos = pos_t;
                    maybe_idx = i;
                } else
                    break;
            }

            return utf8codepointindex(text.get().c_str(), maybe_idx);
        }

        Point TextBox::_index_to_pos__text_widget(unsigned idx) const noexcept {
            if (!window()) return {0, 0};
            unsigned idx_t = 0;
            if (idx > _codepoint_count) {
                idx = _codepoint_count;
                idx_t = text.get().size();
            } else
                idx_t = utf8utf8index(text.get().c_str(), idx);
            return _index_to_pos__text_widget__utf8(idx_t);
        }
        Point TextBox::_index_to_pos__text_widget__utf8(unsigned idx) const noexcept {
            idx = std::min<unsigned>(idx, text.get().size());
            IPainter* painter = window()->painter();
            Size s = painter->text_size(text.get().c_str(), idx);
            return {s.width, 0};
        }

        void TextBox::_cursor_to_index(unsigned idx) noexcept {
            // unsigned idx_u = 0;
            if (idx > _codepoint_count) {
                idx = _codepoint_count;
                // idx_u = text.get().size();
            }
            // } else
            //     idx_u = utf8utf8index(text.get().c_str(), idx);

            // _utf8_index = idx_u;
            _cursor_index = idx;

            _cursor_pos = _index_to_pos(_cursor_index);
            _cursor_widget.set_pos(-10, 0);
        }

        void TextBox::_update_text_pos() noexcept {
            Point _cursor_pos__padding = _cursor_pos - _padding_widget.pos();
            if (_cursor_pos__padding.x > _padding_widget.size().width) {
                int offset = _cursor_pos__padding.x - _padding_widget.size().width;
                _text_widget.move(-offset, 0);
            } else if (_cursor_pos__padding.x < 0) {
                int offset = -_cursor_pos__padding.x;
                _text_widget.move(offset, 0);
            }
            _cursor_to_index(cursor_index());
        }

    } // namespace widget
} // namespace RolUI
