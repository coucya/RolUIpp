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
#include "RolUI/events/MouseEvent.hpp"
#include "RolUI/events/Widget_event.hpp"
#include "RolUI/events/CharEvent.hpp"
#include "RolUI/timer.hpp"
#include "RolUI/Application.hpp"

#include "RolUI/widgets/Text.hpp"
#include "RolUI/widgets/Image.hpp"
#include "RolUI/widgets/layer.hpp"
#include "RolUI/widgets/flow.hpp"
#include "RolUI/widgets/container.hpp"
#include "RolUI/widgets/PointerListener.hpp"

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

using namespace RolUI;

template <typename PF, typename HF>
Widget* make_button(std::string str, PF&& pf, HF&& hf) {
    Color fg = {55, 55, 255};
    Color bg = {255, 155, 155};

    widgets::PointerListener* pl = new widgets::PointerListener();
    widgets::Text* text = new widgets::Text{str};
    widgets::Box* box = new widgets::Box{8};
    widgets::Margin* padding = new widgets::Margin{16};
    widgets::Margin* margin = new widgets::Margin{8};

    text->font_size = 50;
    text->font_color = fg;
    box->background_color = bg;
    pl->on_down.connect(std::forward<PF>(pf));
    pl->on_hover.connect(std::forward<HF>(hf));

    padding->set_child(text);
    // box->set_child(padding);
    pl->set_child(padding);
    // margin->add_child(pl);

    return pl;
}

Widget* make_box(Color c) {
    widgets::Box* box = new widgets::Box();
    widgets::SizedBox* sb = new widgets::SizedBox();
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

    widgets::Align center;
    widgets::SizedBox sb;
    widgets::ColumnGrid rg;

    rg.add_child(make_box({255, 0, 0}));
    rg.add_child(make_box({0, 255, 0}), 2);
    rg.add_child(make_box({0, 0, 255}));
    rg.add_child(make_box({255, 255, 0}), 4);

    sb.width = widgets::SizeUnit::percentage(0.9f);
    sb.height = widgets::SizeUnit::percentage(0.9f);

    sb.set_child(&rg);
    center.set_child(&sb);

    win.show();

    Application::run(&center);

    return 0;
}
