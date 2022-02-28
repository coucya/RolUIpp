#pragma once

#include "../Property.hpp"
#include "../Widget.hpp"

namespace RolUI {
    namespace widget {

        class Row : public MultiChildWidget {
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

            Size layout(Constraint constraint) noexcept override;
        };

    } // namespace widget
} // namespace RolUI