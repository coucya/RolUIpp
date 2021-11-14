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
#include "RolUI/widgets/EllipseWidget.hpp"
#include "RolUI/widgets/Rect.hpp"
#include "RolUI/widgets/Text.hpp"
#include "RolUI/widgets/Label.hpp"
#include "RolUI/widgets/Button.hpp"
#include "RolUI/widgets/Scroll.hpp"
#include "RolUI/events/MouseEvent.hpp"
#include "RolUI/events/Widget_event.hpp"
#include "RolUI/timer.hpp"
#include "RolUI/Application.hpp"

using namespace RolUI;

std::string get_font_path() {
    std::filesystem::path self_path{__FILE__};
    std::filesystem::path font_path = self_path.parent_path() / ".." / "resources" / "Roboto-Regular.ttf";
    return font_path.string();
}

std::vector<double> ts;
void timeout_cb(double interval, void* arg) {
    Application* app = (Application*)arg;
    size_t handle = app->set_timeout(timeout_cb, 1.0, app);

    using namespace std::chrono;
    double current_time = duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count() / 1000000.0;

    ts.push_back(interval);

    double avg = std::reduce(ts.begin(), ts.end(), 0.0) / ts.size();

    printf("timeout at: %.6f, diff: %.6f, avg: %.6f \n", current_time, interval, avg);

    if (ts.size() > 5) app->remove_timeout(handle);
}

int main(int argc, char* argv[]) {
    using namespace RolUI;

    RolUI::Application app;
    RolUIBackend::GLFWWindow win(800, 600, "01");
    win.on_exit = [&] { app.exit(); };

    app.set_window(&win);

    std::string font_path = get_font_path();
    // if (win.painter()->load_font("san", font_path.c_str()) == false)
    if (win.painter()->load_font("default", "C:\\WINDOWS\\FONTS\\MSYHL.TTC") == false)
        throw std::runtime_error("can't load font.");

    widget::Scroll scroll{};
    widget::Button button{"button"};
    // widget::Rect button{};
    // widget::Label button{"label"};

    win.set_content_widget(&scroll);
    // win.set_content_widget(&button);

    button.set_size(300, 300);
    button.set_border_color({0, 0, 0});
    button.set_border_width(1);
    button.set_background_color({250, 240, 240});

    scroll.set_size(200, 200);
    scroll.set_pos_relative(RelativeTarget::parent, AnchorPoint::centre_middle, AnchorPoint::centre_middle);
    scroll.set_widget(&button);

    // button.adjust_size();

    bool is_press = false;
    button.add_listener(MousePressEvent_type(), [&](IEvent* e) {
        is_press = true;
        printf("button press. \n");
        return true;
    });
    button.add_listener(MouseReleaseEvent_type(), [&](IEvent* e) {
        is_press = false;
        printf("button release. \n");
        return true;
    });
    button.add_listener(MouseLeaveEvent_type(), [&](IEvent* e) {
        is_press = false;
        printf("button leave. \n");
        return true;
    });
    button.add_listener(MousePosEvent_type(), [&](IEvent* e) {
        if (!is_press) return false;
        MouseEvent* me = (MouseEvent*)e;
        Vector offset = me->offset();
        scroll.scroll_by_px(offset.x, offset.y);
        printf("button move: %d, %d. \n", offset.x, offset.y);
        return true;
    });

    win.show();

    app.run();

    return 0;
}
