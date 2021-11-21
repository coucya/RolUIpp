#pragma once

#include <string>
#include <optional>

#include "RolUI/Color.hpp"
#include "RolUI/Vector.hpp"
#include "RolUI/WidgetState.hpp"

namespace RolUI {

    struct Style {
        std::optional<Color> color;

        std::optional<std::string> font_name;
        std::optional<unsigned> font_size;

        std::optional<unsigned> round;

        std::optional<unsigned> border_width;
        std::optional<Color> border_color;

        std::optional<Color> background_color;

        std::optional<Vector> padding;
    };

    Style merge_style(const Style& a, const Style& b);

    class StyleSheet {
      public:
        template <typename F, typename = std::enable_if_t<std::is_invocable_v<F, WidgetState, const Style&>>>
        void foreach_style(WidgetState state, F&& f) const noexcept;

        void set_style(const Style& s, WidgetState state = WIDGET_STATE_DEFAULT) noexcept;

        const Style& get_style(WidgetState state = WIDGET_STATE_DEFAULT) const noexcept;
        Style& get_style(WidgetState state = WIDGET_STATE_DEFAULT) noexcept;

        Style merge_with(WidgetState state) const noexcept;

      private:
        Style _styles[WIDGET_STATE_COUNT];
    };

    template <typename F, typename>
    void StyleSheet::foreach_style(WidgetState state, F&& f) const noexcept {
        for (int i = 0; i < WIDGET_STATE_COUNT; i++) {
            WidgetState ts = (1 << i);
            if (widget_state_has_state(state, ts))
                f(ts, _styles[i]);
        }
    }

} // namespace RolUI
