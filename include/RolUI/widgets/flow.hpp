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

        class ColumnWidget : public MultiChildWidget {
          public:
            Property<ColumnWidget, CrossAxisAlignment> cross_axis_alignment{this, CrossAxisAlignment::center};
            Property<ColumnWidget, int> gap{this, 0};

          public:
            ColumnWidget() noexcept;
            Size perform_layout(Constraint constraint) noexcept override;
        };

        class RowWidget : public MultiChildWidget {
          public:
            Property<RowWidget, CrossAxisAlignment> cross_axis_alignment{this, CrossAxisAlignment::center};
            Property<RowWidget, int> gap{this, 0};

          public:
            RowWidget() noexcept;
            Size perform_layout(Constraint constraint) noexcept override;
        };

        namespace _details {
            class FlowGridWidgetBase : public MultiChildWidget {
              public:
                FlowGridWidgetBase() noexcept;

                float flex_of(int index) const noexcept;
                float flex_sum() const noexcept;

                FlowGridWidgetBase* add_child(Widget* child, float flex = 1.0f) noexcept;
                FlowGridWidgetBase* set_child(int index, Widget* child, float flex = 1.0f) noexcept;

                void remove_child(Widget* child) noexcept;
                void remove_child(int index) noexcept;

              private:
                std::vector<float> _flexs;
            };

        } // namespace _details

        class ColumnGridWidget : public _details::FlowGridWidgetBase {
          public:
            ColumnGridWidget() noexcept;
            Size perform_layout(Constraint constraint) noexcept override;
        };

        class RowGridWidget : public _details::FlowGridWidgetBase {
          public:
            RowGridWidget() noexcept;
            Size perform_layout(Constraint constraint) noexcept override;
        };

    } // namespace widgets
} // namespace RolUI