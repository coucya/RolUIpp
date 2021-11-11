#pragma once

#include <cstdio>
#include <string>

#include "RolUI/Widget.hpp"
#include "RolUI/IPainter.hpp"

namespace RolUI {
    namespace widget {

        class Text : public Widget {
          public:
            struct StyleProperty {
                unsigned int font_size = 12;
                Color font_color = {0, 0, 0, 255};
                const char* font_name = "";
            };

          public:
            Text() noexcept;
            ~Text() override;

            const std::string& text() const noexcept;
            const char* font() const noexcept;
            unsigned font_size() const noexcept;
            Color font_color() const noexcept;

            const StyleProperty& style_property() const noexcept;

            void set_text(const char* text) noexcept;
            void set_text(const char* text, unsigned len) noexcept;
            void set_text(const std::string& text) noexcept;

            void set_font(const char* name) noexcept;

            void set_font_size(unsigned size) noexcept;
            void set_font_color(Color c) noexcept;

            void on_draw(IPainter* painter) override;

            void set_style_property(const StyleProperty& property) noexcept;

          private:
            void _update_size();

          private:
            StyleProperty _propertys;
            std::string _text;
        };

    } // namespace widget
} // namespace RolUI
