#pragma once

#include <stdint.h>
#include <string>

#include "RolUI/Widget.hpp"
#include "RolUI/IPainter.hpp"

namespace RolUI {

    class LabelWidget : public Widget {
      private:
        uint32_t _font_size;
        Color _font_color;
        const char* _font_name;
        std::string _text;

      public:
        LabelWidget() noexcept;
        ~LabelWidget() override;

        void set_text(const char* text);
        void set_text(const char* text, uint32_t len) noexcept;
        void set_text(const std::string& text);
        void set_font(const char* name) noexcept;
        void set_font_size(uint32_t size) noexcept;
        void set_font_color(Color c) noexcept;

        void on_draw(IPainter* painter) override;

      private:
        void _update_size();
    };

} // namespace RolUI
