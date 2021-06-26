
#include "RolUI/string.h"
#include "RolUI/Window.h"
#include "RolUI/widgets/LabelWidget.h"

namespace RolUI {

    LabelWidget::LabelWidget() noexcept
        : _is_update_size(false), _text(nullptr), _text_len(0),
          _font_size(15), _font_color(Color()) {}

    LabelWidget::~LabelWidget() {}

    void LabelWidget::_update_size() {
        if (_is_update_size) return;

        Window* win = window();

        if (win == nullptr)
            _is_update_size = false;
        else {
            Size s = win->painter()->text_size(_text, _text_len);
            Widget::set_size(s);
            _is_update_size = true;
        }
    }

    Size LabelWidget::size() {
        _update_size();
        return Widget::size();
    }
    void LabelWidget::set_font(const char* name) noexcept {
        _font_name = name;
    }
    void LabelWidget::set_font_size(uint32_t size) noexcept {
        _font_size = size;
    }
    void LabelWidget::set_font_color(Color c) noexcept {
        _font_color = c;
    }

    void LabelWidget::set_text(const char* text) {
        if (text == nullptr) return;

        set_text(text, strlen(text));
    }
    void LabelWidget::set_text(const char* text, uint32_t len) noexcept {
        if (text == nullptr) return;

        _text = text;
        _text_len = len;

        _update_size();
    }

    void LabelWidget::draw(IPainter* painter) {
        painter->set_font_size(_font_size);
        painter->set_font_color(_font_color);
        painter->set_font(_font_name);
        painter->draw_text(_pos, _text, _text_len);
    }

} // namespace RolUI
