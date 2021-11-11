#pragma once

#include "RolUI/Color.hpp"
#include "RolUI/Rect.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/IPainter.hpp"

namespace RolUI {
    namespace widget {

        class Rect : public Widget {
          public:
            struct StyleProperty {
                unsigned round = 0;
                unsigned border_width = 0;
                Color border_color = {0, 0, 0, 255};
                Color background_color = {255, 255, 255, 255};
            };

          public:
            Rect() noexcept {}
            Rect(int x, int y, unsigned w, unsigned h,
                 unsigned round = 0) noexcept;

            ~Rect() override;

            unsigned round() const noexcept;
            unsigned border_width() const noexcept;

            Color border_color() const noexcept;
            Color background_color() const noexcept;

            const StyleProperty& style_property() const noexcept;

            void set_round(unsigned r) noexcept;
            void set_border_width(unsigned w) noexcept;

            void set_border_color(const Color& c) noexcept;
            void set_background_color(const Color& c) noexcept;

            void set_style_property(const StyleProperty& property) noexcept;

            void on_draw(IPainter* painter) override;

          private:
            StyleProperty _propertys = {};
        };

    } // namespace widget
} // namespace RolUI
