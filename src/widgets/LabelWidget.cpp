

#include "RolUI/widgets/LabelWidget.h"

namespace RolUI {

    LabelWidget::LabelWidget() noexcept
        : _text(nullptr), _text_len(0),
          _font_size(15), _font_color(Color()) {}

    LabelWidget::~LabelWidget() {}

    LabelWidget& LabelWidget::set_font(const char* name) noexcept {
        _font_name = name;
        return *this;
    }
    LabelWidget& LabelWidget::set_font_size(uint32_t size) noexcept {
        _font_size = size;
        return *this;
    }
    LabelWidget& LabelWidget::set_font_color(Color c) noexcept {
        _font_color = c;
        return *this;
    }

    LabelWidget& LabelWidget::set_text(const char* text) {
        if (text == nullptr) return *this;

        uint32_t len = 0;
        const char* text_it = text;
        while (*(text_it++) != '\0') len++;

        set_text(text, len);

        return *this;
    }
    LabelWidget& LabelWidget::set_text(const char* text, uint32_t len) noexcept {
        if (text == nullptr) return *this;

        _text = text;
        _text_len = len;
        return *this;
    }

    LabelWidget& LabelWidget::set_x(int32_t x) noexcept {
        _pos.x = x;
        return *this;
    }
    LabelWidget& LabelWidget::set_y(int32_t y) noexcept {
        _pos.y = y;
        return *this;
    }
    LabelWidget& LabelWidget::set_pos(int32_t x, int32_t y) noexcept {
        _pos = Point(x, y);
        return *this;
    }

    void LabelWidget::draw(IPainter* painter) {
        painter->set_font_size(_font_size);
        painter->set_font_color(_font_color);
        painter->set_font(_font_name);
        painter->draw_text(_pos, _text, _text_len);
    }

} // namespace RolUI
