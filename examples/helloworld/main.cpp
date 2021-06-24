#include <iostream>

#include "glfw_backend/GLFWWindow.h"
#include "RolUI/widgets/RectWidget.h"

int main(int argc, char* argv[]) {
    RolUIBackend::GLFWWindow win(800, 600, "helloworld");

    RolUI::RectWidget rw1, cw1, cw2;

    rw1.set_pos(10, 10)
        .set_size(500, 500)
        .set_round(10)
        .set_background_color(RolUI::Color(240, 240, 0));

    cw1.set_pos(30, 30)
        .set_size(100, 100)
        .set_round(10)
        .set_background_color(RolUI::Color(240, 0, 0));

    cw2.set_pos(150, 30)
        .set_size(100, 100)
        .set_round(10)
        .set_background_color(RolUI::Color(240, 0, 0))
        .set_border_width(10);

    rw1.add_child(&cw1);
    rw1.add_child(&cw2);

    win.set_widget(&rw1);

    win.show();

    win.run();

    return 0;
}