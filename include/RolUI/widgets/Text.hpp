#pragma once

#include <stdint.h>
#include <string>

#include "RolUI/Widget.hpp"
#include "RolUI/IPainter.hpp"

namespace RolUI {
    namespace widget {

        class Text : public Widget {
          private:
            uint32_t _font_size = 12;
            Color _font_color;
            const char* _font_name;
            std::string _text;

          public:
            Text() noexcept;
            ~Text() override;

            const std::string& text() const noexcept;
            const char* font() const noexcept;
            uint32_t font_size() const noexcept;
            Color font_color() const noexcept;

            void set_text(const char* text) noexcept;
            void set_text(const char* text, uint32_t len) noexcept;
            void set_text(const std::string& text) noexcept;

            void set_font(const char* name) noexcept;

            void set_font_size(uint32_t size) noexcept;
            void set_font_color(Color c) noexcept;

            void on_draw(IPainter* painter) override;

          private:
            void _update_size();
        };

    } // namespace widget
} // namespace RolUI
