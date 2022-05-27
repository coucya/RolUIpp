#include <iostream>
#include <string>
#include <filesystem>
#include <stdexcept>

#include "RolUI/widgets/Text.hpp"
#include "glfw_backend/GLFWWindow.h"

#include "RolUI/Widget.hpp"
#include "RolUI/widgets/widgets.hpp"
#include "RolUI/widgets/container.hpp"
#include "RolUI/Application.hpp"

using namespace RolUI;

std::string get_font_path() {
    std::filesystem::path self_path{__FILE__};
    std::filesystem::path font_path = self_path.parent_path() / ".." / "resources" / "Roboto-Regular.ttf";
    return font_path.string();
}

int main(int argc, char* argv[]) {
    using namespace RolUI;

    RolUIBackend::GLFWWindow win(800, 600, "hello world");
    win.on_exit = [&] { RolUI::Application::exit(); };

    RolUI::Application::init(&win);

    std::string font_path = get_font_path();
    // if (win.painter()->load_font("default", font_path.c_str()) == false)
    //     throw std::runtime_error("can't load font.");

    Widget* label = widgets::mk_widget<widgets::TextWidget>("hello world~");
    Widget* w = widgets::mk_widget<widgets::AlignWidget>();
    w->set_child(label);

    Application::run(w);

    return 0;
}
