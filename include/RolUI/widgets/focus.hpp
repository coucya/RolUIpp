
#include "../sigslot/Signal.hpp"

#include "../Widget.hpp"
#include "../Widget.hpp"

namespace RolUI {
    namespace widgets {

        class Focus : public SingleChildWidget {
          public:
            Signal<bool> on_focus;

          public:
            Focus() noexcept;

            void focus() noexcept;
            void unfocus() noexcept;

            bool handle_event(IEvent* e) noexcept override;
        };

    } // namespace widgets

} // namespace RolUI