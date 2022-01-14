#pragma once

#include "../Widget.hpp"
#include "../Property.hpp"

namespace RolUI {
    namespace widget {

        class Column : public Widget {
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

          protected:
            Size perlayout(Constraint constraint) override;
        };

    } // namespace widget
} // namespace RolUI