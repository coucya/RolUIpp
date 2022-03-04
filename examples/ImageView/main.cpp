#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include <stdexcept>
#include <algorithm>
#include <tuple>

#include "glfw_backend/GLFWWindow.h"

#include "RolUI/Point.hpp"
#include "RolUI/IEvent.hpp"
#include "RolUI/Image.hpp"
#include "RolUI/Widget.hpp"
#include "RolUI/widgets/widgets.hpp"
#include "RolUI/widgets/container.hpp"
#include "RolUI/widgets/flow.hpp"
#include "RolUI/widgets/Image.hpp"
#include "RolUI/widgets/PointerListener.hpp"
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
float image_scale = 1.0f;
Point image_offset = {0, 0};
Image first_image;

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

void init(int argc, char* argv[]) {
    using namespace std::filesystem;
    if (argc < 2)
        image_dir = std::filesystem::current_path().string();
    else
        image_dir = std::filesystem::path(argv[1]).parent_path().string();

    for (auto item : std::filesystem::directory_iterator(image_dir)) {
        std::string en = item.path().extension().string();
        if (en == ".png" || en == ".jpg" || en == ".jpeg" || en == ".bmp")
            image_paths.push_back(item.path().string());
    }

    if (argc > 1 && is_regular_file(path(argv[1]))) {
        first_image = load_image(argv[1]);
    } else if (!image_paths.empty()) {
        first_image = load_image(image_paths[0]);
    }
}

template <typename F>
Widget* build_button(const char* text, F&& callback) {
    return button(label("", 0, {0, 0, 0, 0}, {0, 0, 0, 0}),
                  label(text, 50, {255, 255, 255}, {88, 88, 88, 50}),
                  label(text, 50, {255, 255, 255}, {88, 88, 88, 0}), callback);
}

std::tuple<RolUI::Widget*,
           std::function<void(Image)>,
           std::function<void(Point, bool)>,
           std::function<void(float)>>
build_image_view() {
    PointerListenerWidget* plw = mk_widget<PointerListenerWidget>();
    ScrollWidget* sw = mk_widget<ScrollWidget>();
    SizedBoxWidget* sbw = mk_widget<SizedBoxWidget>()
                              ->width(SizeUnit::pixel(800))
                              ->height(SizeUnit::pixel(600));
    ImageWidget* image_w = mk_widget<ImageWidget>()->fit(ImageWidget::contain);
    plw->set_child(sw->set_child(sbw->set_child(image_w)));

    image_w->image(first_image);

    std::function<void(Image)> set_image{[=](Image image) {
        image_w->image(image);
    }};
    std::function<void(Point, bool)> set_offset{[=](Point offset, bool is_offset = false) {
        if (is_offset)
            sw->scroll_by_px(offset.x, offset.y);
        else
            sw->scroll_to_px(offset.x, offset.y);
    }};
    std::function<void(float)> set_scale{[=](float scale) {
        int w = plw->size().width * scale;
        int h = plw->size().height * scale;
        sbw->width(SizeUnit::pixel(w));
        sbw->height(SizeUnit::pixel(h));
    }};

    return {plw, std::move(set_image), std::move(set_offset), std::move(set_scale)};
}

Widget* build_ui() {
    auto [img_view, set_image, set_offset, set_scale] = build_image_view();

    auto update_image = [=, set_image = set_image, set_offset = set_offset, set_scale = set_scale]() {
        if (image_selected >= image_paths.size()) return;
        Image img = load_image(image_paths[image_selected]);
        set_image(img);
        set_offset({0, 0}, false);
        set_scale(1.0f);
    };
    PointerListenerWidget* plw = mk_widget<PointerListenerWidget>();
    plw->on_drag.connect([=, set_offset = set_offset](Vec2i offset) {
        set_offset(offset, true);
    });
    plw->on_scroll.connect([=, set_scale = set_scale](Vec2i offset) {
        set_scale(image_scale = image_scale + double(offset.y) * 0.1);
    });
    return mk_widget<BoxWidget>()
        ->background_color({40, 44, 52})
        ->set_child(mk_widget<StackWidget>()
                        ->add_child(img_view)
                        ->add_child(
                            mk_widget<RowGridWidget>()
                                ->add_child(build_button("<", [=] { image_idx_dec(); update_image(); }), 1.0)
                                ->add_child(plw->set_child(mk_widget<SizedBoxWidget>()), 9.0)
                                ->add_child(build_button(">", [=] { image_idx_inc(); update_image(); }), 1.0)));
}

int main(int argc, char* argv[]) {
    using namespace RolUI;
    using namespace RolUI::widgets;

    RolUIBackend::GLFWWindow win(800, 600, "image view");
    win.on_exit = [&] { RolUI::Application::exit(); };

    RolUI::Application::init(&win);

    init(argc, argv);

    if (win.painter()->load_font("default", "C:\\WINDOWS\\FONTS\\MSYHL.TTC") == false)
        throw std::runtime_error("can't load font.");

    Widget* w = build_ui();

    win.show();

    Application::run(w);

    return 0;
}
