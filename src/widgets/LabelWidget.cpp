
#include "RolUI/string.h"
#include "RolUI/widgets/LabelWidget.h"

namespace RolUI {

    LabelWidget::LabelWidget() noexcept
        : _text(nullptr), _text_len(0),
          _font_size(15), _font_color(Color()) {}

    LabelWidget::~LabelWidget() {}

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
    }

    void LabelWidget::draw(IPainter* painter) {
        painter->set_font_size(_font_size);
        painter->set_font_color(_font_color);
        painter->set_font(_font_name);
        painter->draw_text(_pos, _text, _text_len);
    }

} // namespace RolUI
