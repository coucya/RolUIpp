#pragma once

#include "../Image.hpp"
#include "../Property.hpp"
#include "../Widget.hpp"
#include "../IPainter.hpp"

namespace RolUI {
    namespace widget {

        class Image : public Widget {
          public:
            enum Fit {
                fill,
                cover,
                contain,
            };

          public:
            Property<RolUI::Image> image{this};
            Property<Fit> fit{this};

          public:
            Image(RolUI::Image image, Fit fit = Fit::contain) noexcept;
            ~Image() override;

          protected:
            void on_draw(IPainter* painter) override;
            Size perlayout(Constraint constraint) override;
        };

    } // namespace widget
} // namespace RolUI
