#include <cstddef>
#include <iostream>
#include <string>
#include <string_view>
#include <filesystem>
#include <type_traits>
#include <chrono>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>
#include <random>

#include "glfw_backend/GLFWWindow.h"

#include "RolUI/Point.hpp"
#include "RolUI/Vector.hpp"
#include "RolUI/IEvent.hpp"
#include "RolUI/Object.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/events/MouseEvent.hpp"
#include "RolUI/events/Widget_event.hpp"
#include "RolUI/events/CharEvent.hpp"
#include "RolUI/Application.hpp"

#include "RolUI/widgets/widgets.hpp"
#include "RolUI/widgets/Text.hpp"
#include "RolUI/widgets/Image.hpp"
#include "RolUI/widgets/Scroll.hpp"
#include "RolUI/widgets/layer.hpp"
#include "RolUI/widgets/flow.hpp"
#include "RolUI/widgets/container.hpp"
#include "RolUI/widgets/decoration.hpp"
#include "RolUI/widgets/listener.hpp"

using namespace RolUI;
using namespace RolUI::widgets;

std::default_random_engine e;

Color random_color() {
    std::uniform_int_distribution<uint8_t> rc{0, 255};
    return Color{rc(e), rc(e), rc(e)};
}
Size random_size(int beg = 0, int end = 100) {
    std::uniform_int_distribution<int> rs{beg, end};
    return Size(rs(e), rs(e));
}
int random_int(int beg = 0, int end = 100) {
    std::uniform_int_distribution<int> rs{beg, end};
    return rs(e);
}

Widget* build_size_box(Size s, Color color) {
    return mk_widget<BoxWidget>()
        ->background_color(color)
        ->set_child(
            mk_widget<SizedWidget>()->width(s.width)->height(s.height));
}

int main(int argc, char* argv[]) {

    RolUIBackend::GLFWWindow win(800, 600, "text box");
    win.on_exit = [&] { RolUI::Application::exit(); };
    win.on_size_change.connect([](Size) { RolUI::Application::flush_frame(); });

    RolUI::Application::init(&win);

    if (win.painter()->load_font("default", "C:\\WINDOWS\\FONTS\\MSYHL.TTC") == false)
        throw std::runtime_error("can't load font.");

    FlexWidget flex_w;
    RichTextWidget rich_w;
    MouseListener mouse_l;
    VSeparatorWidget vsep_w;
    SizedWidget size_w;
    MarginWidget margin_w;
    StackWidget stack_w;

    for (int i = 0; i < 10; i++) {
        RichTextLineWidget* rtl_w = mk_widget<RichTextLineWidget>();
        for (int j = 0; j < 10; j++) {
            TextSpanWidget* ts_w = mk_widget<TextSpanWidget>();
            ts_w->text("Text");
            ts_w->font_color(random_color());
            ts_w->font_size((unsigned)random_int(20, 40));
            rtl_w->add_child(ts_w);
        }
        rich_w.add_child(rtl_w);
    }

    vsep_w.color(Color{255, 0, 0});
    vsep_w.width(2);
    size_w.height(30);
    size_w.set_child(&vsep_w);
    margin_w.top(0)->left(0);
    margin_w.set_child(&size_w);

    stack_w.align_x(-1)->align_y(-1);
    stack_w.add_child(&rich_w)->add_child(&margin_w);

    mouse_l.set_child(&stack_w);

    mouse_l.on_down.connect([&](MouseKey k, Point mouse_pos) {
        Point pos = mouse_pos - rich_w.abs_pos();
        unsigned idx = rich_w.pos_to_index(pos);
        Point cursor_pos = rich_w.index_to_pos(idx) + rich_w.pos();
        margin_w.top(cursor_pos.y)->left(cursor_pos.x);
        std::cout << "mouse pos: (" << mouse_pos.x << ", " << mouse_pos.y << "),";
        std::cout << "cursor pos: (" << cursor_pos.x << ", " << cursor_pos.y << "),";
        std::cout << " idx: " << idx << std::endl;
    });

    Widget* w = &mouse_l;
    Application::run(w);

    return 0;
}
