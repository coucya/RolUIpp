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
            ImageWidget() noexcept;
            ImageWidget(RolUI::Image image, Fit fit = Fit::contain) noexcept;
            ~ImageWidget() override;

            const ObjectType* object_type() const noexcept override;

          protected:
            void perform_draw(IPainter* painter) noexcept override;
            Size perform_layout(Constraint constraint) noexcept override;
        };

    } // namespace widgets

    RolUI_decl_object_type_of(widgets::ImageWidget);

} // namespace RolUI
