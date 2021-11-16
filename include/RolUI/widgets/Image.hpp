#pragma once

#include "RolUI/Widget.hpp"
#include "RolUI/IPainter.hpp"

namespace RolUI {
    namespace widget {

        class Image : public Widget {
          public:
            struct StyleProperty {};

          public:
            Image(Widget* parent) noexcept;
            Image(Widget* parent, int image_handle) noexcept;
            ~Image() override;

            int image() const noexcept;
            void set_image(int img_handle) noexcept;

            const StyleProperty& style_property() const noexcept;
            void set_style_property(const StyleProperty& property) noexcept;

            void adjust_size() noexcept;

            void on_draw(IPainter* painter) override;

          private:
            StyleProperty _propertys;
            int _image_handle;
        };
    } // namespace widget
} // namespace RolUI
