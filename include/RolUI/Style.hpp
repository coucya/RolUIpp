#pragma once

#include <string>
#include <optional>

#include "RolUI/Color.hpp"

namespace RolUI {

    struct Style {
        std::optional<Color> color;
        std::optional<std::string> font_name;
        std::optional<unsigned> font_size;
        std::optional<unsigned> round;
        std::optional<unsigned> border_width;
        std::optional<Color> border_color;
        std::optional<Color> background_color;
    };

} // namespace RolUI
