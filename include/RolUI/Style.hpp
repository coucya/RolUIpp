#pragma once

#include <string>
#include <optional>

#include "RolUI/Color.hpp"

namespace RolUI {

    struct Style {
        std::optional<Color> color = Color{0, 0, 0};
        std::optional<std::string> font_name = "default";
        std::optional<unsigned> font_size = 15;
        std::optional<unsigned> round = 0;
        std::optional<unsigned> border_width = 0;
        std::optional<Color> border_color = Color{0, 0, 0};
        std::optional<Color> background_color = Color{255, 255, 255, 0};
    };

} // namespace RolUI
