#pragma once

#include <vector>

#include "RolUI/Object.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/Property.hpp"

namespace RolUI {
    namespace widgets {

        class ColumnWidget : public MultiChildWidget {
          public:
            Property<ColumnWidget, float> cross_axis_alignment{this, 0.0f};
            Property<ColumnWidget, int> gap{this, 0};

          public:
            ColumnWidget() noexcept;

            const ObjectType* object_type() const noexcept override;

          protected:
            Size perform_layout(Constraint constraint) noexcept override;
        };

        class RowWidget : public MultiChildWidget {
          public:
            Property<RowWidget, float> cross_axis_alignment{this, 0.0f};
            Property<RowWidget, int> gap{this, 0};

          public:
            RowWidget() noexcept;

            const ObjectType* object_type() const noexcept override;

          protected:
            Size perform_layout(Constraint constraint) noexcept override;
        };

        enum class FlexFit {
            fixed,
            percentage,
            flex,
            expand,
        };

        class FlexableWidget : public SingleChildWidget {
          public:
            Property<FlexableWidget, FlexFit> fit{this, FlexFit::flex};
            Property<FlexableWidget, float> flex{this, 1.0};
            Property<FlexableWidget, float> percentage{this, 1.0};
            Property<FlexableWidget, int> fixed{this, 0};

          public:
            FlexableWidget() noexcept;

            const ObjectType* object_type() const noexcept override;

          protected:
            Size perform_layout(Constraint constraint) noexcept override;
        };

        class ColumnGridWidget : public MultiChildWidget {
          public:
            ColumnGridWidget() noexcept;

            const ObjectType* object_type() const noexcept override;

          protected:
            Size perform_layout(Constraint constraint) noexcept override;
        };

        class RowGridWidget : public MultiChildWidget {
          public:
            RowGridWidget() noexcept;

            const ObjectType* object_type() const noexcept override;

          protected:
            Size perform_layout(Constraint constraint) noexcept override;
        };

        enum class Direction {
            row,
            column,
            row_reverse,
            column_reverse
        };

        class FlexWidget : public MultiChildWidget {
          public:
            Property<FlexWidget, float> cross_axis_alignment{this, 0.0f};
            Property<FlexWidget, Direction> direction{this, Direction::row};

          public:
            FlexWidget() noexcept;

            const ObjectType* object_type() const noexcept override;

          protected:
            Size perform_layout(Constraint constraint) noexcept override;

          private:
            void _layout_children(Constraint constraint) noexcept;
        };

    } // namespace widgets

    RolUI_decl_object_type_of(widgets::ColumnWidget);
    RolUI_decl_object_type_of(widgets::RowWidget);
    RolUI_decl_object_type_of(widgets::FlexableWidget);
    RolUI_decl_object_type_of(widgets::ColumnGridWidget);
    RolUI_decl_object_type_of(widgets::RowGridWidget);
    RolUI_decl_object_type_of(widgets::FlexWidget);

} // namespace RolUI
