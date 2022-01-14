#pragma once

#include "../Widget.hpp"
#include "../Property.hpp"

namespace RolUI {
    namespace widget {

        class Column : public Widget {
          public:
            Column() noexcept;

          protected:
            Size perlayout(Constraint constraint) override;
        };

    } // namespace widget
} // namespace RolUI