#pragma once

#include <functional>

#include "../Image.hpp"

#include "../Widget.hpp"
#include "./flow.hpp"
#include "./layer.hpp"
#include "./container.hpp"
#include "./listener.hpp"
#include "./Text.hpp"
#include "./Image.hpp"
#include "./Scroll.hpp"

namespace RolUI {
    namespace widgets {

        template <typename WT, typename... Args>
        WT* mk_widget(Args&&... args) { return new WT(args...); }

    } // namespace widgets
} // namespace RolUI
