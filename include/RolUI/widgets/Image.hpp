#pragma once

#include "RolUI/Image.hpp"
#include "RolUI/Property.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/IPainter.hpp"

namespace RolUI {
    namespace widget {

        class Image : public Widget {
          public:
            Property<RolUI::Image> image{this};

          public:
            Image(Widget* parent) noexcept;
            Image(Widget* parent, RolUI::Image image) noexcept;
            ~Image() override;

            void adjust_size() noexcept;

            void on_draw(IPainter* painter) override;
        };

    } // namespace widget
} // namespace RolUI
