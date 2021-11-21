
#include <optional>

#include "RolUI/Style.hpp"

namespace RolUI {

    template <typename T>
    std::optional<T> merge_option(const std::optional<T>& a, const std::optional<T>& b) {
        if (a.has_value())
            return a.value();
        else if (b.has_value())
            return b.value();
        else
            return {};
    }

    Style merge_style(const Style& a, const Style& b) {
        Style res;
#define merge(attr) res.attr = merge_option(a.attr, b.attr)
        merge(color);
        merge(font_name);
        merge(font_size);
        merge(round);
        merge(border_color);
        merge(border_color);
        merge(background_color);
        merge(padding);
#undef merge
        return res;
    }

    void StyleSheet::set_style(const Style& s, WidgetState state) noexcept {
        widget_state_foreach(state, [&, this](WidgetState, unsigned i) {
            this->_styles[i] = s;
        });
    }
    const Style& StyleSheet::get_style(WidgetState state) const noexcept {
        int idx = widget_state_to_index(state);
        return _styles[idx];
    }
    Style& StyleSheet::get_style(WidgetState state) noexcept {
        int idx = widget_state_to_index(state);
        return _styles[idx];
    }

    Style StyleSheet::merge_with(WidgetState state) const noexcept {
        Style res;
        foreach_style(state, [&res](WidgetState, const Style& s) {
            res = merge_style(s, res);
        });
        return res;
    }

} // namespace RolUI
