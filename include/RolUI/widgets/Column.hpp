#pragma once

#include "../Property.hpp"
#include "./container.hpp"

namespace RolUI {
    namespace widget {

        class Column : public MultiChildWidget {
          public:
            enum CrossAxisAlignment {
                left,
                center,
                right
            };

          public:
            Property<CrossAxisAlignment> cross_axis_alignment{this, center};

          public:
            Column() noexcept;

            Size perlayout(Constraint constraint) override;
        };

    } // namespace widget
} // namespace RolUI