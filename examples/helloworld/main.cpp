#include <iostream>
#include <string>
#include <filesystem>
#include <stdexcept>

#include "GLFWWindow.h"

#include "RolUI/Widget.hpp"
#include "RolUI/Application.hpp"
#include "RolUI-widgets/widgets.hpp"
#include "RolUI-widgets/container.hpp"
#include "RolUI-widgets/Text.hpp"

using namespace RolUI;

std::string get_font_path() {
    std::filesystem::path self_path{__FILE__};
    std::filesystem::path font_path = self_path.parent_path() / ".." / "resources" / "Roboto-Regular.ttf";
    return font_path.string();
}

int main(int argc, char* argv[]) {
    using namespace RolUI;

    RolUIGLFW::GLFWWindow win(800, 600, "hello world");
    win.on_exit = [&] { RolUI::Application::exit(); };

    RolUI::Application::init(&win);

    std::string font_path = get_font_path();
    int msyhl_handle = win.load_font("C:\\WINDOWS\\FONTS\\MSYHL.TTC");
    // int msyhl_handle = win.load_font(font_path.c_str());

    std::cout << "msyhl_handle: " << msyhl_handle << std::endl;

    Widget* label = widgets::mk_widget<widgets::TextWidget>("hello world~")->font(msyhl_handle);
    Widget* w = widgets::mk_widget<widgets::AlignWidget>();
    w->set_child(label);

    Application::root_widget()->set_content_widget(w);

    win.run();

    return 0;
}
