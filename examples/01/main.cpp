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
#include "RolUI/IEvent.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/widgets/EllipseWidget.hpp"
#include "RolUI/widgets/Rect.hpp"
#include "RolUI/widgets/Text.hpp"
#include "RolUI/widgets/Label.hpp"
#include "RolUI/widgets/Button.hpp"
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

    widget::Button button{"button"};

    win.set_content_widget(&button);

    button.set_pos_relative(RelativeTarget::parent, AnchorPoint::centre_middle, AnchorPoint::centre_middle);
    button.adjust_size();

    Timer timer;
    timer.on_timeout.connect([](double interval) {
        printf("timer.on_timeout: %.6f \n", interval);
    });

    int click_count = 0;
    button.on_click.connect([&]() {
        click_count++;
        button.set_text(std::string("click: ") + std::to_string(click_count));
        button.adjust_size();
        timer.start(&app, 1);
    });

    win.show();

    app.set_timeout(timeout_cb, 1.0, &app);

    app.run();

    return 0;
}
