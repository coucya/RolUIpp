#include <iostream>
#include <string>
#include <filesystem>
#include <stdexcept>

#include "glfw_backend/GLFWWindow.h"

#include "RolUI/Point.hpp"
#include "RolUI/IEvent.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/widgets/EllipseWidget.hpp"
#include "RolUI/widgets/Rect.hpp"
#include "RolUI/widgets/Text.hpp"
#include "RolUI/widgets/Label.hpp"
#include "RolUI/events/MouseEvent.hpp"
#include "RolUI/events/Widget_event.hpp"
#include "RolUI/Application.hpp"

using namespace RolUI;

std::string get_font_path() {
    std::filesystem::path self_path{__FILE__};
    std::filesystem::path font_path = self_path.parent_path() / ".." / "resources" / "Roboto-Regular.ttf";
    return font_path.string();
}

int main(int argc, char* argv[]) {
    using namespace RolUI;

    RolUI::Application app;
    RolUIBackend::GLFWWindow win(800, 600, "helloworld");
    win.on_exit = [&] { app.exit(); };

    app.set_window(&win);

    std::string font_path = get_font_path();
    if (win.painter()->load_font("san", "C:\\WINDOWS\\FONTS\\MSYHL.TTC") == false)
        throw std::runtime_error("can't load font.");

    widget::Label label1{"Hello World!"};

    win.set_content_widget(&label1);

    label1.set_pos(-10, -10);
    label1.set_pos_relative(RelativeTarget::parent, AnchorPoint::right_bottom, AnchorPoint::right_bottom);
    label1.set_font("san");
    label1.set_font_size(30);
    label1.adjust_size();

    win.show();

    app.run();

    return 0;
}
