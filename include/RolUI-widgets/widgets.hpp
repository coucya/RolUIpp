#pragma once

namespace RolUI {
    namespace widgets {

        template <typename WT, typename... Args>
        WT* mk_widget(Args&&... args) { return new WT(args...); }

    } // namespace widgets
} // namespace RolUI
