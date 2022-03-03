#include <cstdint>
#include <iostream>
#include <string>
#include <filesystem>
#include <stdexcept>

#include "glfw_backend/GLFWWindow.h"

#include "RolUI/Point.hpp"
#include "RolUI/IEvent.hpp"
#include "RolUI/Image.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/widgets/Image.hpp"
#include "RolUI/events/MouseEvent.hpp"
#include "RolUI/events/Widget_event.hpp"
#include "RolUI/Application.hpp"

#include "stb_image.h"

using namespace RolUI;

std::string get_resources_dir() {
    std::filesystem::path self_path{__FILE__};
    std::filesystem::path path = self_path.parent_path() / ".." / "resources";
    return path.string();
}

std::string get_font_path() {
    using namespace std;
    filesystem::path font_path = filesystem::path(get_resources_dir()) / "Roboto-Regular.ttf";
    return font_path.string();
}
std::string get_image_huaji_path() {
    using namespace std;
    filesystem::path font_path = filesystem::path(get_resources_dir()) / "huaji.png";
    return font_path.string();
}

Image load_image(std::string filename) {
    int image_w, image_h, image_c;
    uint8_t* image_data = stbi_load(filename.c_str(), &image_w, &image_h, &image_c, 4);

    RolUI::Image img = RolUI::Image::create_rgba_mem(image_data, image_w, image_h);

    stbi_image_free(image_data);

    return img;
}

int main(int argc, char* argv[]) {
    using namespace RolUI;

    RolUIBackend::GLFWWindow win(800, 600, "image");
    win.on_exit = [&] { RolUI::Application::exit(); };

    RolUI::Application::init(&win);

    std::string img_huaji_path = get_image_huaji_path();
    std::string font_path = get_font_path();
    if (win.painter()->load_font("default", "C:\\WINDOWS\\FONTS\\MSYHL.TTC") == false)
        throw std::runtime_error("can't load font.");

    RolUI::Image img = load_image(img_huaji_path);
    widgets::ImageWidget image1{img};

    win.show();

    Application::run(&image1);

    return 0;
}
