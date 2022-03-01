#pragma once

#include <vector>

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

        namespace _details {
            class FlowGridWidgetBase : public MultiChildWidget {
              public:
                FlowGridWidgetBase() noexcept;

                float flex_of(int index) const noexcept;
                float flex_sum() const noexcept;

                void add_child(Widget* child, float flex = 1.0f) noexcept;
                void set_child(int index, Widget* child, float flex = 1.0f) noexcept;

                void remove_child(Widget* child) noexcept;
                void remove_child(int index) noexcept;

              private:
                std::vector<float> _flexs;
            };

        } // namespace _details

        class ColumnGrid : public _details::FlowGridWidgetBase {
          public:
            ColumnGrid() noexcept;
            Size perform_layout(Constraint constraint) noexcept override;
        };

        class RowGrid : public _details::FlowGridWidgetBase {
          public:
            RowGrid() noexcept;
            Size perform_layout(Constraint constraint) noexcept override;
        };

    } // namespace widgets
} // namespace RolUI