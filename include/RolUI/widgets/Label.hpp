#pragma once

#include <string>

#include "RolUI/Color.hpp"
#include "RolUI/IEvent.hpp"
#include "RolUI/Vector.hpp"
#include "RolUI/Widget.hpp"

#include "./Rect.hpp"
#include "./Text.hpp"

namespace RolUI {
    namespace widget {

        class Label : public Widget {
          public:
            struct StyleProperty : public Text::StyleProperty, public Rect::StyleProperty {
                Vector padding = {0, 0};
            };

          public:
            Label() noexcept;
            Label(std::string text) noexcept;
            Label(std::string text, Widget* parent) noexcept;
            ~Label();

            const std::string& text() const noexcept;

            const char* font() const noexcept;
            unsigned font_size() const noexcept;
            Color font_color() const noexcept;

            Color background_color() const noexcept;

            unsigned border_width() const noexcept;
            Color border_color() const noexcept;

            unsigned round() const noexcept;

            Vector padding() const noexcept;

            const StyleProperty& style_property() const noexcept;

            void set_text(std::string text) noexcept;

            void set_font(const char* name) noexcept;
            void set_font_size(size_t size) noexcept;
            void set_font_color(Color c) noexcept;

            void set_background_color(Color c) noexcept;
            void set_border_width(unsigned w) noexcept;
            void set_border_color(Color c) noexcept;
            void set_round(unsigned r) noexcept;

            void set_padding(Vector) noexcept;
            void set_padding(int x, int y) noexcept;

            void set_style_property(const StyleProperty& property) noexcept;

            void adjust_size() noexcept;

          private:
            void _init_part(std::string&& text) noexcept;

          private:
            widget::Rect _rect_widget;
            widget::Text _text_widget;

            StyleProperty _propertys;
        };

    } // namespace widget
} // namespace RolUI
