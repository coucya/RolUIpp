#pragma once

#include <stdint.h>

#include "RolUI/Color.hpp"
#include "RolUI/Rect.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/IPainter.hpp"

namespace RolUI {
    namespace widget {

        class Rect : public Widget {
          private:
            uint32_t _round = 0;
            uint32_t _border_width = 0;
            Color _border_color;
            Color _background_color;

          public:
            Rect() noexcept {}
            Rect(int32_t x, int32_t y, uint32_t w, uint32_t h,
                 uint32_t round = 0) noexcept;

            ~Rect() override;

            uint32_t round() const noexcept;
            uint32_t border_width() const noexcept;

            Color border_color() const noexcept;
            Color background_color() const noexcept;

            void set_round(uint32_t r) noexcept;
            void set_border_width(uint32_t w) noexcept;

            void set_border_color(const Color& c) noexcept;
            void set_background_color(const Color& c) noexcept;

            void on_draw(IPainter* painter) override;
        };

    } // namespace widget
} // namespace RolUI
