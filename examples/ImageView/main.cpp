#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include <stdexcept>
#include <algorithm>

#include "glfw_backend/GLFWWindow.h"

#include "RolUI/Point.hpp"
#include "RolUI/IEvent.hpp"
#include "RolUI/Image.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/widgets/widgets.hpp"
#include "RolUI/widgets/container.hpp"
#include "RolUI/widgets/flow.hpp"
#include "RolUI/widgets/Image.hpp"
#include "RolUI/events/MouseEvent.hpp"
#include "RolUI/events/Widget_event.hpp"
#include "RolUI/Application.hpp"

#include "stb_image.h"

using namespace RolUI;
using namespace RolUI::widgets;

std::string resources_dir() {
    std::filesystem::path self_path{__FILE__};
    std::filesystem::path path = self_path.parent_path() / ".." / "resources";
    return path.string();
}

std::map<std::string, RolUI::Image> image_cache;
Image load_image(std::string filename) {
    using namespace std::filesystem;

    auto abs_path = absolute(path(filename)).string();

    RolUI::Image img;
    if (image_cache.find(abs_path) == image_cache.end()) {
        int image_w, image_h, image_c;
        uint8_t* image_data = stbi_load(filename.c_str(), &image_w, &image_h, &image_c, 4);

        img = RolUI::Image::create_rgba_mem(image_data, image_w, image_h);
        image_cache[abs_path] = img;

        stbi_image_free(image_data);
    } else {
        img = image_cache[abs_path];
    }

    return img;
}

std::string image_dir;
std::vector<std::string> image_paths;
int image_selected = 0;

void image_idx_inc() {
    if (image_paths.size() == 0) return;
    image_selected++;
    if (image_selected >= image_paths.size())
        image_selected = 0;
}
void image_idx_dec() {
    if (image_paths.size() == 0) return;
    image_selected--;
    if (image_selected < 0)
        image_selected = std::max<int>(image_paths.size() - 1, 0);
}
void update_image() {
}

void init(int argc, char* argv[]) {
    if (argc < 2)
        image_dir = std::filesystem::current_path().string();
    else
        image_dir = std::filesystem::path(argv[1]).parent_path().string();

    for (auto item : std::filesystem::directory_iterator(image_dir)) {
        std::string en = item.path().extension().string();
        if (en == ".png" || en == ".jpg" || en == ".jpeg" || en == ".bmp")
            image_paths.push_back(item.path().string());
    }
}

template <typename F>
Widget* build_button(const char* text, F&& callback) {
    return sized(75, 35,
                 button(text, std::forward<F>(callback)));
}

template <typename F>
Widget* build_control_bar(F&& update_image) {
    return mk_widget<AlignWidget>()
        ->set_child(
            mk_widget<RowWidget>()
                ->gap(50)
                ->add_child(build_button("上一个", [=](Point) { image_idx_dec(); update_image(); }))
                ->add_child(build_button("下一个", [=](Point) { image_idx_inc(); update_image(); })));
}

Widget* build_ui() {
    ImageWidget* img_widget = mk_widget<ImageWidget>();
    auto update_image = [=]() {
        if (image_selected >= image_paths.size()) return;
        Image img = load_image(image_paths[image_selected]);
        img_widget->image(img);
    };
    return mk_widget<ColumnGridWidget>()
        ->add_child(img_widget, 9)
        ->add_child(build_control_bar(update_image));
}

int main(int argc, char* argv[]) {
    using namespace RolUI;
    using namespace RolUI::widgets;

    init(argc, argv);

    RolUIBackend::GLFWWindow win(800, 600, "image");
    win.on_exit = [&] { RolUI::Application::exit(); };

    RolUI::Application::init(&win);

    if (win.painter()->load_font("default", "C:\\WINDOWS\\FONTS\\MSYHL.TTC") == false)
        throw std::runtime_error("can't load font.");

    Widget* w = build_ui();

    win.show();

    Application::run(w);

    return 0;
}
