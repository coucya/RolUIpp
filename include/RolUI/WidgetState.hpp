#pragma once

#include <cstdint>
#include <type_traits>

namespace RolUI {

    typedef uint8_t WidgetState;

    constexpr WidgetState WIDGET_STATE_DEFAULT = 1;
    constexpr WidgetState WIDGET_STATE_DISABLED = 2;
    constexpr WidgetState WIDGET_STATE_FOCUS = 4;
    constexpr WidgetState WIDGET_STATE_HOVER = 8;
    constexpr WidgetState WIDGET_STATE_PRESS = 16;
    constexpr WidgetState WIDGET_STATE_USER_STATE_1 = 32;
    constexpr WidgetState WIDGET_STATE_USER_STATE_2 = 64;
    constexpr WidgetState WIDGET_STATE_USER_STATE_3 = 128;

    constexpr unsigned WIDGET_STATE_COUNT = 8;
    constexpr unsigned WIDGET_STATE_USER_STATE_COUNT = 3;

    inline unsigned widget_state_to_index(WidgetState s) {
        switch (s) {
            case WIDGET_STATE_DEFAULT: return 0;
            case WIDGET_STATE_DISABLED: return 1;
            case WIDGET_STATE_FOCUS: return 2;
            case WIDGET_STATE_HOVER: return 3;
            case WIDGET_STATE_PRESS: return 4;
            case WIDGET_STATE_USER_STATE_1: return 5;
            case WIDGET_STATE_USER_STATE_2: return 6;
            case WIDGET_STATE_USER_STATE_3: return 7;
            default: return 0;
        }
    }
    inline WidgetState index_to_widget_state(unsigned idx) {
        switch (idx) {
            case 0: return WIDGET_STATE_DEFAULT;
            case 1: return WIDGET_STATE_DISABLED;
            case 2: return WIDGET_STATE_FOCUS;
            case 3: return WIDGET_STATE_HOVER;
            case 4: return WIDGET_STATE_PRESS;
            case 5: return WIDGET_STATE_USER_STATE_1;
            case 6: return WIDGET_STATE_USER_STATE_2;
            case 7: return WIDGET_STATE_USER_STATE_3;
            default: return WIDGET_STATE_DEFAULT;
        }
    }

    inline bool widget_state_has_state(WidgetState state, WidgetState target) {
        return state & target;
    }
    inline WidgetState widget_state_set_state(WidgetState state, WidgetState target) {
        return state | target;
    }
    inline WidgetState widget_state_clear_state(WidgetState state, WidgetState target) {
        return state & !target;
    }

    template <typename F, typename = std::enable_if_t<std::is_invocable_v<F, WidgetState, unsigned>>>
    void widget_state_foreach(WidgetState state, F&& f) {
        for (int i = 0; i < WIDGET_STATE_COUNT; i++) {
            WidgetState ts = (1 << i);
            if (widget_state_has_state(state, ts))
                f(ts, i);
        }
    }

} // namespace RolUI
