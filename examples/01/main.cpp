#include <cstddef>
#include <iostream>
#include <string>
#include <filesystem>
#include <type_traits>
#include <chrono>
#include <vector>
#include <algorithm>
#include <numeric>

#include "glfw_backend/GLFWWindow.h"

#include "RolUI/Point.hpp"
#include "RolUI/Vector.hpp"
#include "RolUI/IEvent.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/IEventListener.hpp"
#include "RolUI/events/MouseEvent.hpp"
#include "RolUI/events/Widget_event.hpp"
#include "RolUI/events/CharEvent.hpp"
#include "RolUI/timer.hpp"
#include "RolUI/Application.hpp"

#include "RolUI/widgets/Rect.hpp"
#include "RolUI/widgets/Text.hpp"
#include "RolUI/widgets/Label.hpp"
#include "RolUI/widgets/Button.hpp"
#include "RolUI/widgets/Scroll.hpp"
#include "RolUI/widgets/Image.hpp"
#include "RolUI/widgets/TextBox.hpp"

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

    RolUIBackend::GLFWWindow win(800, 600, "01");
    win.on_exit = [&] { RolUI::Application::exit(); };

    RolUI::Application::init(&win);

    if (win.painter()->load_font("default", "C:\\WINDOWS\\FONTS\\MSYHL.TTC") == false)
        throw std::runtime_error("can't load font.");

    // widget::Image image{nullptr};
    Widget widget;
    widget::Button blur{&widget, "blur"};
    widget::Button prev{&widget, "prev"};
    widget::Button next{&widget, "next"};
    widget::Button del_prev{&widget, "del prev"};
    widget::Button del_next{&widget, "del next"};

    widget::TextBox textbox{&widget};

    win.set_content_widget(&widget);

    widget.set_size_relative(RelativeTarget::parent, SizeMode::relative);

    // blur.set_pos_relative(RelativeTarget::parent, AnchorPoint::left_bottom);
    prev.set_pos_relative(RelativeTarget::prev, AnchorPoint::left_bottom);
    next.set_pos_relative(RelativeTarget::prev, AnchorPoint::left_bottom);
    del_prev.set_pos_relative(RelativeTarget::prev, AnchorPoint::left_bottom);
    del_next.set_pos_relative(RelativeTarget::prev, AnchorPoint::left_bottom);

    textbox.text = std::string{u8"输入框测试"};
    textbox.adjust_size();

    textbox.set_pos_relative(RelativeTarget::parent, AnchorPoint::centre_middle, AnchorPoint::centre_middle);

    blur.on_click.connect([&] { win.set_focus_widget(nullptr); });
    prev.on_click.connect([&] {
        textbox.cursor_to_prev_char();
    });
    next.on_click.connect([&] {
        textbox.cursor_to_next_char();
    });
    del_prev.on_click.connect([&] {
        textbox.delete_cursor_prev();
    });
    del_next.on_click.connect([&] {
        textbox.delete_cursor_next();
    });

    win.show();

    Application::run();

    return 0;
}
