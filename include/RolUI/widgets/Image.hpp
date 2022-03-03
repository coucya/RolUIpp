#pragma once

#include "../Image.hpp"
#include "../Property.hpp"
#include "../Widget.hpp"
#include "../IPainter.hpp"

namespace RolUI {
    namespace widgets {

        class ImageWidget : public Widget {
          public:
            enum Fit {
                fill,
                cover,
                contain,
            };

          public:
            Property<ImageWidget, RolUI::Image> image{this};
            Property<ImageWidget, Fit> fit{this};

          public:
            ImageWidget(RolUI::Image image, Fit fit = Fit::contain) noexcept;
            ~ImageWidget() override;

          protected:
            void draw(IPainter* painter) noexcept override;
            Size perform_layout(Constraint constraint) noexcept override;
        };

    } // namespace widgets
} // namespace RolUI
