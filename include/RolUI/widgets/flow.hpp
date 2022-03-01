#pragma once

#include "../Property.hpp"
#include "../Widget.hpp"

namespace RolUI {
    namespace widgets {

        enum class CrossAxisAlignment {
            start,
            center,
            end
        };

        class Column : public MultiChildWidget {
          public:
            Property<CrossAxisAlignment> cross_axis_alignment{this, CrossAxisAlignment::center};

          public:
            Column() noexcept;

            Size perform_layout(Constraint constraint) noexcept override;
        };

        class Row : public MultiChildWidget {
          public:
            Property<CrossAxisAlignment> cross_axis_alignment{this, CrossAxisAlignment::center};

          public:
            Row() noexcept;

            Size perform_layout(Constraint constraint) noexcept override;
        };
    } // namespace widgets
} // namespace RolUI