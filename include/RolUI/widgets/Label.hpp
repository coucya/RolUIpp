#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

#include "RolUI/Color.hpp"
#include "RolUI/IEvent.hpp"
#include "RolUI/Widget.hpp"

#include "./Rect.hpp"
#include "./Text.hpp"

namespace RolUI {
    namespace widget {

        class Label : public Widget {
          public:
            Label() noexcept;
            Label(std::string text) noexcept;
            Label(std::string text, Widget* parent) noexcept;
            ~Label();

            const std::string& text() const noexcept;

            const char* font() const noexcept;
            size_t font_size() const noexcept;

            Color background_color() const noexcept;

            uint32_t border_width() const noexcept;
            Color border_color() const noexcept;

            uint32_t round() const noexcept;

            void set_text(std::string text) noexcept;

            void set_font(const char* name) noexcept;
            void set_font_size(size_t size) noexcept;

            void set_background_color(Color c) noexcept;
            void set_border_width(uint32_t w) noexcept;
            void set_border_color(Color c) noexcept;
            void set_round(uint32_t r) noexcept;

          private:
            void _init_part(std::string&& text) noexcept;

          private:
            widget::Rect _rect_widget;
            widget::Text _text_widget;
        };

    } // namespace widget
} // namespace RolUI
