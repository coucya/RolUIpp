
#include "../sigslot/Signal.hpp"

#include "../Widget.hpp"
#include "./container.hpp"

namespace RolUI {
    namespace widget {

        class Focus : public SingleChildWidget {
          public:
            Signal<bool> on_focus;

          public:
            Focus() noexcept;

            void focus() noexcept;
            void unfocus() noexcept;

            bool handle_event(IEvent* e) override;
        };

    } // namespace widget

} // namespace RolUI