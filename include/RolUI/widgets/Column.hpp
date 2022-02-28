#pragma once

#include "../Property.hpp"
#include "../Widget.hpp"

namespace RolUI {
    namespace widgets {

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

            Size layout(Constraint constraint) noexcept override;
        };

    } // namespace widgets
} // namespace RolUI