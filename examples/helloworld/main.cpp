#include <iostream>
#include <string>
#include <filesystem>
#include <stdexcept>

#include "glfw_backend/GLFWWindow.h"

#include "RolUI/Point.hpp"
#include "RolUI/IEvent.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/widgets/Rect.hpp"
#include "RolUI/widgets/Text.hpp"
#include "RolUI/widgets/Margin.hpp"
#include "RolUI/widgets/Align.hpp"
#include "RolUI/widgets/Column.hpp"
#include "RolUI/widgets/Row.hpp"
#include "RolUI/events/MouseEvent.hpp"
#include "RolUI/events/Widget_event.hpp"
#include "RolUI/Application.hpp"
#include "RolUI/timer.hpp"

using namespace RolUI;

std::string get_font_path() {
    std::filesystem::path self_path{__FILE__};
    std::filesystem::path font_path = self_path.parent_path() / ".." / "resources" / "Roboto-Regular.ttf";
    return font_path.string();
}

Widget* make_label(std::string str, Color fg = Color(255, 255, 255), Color bg = Color(255, 100, 100)) {
    widget::Text* text = new widget::Text{str};
    widget::Rect* box = new widget::Rect{8};
    widget::Margin* padding = new widget::Margin{16};
    widget::Margin* margin = new widget::Margin{8};

    text->font_size = 20;
    text->font_color = fg;
    box->background_color = bg;

    padding->add_child(text);
    box->add_child(padding);
    margin->add_child(box);

    return margin;
}

int main(int argc, char* argv[]) {
    using namespace RolUI;

    RolUIBackend::GLFWWindow win(800, 600, "helloworld");
    win.on_exit = [&] { RolUI::Application::exit(); };

    RolUI::Application::init(&win);

    std::string font_path = get_font_path();
    if (win.painter()->load_font("default", "C:\\WINDOWS\\FONTS\\MSYHL.TTC") == false)
        throw std::runtime_error("can't load font.");

    widget::Align align{};
    widget::Column column{};

    column.cross_axis_alignment = widget::Column::right;

    align.add_child(&column);

    for (int i = 0; i < 5; i++) {
        Widget* widget = make_label(std::string("label") + std::to_string(i % 2 ? i * 100 : i));
        column.add_child(widget);
    }

    win.set_content_widget(&align);

    win.show();

    // RolUI::Timer t;
    // t.on_timeout.connect([&](double timeout) {
    //     float ax = align.align_x.get();
    //     ax += 0.01;
    //     if (ax > 1.0f)
    //         ax = 0.0f;
    //     align.align_x = ax;
    // });
    // t.start(0.03, false);

    Application::run();

    return 0;
}
