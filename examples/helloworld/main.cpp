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

int main(int argc, char* argv[]) {
    using namespace RolUI;

    RolUIBackend::GLFWWindow win(800, 600, "helloworld");
    win.on_exit = [&] { RolUI::Application::exit(); };

    RolUI::Application::init(&win);

    std::string font_path = get_font_path();
    if (win.painter()->load_font("default", "C:\\WINDOWS\\FONTS\\MSYHL.TTC") == false)
        throw std::runtime_error("can't load font.");

    widget::Text label1{"Hello World!"};
    widget::Rect box{16};
    widget::Margin m{32};
    widget::Align align{};

    label1.font_size = 30;
    box.background_color = {255, 0, 0};

    m.add_child(&label1);
    box.add_child(&m);
    align.add_child(&box);

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
