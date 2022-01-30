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
#include "RolUI/widgets/Image.hpp"
#include "RolUI/widgets/Margin.hpp"
#include "RolUI/widgets/Column.hpp"
#include "RolUI/widgets/layer.hpp"
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

    widget::PointerListener* pl = new widget::PointerListener();
    widget::Text* text = new widget::Text{str};
    widget::Rect* box = new widget::Rect{8};
    widget::Margin* padding = new widget::Margin{16};
    widget::Margin* margin = new widget::Margin{8};

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

int main(int argc, char* argv[]) {

    RolUIBackend::GLFWWindow win(800, 600, "text box");
    win.on_exit = [&] { RolUI::Application::exit(); };

    RolUI::Application::init(&win);

    if (win.painter()->load_font("default", "C:\\WINDOWS\\FONTS\\MSYHL.TTC") == false)
        throw std::runtime_error("can't load font.");

    widget::Deck c{};
    widget::Text text{"Text test."};
    widget::EditableText et{};
    widget::PointerListener pl{};

    pl.set_child(&et);
    et.text = "text";
    et.font_size = 32;

    pl.on_click.connect([&et](Point pos) {
        et.set_cursor_blinks(true);
        et.cursor_index = et.pos_to_index(pos);
    });

    win.show();

    Application::run(&pl);

    return 0;
}
