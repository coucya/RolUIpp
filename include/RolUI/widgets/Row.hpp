#pragma once

#include "../Widget.hpp"
#include "../Property.hpp"

namespace RolUI {
    namespace widget {

        class Row : public Widget {
          public:
            enum CrossAxisAlignment {
                top,
                center,
                bottom
            };

          public:
            Property<CrossAxisAlignment> cross_axis_alignment{this, center};

          public:
            Row() noexcept;

          protected:
            Size perlayout(Constraint constraint) override;
        };

    } // namespace widget
} // namespace RolUI