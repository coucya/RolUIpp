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
#include "RolUI/IEvent.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/widgets/EllipseWidget.hpp"
#include "RolUI/widgets/Rect.hpp"
#include "RolUI/widgets/Text.hpp"
#include "RolUI/widgets/Label.hpp"
#include "RolUI/widgets/Button.hpp"
#include "RolUI/events/MouseEvent.hpp"
#include "RolUI/events/Widget_event.hpp"
#include "RolUI/Application.hpp"

using namespace RolUI;

std::string get_font_path() {
    std::filesystem::path self_path{__FILE__};
    std::filesystem::path font_path = self_path.parent_path() / ".." / "resources" / "Roboto-Regular.ttf";
    return font_path.string();
}

std::vector<double> ts;
void timeout_cb(void* arg) {
    Application* app = (Application*)arg;
    app->set_timeout(timeout_cb, 1.0, app);

    using namespace std::chrono;
    static double last_time = duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count() / 1000000.0;
    double current_time = duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count() / 1000000.0;

    if (current_time - last_time > 0.001)
        ts.push_back(current_time - last_time);

    double avg = std::reduce(ts.begin(), ts.end(), 0.0) / ts.size();

    printf("timeout at: %.6f, diff: %.6f, avg: %.6f \n", current_time, current_time - last_time, avg);
    last_time = current_time;
}

int main(int argc, char* argv[]) {
    using namespace RolUI;

    RolUI::Application app;
    RolUIBackend::GLFWWindow win(800, 600, "01");
    win.on_exit = [&] { app.exit(); };

    app.set_window(&win);

    std::string font_path = get_font_path();
    // if (win.painter()->load_font("san", font_path.c_str()) == false)
    if (win.painter()->load_font("san", "C:\\WINDOWS\\FONTS\\MSYHL.TTC") == false)
        throw std::runtime_error("can't load font.");

    widget::Button button{"button"};

    win.set_content_widget(&button);

    button.set_pos_relative(RelativeTarget::parent, AnchorPoint::centre_middle, AnchorPoint::centre_middle);
    widget::Button::StyleProperty default_ = button.default_property();
    widget::Button::StyleProperty hover = button.hover_property();
    widget::Button::StyleProperty press = button.press_property();
    default_.font_name = "san";
    hover.font_name = "san";
    press.font_name = "san";
    default_.font_size = 20;
    hover.font_size = 20;
    press.font_size = 20;
    default_.padding = {20, 10};
    hover.padding = {20, 10};
    press.padding = {20, 10};

    button.set_default_property(default_);
    button.set_hover_property(hover);
    button.set_press_property(press);

    button.adjust_size();

    int click_count = 0;
    button.on_click.connect([&]() {
        click_count++;
        button.set_text(std::string("click: ") + std::to_string(click_count));
        button.adjust_size();
    });

    win.show();

    app.set_timeout(timeout_cb, 1.0, &app);

    app.run();

    return 0;
}
