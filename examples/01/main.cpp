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

#include "GLFWWindow.h"

#include "RolUI/Point.hpp"
#include "RolUI/Vector.hpp"
#include "RolUI/IEvent.hpp"
#include "RolUI/Object.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/Application.hpp"
#include "RolUI/Widget_event.hpp"
#include "RolUI-events/MouseEvent.hpp"
#include "RolUI-events/CharEvent.hpp"

#include "RolUI-widgets/widgets.hpp"
#include "RolUI-widgets/Text.hpp"
#include "RolUI-widgets/Image.hpp"
#include "RolUI-widgets/Scroll.hpp"
#include "RolUI-widgets/layer.hpp"
#include "RolUI-widgets/flow.hpp"
#include "RolUI-widgets/container.hpp"
#include "RolUI-widgets/decoration.hpp"
#include "RolUI-widgets/listener.hpp"

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

int main(int argc, char* argv[]) {
    RolUIGLFW::GLFWWindow win(800, 600, "text box");
    win.on_exit = [&] { RolUI::Application::exit(); };
    // win.on_size_change.connect([](Size) { RolUI::Application::flush_frame(); });

    RolUI::Application::init(&win);

    int msyhl_handle = win.load_font("C:\\WINDOWS\\FONTS\\MSYHL.TTC");

    // Widget* label_widget = text("label", 20);
    // Widget* buton_widget = label_button("button", 20, 10, 5);
    // MultiChildWidget* column_widget = (MultiChildWidget*)column();

    // column_widget->add_child(label_widget);
    // column_widget->add_child(buton_widget);

    // Widget* w = align(0, 0, column_widget);

    RowGridWidget* row_grid = mk_widget<RowGridWidget>();
    FlexableWidget* flexable_w = mk_widget<FlexableWidget>();
    ColumnWidget* cloumn_w = mk_widget<ColumnWidget>();
    BoxWidget* box1_w = mk_widget<BoxWidget>()->background_color({255, 0, 0});
    BoxWidget* box2_w = mk_widget<BoxWidget>()->background_color({0, 255, 0});

    for (int i = 0; i < 10; i++) {
        cloumn_w->add_child(mk_widget<TextWidget>("aaa")->text_size(20)->font(msyhl_handle));
    }

    flexable_w->fit(FlexFit::fixed)->fixed(300);
    flexable_w->set_child(cloumn_w);

    row_grid->add_child(flexable_w);
    row_grid->add_child(box2_w);
    Widget* w = row_grid;

    RolUI::Application::root_widget()->set_content_widget(w);

    win.run();

    return 0;
}
