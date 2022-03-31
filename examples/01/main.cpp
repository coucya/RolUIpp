#include <cstddef>
#include <iostream>
#include <string>
#include <string_view>
#include <filesystem>
#include <type_traits>
#include <chrono>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>
#include <random>

#include "glfw_backend/GLFWWindow.h"

#include "RolUI/Point.hpp"
#include "RolUI/Vector.hpp"
#include "RolUI/IEvent.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/events/MouseEvent.hpp"
#include "RolUI/events/Widget_event.hpp"
#include "RolUI/events/CharEvent.hpp"
#include "RolUI/Application.hpp"

#include "RolUI/widgets/widgets.hpp"
#include "RolUI/widgets/Text.hpp"
#include "RolUI/widgets/Image.hpp"
#include "RolUI/widgets/Scroll.hpp"
#include "RolUI/widgets/layer.hpp"
#include "RolUI/widgets/flow.hpp"
#include "RolUI/widgets/container.hpp"
#include "RolUI/widgets/listener.hpp"

using namespace RolUI;
using namespace RolUI::widgets;

std::default_random_engine e;

Color random_color() {
    std::uniform_int_distribution<uint8_t> rc{0, 255};
    return Color{rc(e), rc(e), rc(e)};
}
Size random_size(int beg = 0, int end = 100) {
    std::uniform_int_distribution<int> rs{beg, end};
    return Size(rs(e), rs(e));
}

Widget* build_size_box(Size s, Color color) {
    return mk_widget<BoxWidget>()
        ->background_color(color)
        ->set_child(
            mk_widget<SizedBoxWidget>()->width(s.width)->height(s.height));
}

int main(int argc, char* argv[]) {

    RolUIBackend::GLFWWindow win(800, 600, "text box");
    win.on_exit = [&] { RolUI::Application::exit(); };
    win.on_size_change.connect([](Size) { RolUI::Application::flush_frame(); });

    RolUI::Application::init(&win);

    if (win.painter()->load_font("default", "C:\\WINDOWS\\FONTS\\MSYHL.TTC") == false)
        throw std::runtime_error("can't load font.");

    FlexWidget flex;

    for (int i = 0; i < 10; i++) {
        Color c = random_color();
        Size s = random_size(30, 100);
        std::cout << "size: (" << s.width << ", " << s.height << "), ";
        std::cout << "color: (" << (int)c.r << ", " << (int)c.g << ", " << (int)c.b << ")";
        std::cout << std::endl;
        Widget* w = build_size_box(s, c);
        flex.add_child(w);
    }

    float dir = 0.05;
    Application::set_interval(0.03, [&](double) {
        float caa = flex.cross_axis_alignment();
        if (caa >= 1.0 || caa <= -1.0)
            dir = -dir;
        flex.cross_axis_alignment(caa + dir);
    });
    Application::set_interval(3.0, [&](double) {
        Direction flex_dir = flex.direction();
        switch (flex.direction()) {
            case Direction::row: flex.direction(Direction::row_reverse); break;
            case Direction::row_reverse: flex.direction(Direction::column); break;
            case Direction::column: flex.direction(Direction::column_reverse); break;
            case Direction::column_reverse: flex.direction(Direction::row); break;
        }
    });

    Widget* w = &flex;
    Application::run(w);

    return 0;
}
