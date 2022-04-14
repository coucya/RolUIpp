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
#include "RolUI/Object.hpp"
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
#include "RolUI/widgets/decoration.hpp"
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
int random_int(int beg = 0, int end = 100) {
    std::uniform_int_distribution<int> rs{beg, end};
    return rs(e);
}

Widget* build_size_box(Size s, Color color) {
    return mk_widget<BoxWidget>()
        ->background_color(color)
        ->set_child(
            mk_widget<SizedWidget>()->width(s.width)->height(s.height));
}

FlexableWidget* build_flexable(FlexFit fit, Color color, float val = 0.0) {
    FlexableWidget* fw = mk_widget<FlexableWidget>()->fit(fit);
    BoxWidget* bw = mk_widget<BoxWidget>()->background_color(color);
    fw->set_child(bw);

    if (fit == FlexFit::fixed)
        fw->fixed(int(val));
    else if (fit == FlexFit::flex)
        fw->flex(val);
    else if (fit == FlexFit::percentage)
        fw->percentage(val);
    return fw;
}

int main(int argc, char* argv[]) {

    RolUIBackend::GLFWWindow win(800, 600, "text box");
    win.on_exit = [&] { RolUI::Application::exit(); };
    win.on_size_change.connect([](Size) { RolUI::Application::flush_frame(); });

    RolUI::Application::init(&win);

    if (win.painter()->load_font("default", "C:\\WINDOWS\\FONTS\\MSYHL.TTC") == false)
        throw std::runtime_error("can't load font.");

    FlexWidget flex_w;
    RichTextWidget rich_w;
    MouseListener mouse_l;
    VSeparatorWidget vsep_w;
    SizedWidget size_w;
    MarginWidget margin_w;
    StackWidget stack_w;
    RowGridWidget column_grid_w;

    column_grid_w.add_child(build_flexable(FlexFit::fixed, random_color(), 100));
    column_grid_w.add_child(build_flexable(FlexFit::fixed, random_color(), 100));
    column_grid_w.add_child(build_flexable(FlexFit::fixed, random_color(), 200));
    column_grid_w.add_child(build_flexable(FlexFit::expand, random_color()));
    column_grid_w.add_child(build_flexable(FlexFit::fixed, random_color(), 100));
    column_grid_w.add_child(build_flexable(FlexFit::flex, random_color(), 2));
    column_grid_w.add_child(build_flexable(FlexFit::flex, random_color(), 1));

    Widget* w = &column_grid_w;
    Application::run(w);

    return 0;
}
