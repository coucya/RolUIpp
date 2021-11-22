#include <cstddef>
#include <iostream>
#include <string>
#include <filesystem>
#include <type_traits>
#include <chrono>
#include <vector>
#include <algorithm>
#include <numeric>

#include "RolUI/Window.hpp"
#include "RolUI/widgets/TextBox.hpp"
#include "glfw_backend/GLFWWindow.h"

#include "RolUI/Point.hpp"
#include "RolUI/Vector.hpp"
#include "RolUI/IEvent.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/IEventListener.hpp"
#include "RolUI/widgets/EllipseWidget.hpp"
#include "RolUI/widgets/Rect.hpp"
#include "RolUI/widgets/Text.hpp"
#include "RolUI/widgets/Label.hpp"
#include "RolUI/widgets/Button.hpp"
#include "RolUI/widgets/Scroll.hpp"
#include "RolUI/widgets/Image.hpp"
#include "RolUI/events/MouseEvent.hpp"
#include "RolUI/events/Widget_event.hpp"
#include "RolUI/events/CharEvent.hpp"
#include "RolUI/timer.hpp"
#include "RolUI/Application.hpp"

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

int main(int argc, char* argv[]) {
    using namespace RolUI;

    RolUI::Application app;
    RolUIBackend::GLFWWindow win(800, 600, "01");
    win.on_exit = [&] { app.exit(); };

    app.set_window(&win);

    if (win.painter()->load_font("default", "C:\\WINDOWS\\FONTS\\MSYHL.TTC") == false)
        throw std::runtime_error("can't load font.");

    // widget::Image image{nullptr};
    Widget widget;
    widget::Button button{"button", &widget};
    widget::TextBox textbox{&widget};

    widget.set_size_relative(RelativeTarget::parent, SizeMode::relative);

    win.set_content_widget(&widget);

    textbox.text = std::string{u8"输入框"};
    textbox.set_size(100, 30);
    textbox.border_width = 1;
    textbox.border_color = {0, 0, 0};

    textbox.set_pos_relative(RelativeTarget::parent, AnchorPoint::centre_middle, AnchorPoint::centre_middle);
    button.on_click.connect([&] { win.set_focus_widget(nullptr); });

    win.show();

    app.run();

    return 0;
}
