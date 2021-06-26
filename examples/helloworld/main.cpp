#include <iostream>
#include <string>
#include <filesystem>
#include <stdexcept>

#include "glfw_backend/GLFWWindow.h"
#include "RolUI/widgets/RectWidget.h"
#include "RolUI/widgets/EllipseWidget.h"
#include "RolUI/widgets/LabelWidget.h"

std::string get_font_path() {
    std::filesystem::path self_path{__FILE__};

    std::filesystem::path font_path = self_path.parent_path() / "Roboto-Regular.ttf";
    return font_path.string();
}

int main(int argc, char* argv[]) {

    RolUIBackend::GLFWWindow win(800, 600, "helloworld");

    std::string font_path = get_font_path();
    if (win.painter()->load_font("san", font_path.c_str()) == false)
        throw std::runtime_error("can't load font.");

    RolUI::RectWidget rw1, cw1, cw2;
    RolUI::EllipseWidget cw3, cw4;
    RolUI::LabelWidget cw5;

    rw1.set_pos(10, 10);
    rw1.set_size(500, 500);
    rw1.set_round(10);
    rw1.set_background_color(RolUI::Color(240, 240, 0));

    cw1.set_pos(10, 10);
    cw1.set_size(100, 100);
    cw1.set_round(10);
    cw1.set_background_color(RolUI::Color(240, 0, 0));

    cw2.set_pos(150, 10);
    cw2.set_size(100, 100);
    cw2.set_round(10);
    cw2.set_background_color(RolUI::Color(240, 0, 0));
    cw2.set_border_width(10);

    cw3.set_pos(10, 150);
    cw3.set_size(100, 200);
    cw3.set_background_color(RolUI::Color(240, 0, 0));

    cw4.set_pos(150, 150);
    cw4.set_size(100, 200);
    cw4.set_background_color(RolUI::Color(240, 0, 0));
    cw4.set_border_width(10);

    cw5.set_pos(270, 10);
    cw5.set_font_size(16);
    cw5.set_font("san");
    cw5.set_text("label widget.");

    rw1.add_child(&cw1);
    rw1.add_child(&cw2);
    rw1.add_child(&cw3);
    rw1.add_child(&cw4);
    rw1.add_child(&cw5);

    win.set_widget(&rw1);

    win.show();

    win.run();

    return 0;
}