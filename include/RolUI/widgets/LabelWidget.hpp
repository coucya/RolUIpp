#pragma once

#include <stdint.h>

#include "RolUI/Widget.h"
#include "RolUI/IPainter.h"

namespace RolUI {

    class LabelWidget : public Widget {
      private:
        bool _is_update_size;

        uint32_t _font_size;
        Color _font_color;
        uint32_t _text_len;
        const char* _font_name;
        const char* _text;

      public:
        LabelWidget() noexcept;
        ~LabelWidget() override;

        Size size() override;

        void set_text(const char* text);
        void set_text(const char* text, uint32_t len) noexcept;
        void set_font(const char* name) noexcept;
        void set_font_size(uint32_t size) noexcept;
        void set_font_color(Color c) noexcept;

        void draw(IPainter* painter) override;

      private:
        void _update_size();

        void set_size(const Size& size) noexcept;
        void set_size(uint32_t w, uint32_t h) noexcept;
    };

} // namespace RolUI
