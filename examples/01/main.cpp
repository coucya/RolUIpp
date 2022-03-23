#include <cstddef>
#include <iostream>
#include <string>
#include <filesystem>
#include <type_traits>
#include <chrono>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>

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

Widget* build_property_widget(const char* str) {
    Widget* pnw = widgets::align(1.0, 0.0, widgets::text(str));
    Widget* pew = widgets::align(-1.0, 0.0, widgets::text(str));
    Widget* w = widgets::row_grid()
                    ->add_child(pnw)
                    ->add_child(pew);
    return w;
}

Widget* build_property_table_widget() {
    std::initializer_list<const char*> ps{"aaa", "bbb", "ccc", "ddd"};
    ColumnWidget* cw = widgets::column();
    for (const char* p : ps) {
        auto w = build_property_widget(p);
        cw->add_child(w);
    }
    return cw;
}

int main(int argc, char* argv[]) {

    RolUIBackend::GLFWWindow win(800, 600, "text box");
    win.on_exit = [&] { RolUI::Application::exit(); };

    RolUI::Application::init(&win);

    if (win.painter()->load_font("default", "C:\\WINDOWS\\FONTS\\MSYHL.TTC") == false)
        throw std::runtime_error("can't load font.");

    RowWidget* cw = mk_widget<RowWidget>();
    cw->gap(10);
    for (int i = 0; i < 40; i++) {
        auto s = std::to_string(i);
        cw->add_child(text(s.c_str(), 30));
    }
    HScrollView vsv;
    vsv.scroll_step(30.0);
    vsv.set_child(cw);

    Widget* w = &vsv;
    Application::run(w);

    return 0;
}
