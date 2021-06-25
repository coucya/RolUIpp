#pragma once

#include <stdint.h>

#include "RolUI/Widget.h"
#include "RolUI/IPainter.h"

namespace RolUI {

    class LabelWidget : public Widget {
      private:
        uint32_t _font_size;
        Color _font_color;
        uint32_t _text_len;
        const char* _font_name;
        const char* _text;

        Point _pos;

      public:
        LabelWidget() noexcept;
        ~LabelWidget() override;

        LabelWidget& set_text(const char* text);
        LabelWidget& set_text(const char* text, uint32_t len) noexcept;

        LabelWidget& set_font(const char* name) noexcept;
        LabelWidget& set_font_size(uint32_t size) noexcept;
        LabelWidget& set_font_color(Color c) noexcept;

        LabelWidget& set_x(int32_t x) noexcept;
        LabelWidget& set_y(int32_t y) noexcept;
        LabelWidget& set_pos(int32_t x, int32_t y) noexcept;

        void draw(IPainter* painter) override;
    };

} // namespace RolUI
