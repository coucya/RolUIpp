#include <cstdint>
#include <iostream>
#include <string>
#include <filesystem>
#include <stdexcept>

#include "GLFWWindow.h"

#include "RolUI/Point.hpp"
#include "RolUI/IEvent.hpp"
#include "RolUI/Image.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/Application.hpp"
#include "RolUI/Widget_event.hpp"
#include "RolUI-widgets/Image.hpp"
#include "RolUI-events/MouseEvent.hpp"

using namespace RolUI;

std::string get_resources_dir() {
    std::filesystem::path self_path{__FILE__};
    std::filesystem::path path = self_path.parent_path() / ".." / "resources";
    path = std::filesystem::absolute(path);
    return path.string();
}

std::string get_image_huaji_path() {
    using namespace std;
    filesystem::path font_path = filesystem::path(get_resources_dir()) / "huaji.png";
    return font_path.string();
}

int main(int argc, char* argv[]) {
    using namespace RolUI;

    RolUIGLFW::GLFWWindow win(800, 600, "image");
    win.on_exit = [&] { RolUI::Application::exit(); };

    RolUI::Application::init(&win);

    std::string img_huaji_path = get_image_huaji_path();
    std::cout << "img_huaji_path: " << img_huaji_path << std::endl;

    RolUI::Image img = win.load_image(img_huaji_path.c_str());
    widgets::ImageWidget image1{img};

    Application::root_widget()->set_content_widget(&image1);

    win.run();

    return 0;
}
