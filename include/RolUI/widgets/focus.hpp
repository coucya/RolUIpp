
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
        };

    } // namespace widget

    class FocusManager {
      public:
        static bool has_focus_widget() noexcept;
        static widget::Focus* focus_widget() noexcept;
        static void set_focus_widget(widget::Focus* widget) noexcept;

      private:
        static widget::Focus* _focus_widget;
    };

} // namespace RolUI