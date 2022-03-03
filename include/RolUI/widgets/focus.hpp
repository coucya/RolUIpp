
#include "../sigslot/Signal.hpp"

#include "../Widget.hpp"
#include "../Widget.hpp"

namespace RolUI {
    namespace widgets {

        class FocusWidget : public SingleChildWidget {
          public:
            Signal<bool> on_focus;

          public:
            FocusWidget() noexcept;

            void focus() noexcept;
            void unfocus() noexcept;

            bool handle_event(IEvent* e) noexcept override;
        };

    } // namespace widgets

} // namespace RolUI