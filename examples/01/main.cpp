#include <cstddef>
#include <iostream>
#include <string>
#include <filesystem>
#include <type_traits>
#include <chrono>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>

#include "glfw_backend/GLFWWindow.h"

#include "RolUI/Point.hpp"
#include "RolUI/Vector.hpp"
#include "RolUI/IEvent.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/events/MouseEvent.hpp"
#include "RolUI/events/Widget_event.hpp"
#include "RolUI/events/CharEvent.hpp"
#include "RolUI/Application.hpp"

#include "RolUI/widgets/widgets.hpp"
#include "RolUI/widgets/Text.hpp"
#include "RolUI/widgets/Image.hpp"
#include "RolUI/widgets/layer.hpp"
#include "RolUI/widgets/flow.hpp"
#include "RolUI/widgets/container.hpp"
#include "RolUI/widgets/listener.hpp"

using namespace RolUI;
using namespace RolUI::widgets;

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

using namespace RolUI;

Widget* make_box(Color c) {
    widgets::BoxWidget* box = new widgets::BoxWidget();
    widgets::SizedBoxWidget* sb = new widgets::SizedBoxWidget();
    box->set_child(sb);
    box->background_color = c;
    return box;
}

int main(int argc, char* argv[]) {

    RolUIBackend::GLFWWindow win(800, 600, "text box");
    win.on_exit = [&] { RolUI::Application::exit(); };

    RolUI::Application::init(&win);

    if (win.painter()->load_font("default", "C:\\WINDOWS\\FONTS\\MSYHL.TTC") == false)
        throw std::runtime_error("can't load font.");

    // button("bt", std::function<void()>{[]() -> void {}});
    // widgets::EditableTextWidget et;
    TextBoxWidget* et = mk_widget<TextBoxWidget>();
    et->font_size(40);
    et->text(u8"啊啊啊啊啊啊");

    // PointerListenerWidget* plw = widgets::pointer_listener(et);
    // plw->on_click.connect([=](Point pos) {
    //     std::cout << "x: " << pos.x << " y: " << pos.y << std::endl;
    //     int idx = et->pos_to_index(pos - et->abs_pos());
    //     et->cursor_index(idx);
    //     et->set_blink(true);
    // });

    Widget* w = widgets::align(0.0, 0.0, et);
    // Widget* w = widgets::sized(200, 200, widgets::button("bt", []() { std::cout << "bt click." << std::endl; }));
    Application::run(w);

    return 0;
}
