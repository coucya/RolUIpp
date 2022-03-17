
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <RolUI/Vector.hpp>
#include <RolUI/Point.hpp>
#include <RolUI/Size.hpp>
#include <RolUI/Rect.hpp>
#include <RolUI/Color.hpp>
#include <RolUI/Image.hpp>
#include <RolUI/IPainter.hpp>
#include <RolUI/Window.hpp>
#include <RolUI/Widget.hpp>
#include <RolUI/Application.hpp>

using namespace pybind11;
using namespace RolUI;

namespace py = pybind11;

#define _CONCAT(l, r) l##r
#define CONCAT(l, r) _CONCAT(l, r)

#define ARG_COUNT(...) _INTERNAL_ARG_COUNT( \
    0, ##__VA_ARGS__,                       \
    8, 7, 6, 5, 4, 3, 2, 1, 0)
#define _INTERNAL_ARG_COUNT( \
    _0, _1, _2, _3, _4, _5, _6, _7, _8, N, ...) N

#define MYPYBIND11_OVERRIDE_PURE_0(ret_type, ct, fn) \
    virtual ret_type fn() override { PYBIND11_OVERRIDE_PURE(ret_type, ct, fn, ); }
#define MYPYBIND11_OVERRIDE_PURE_2(ret_type, ct, fn, a, b) \
    virtual ret_type fn(a b) override { PYBIND11_OVERRIDE_PURE(ret_type, ct, fn, b); }
#define MYPYBIND11_OVERRIDE_PURE_4(ret_type, ct, fn, a, b, c, d) \
    virtual ret_type fn(a b, c d) override { PYBIND11_OVERRIDE_PURE(ret_type, ct, fn, b, d); }
#define MYPYBIND11_OVERRIDE_PURE_6(ret_type, ct, fn, a, b, c, d, e, f) \
    virtual ret_type fn(a b, c d, e f) override { PYBIND11_OVERRIDE_PURE(ret_type, ct, fn, b, d, f); }

#define MYPYBIND11_OVERRIDE_PURE(ret_type, ct, fn, ...)       \
    CONCAT(MYPYBIND11_OVERRIDE_PURE_, ARG_COUNT(__VA_ARGS__)) \
    (ret_type, ct, fn, __VA_ARGS__)

class PyWindow : public Window {
  public:
    virtual Point pos() const override {
        PYBIND11_OVERRIDE_PURE(Point, Window, pos, );
    }
    virtual Size size() const override {
        PYBIND11_OVERRIDE_PURE(Size, Window, size, );
    }

    virtual IPainter* painter() override {
        PYBIND11_OVERRIDE_PURE(IPainter*, Window, painter, );
    }
    virtual void begin_draw() override {
        PYBIND11_OVERRIDE_PURE(void, Window, begin_draw, );
    }

    virtual void end_draw() override {
        PYBIND11_OVERRIDE_PURE(void, Window, end_draw, );
    }

    virtual void dispatch_event(double timeout) override {
        PYBIND11_OVERRIDE_PURE(void, Window, dispatch_event, timeout);
    }
};

#define PyIPainter_OVERRIDE(ret_type, fn, ...) \
    MYPYBIND11_OVERRIDE_PURE(ret_type, IPainter, fn, __VA_ARGS__)
class PyIPainter : public IPainter {
    PyIPainter_OVERRIDE(bool, load_font, const char*, name, const char*, filename);
    PyIPainter_OVERRIDE(int, create_image_with_rgba, const uint8_t*, data, int, w, int, h);
    PyIPainter_OVERRIDE(void, delete_image, int, handle);
    PyIPainter_OVERRIDE(Size, image_size, int, handle);
    PyIPainter_OVERRIDE(void, scissor, Rect, rect);

    PyIPainter_OVERRIDE(void, set_font_size, uint32_t, s);
    PyIPainter_OVERRIDE(void, set_font_color, Color, color);
    PyIPainter_OVERRIDE(void, set_font, const char*, name);

    PyIPainter_OVERRIDE(void, set_stroke_color, Color, color);
    PyIPainter_OVERRIDE(void, set_fill_color, Color, color);
    PyIPainter_OVERRIDE(void, set_stroke_width, uint32_t, w);

    PyIPainter_OVERRIDE(void, draw_text, Point, pos, const char*, text, uint32_t, len);

    PyIPainter_OVERRIDE(void, draw_image, Point, pos, Size, size, int, handle);

    PyIPainter_OVERRIDE(void, draw_line, Point, a, Point, b);
    PyIPainter_OVERRIDE(void, draw_rect, Rect, rect);
    PyIPainter_OVERRIDE(void, draw_circle, Point, centre, uint32_t, r);
    PyIPainter_OVERRIDE(void, draw_ellipse, Rect, rect);
    PyIPainter_OVERRIDE(void, draw_roundedrect, Rect, rect, uint32_t, round);

    PyIPainter_OVERRIDE(void, fill_rect, Rect, rect);
    PyIPainter_OVERRIDE(void, fill_circle, Point, centre, uint32_t, r);
    PyIPainter_OVERRIDE(void, fill_ellipse, Rect, rect);
    PyIPainter_OVERRIDE(void, fill_roundedrect, Rect, rect, uint32_t, round);

    PyIPainter_OVERRIDE(void, draw_hline, Point, a, uint32_t, len);
    PyIPainter_OVERRIDE(void, draw_vline, Point, a, uint32_t, len);

    virtual Size text_size(const char* text, uint32_t len) const override {
        PYBIND11_OVERRIDE_PURE(Size, IPainter, text_size, text, len);
    }
    virtual Size text_size(const char* text) const override {
        PYBIND11_OVERRIDE_PURE(Size, IPainter, text_size, text);
    }
    virtual Rect get_scissor() const override {
        PYBIND11_OVERRIDE_PURE(Rect, IPainter, get_scissor, );
    }
};

static Image load_image(const char* filename) {
    int image_w, image_h, image_c;
    uint8_t* image_data = stbi_load(filename, &image_w, &image_h, &image_c, 4);
    RolUI::Image img = RolUI::Image::create_rgba_mem(image_data, image_w, image_h);
    stbi_image_free(image_data);
    return img;
}
static bool load_font(const char* name, const char* filename) {
    return Application::window()->painter()->load_font("default", "C:\\WINDOWS\\FONTS\\MSYHL.TTC");
}

static std::string Vec2i_to_string(const Vec2i& v) {
    return std::string("Vec2i(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")");
}
static std::string Vec2f_to_string(const Vec2f& v) {
    return std::string("Vec2f(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")");
}
static std::string Size_to_string(const Size& v) {
    return std::string("Size(" + std::to_string(v.width) + ", " + std::to_string(v.height) + ")");
}
static std::string Rect_to_string(const Rect& v) {
    return std::string("Rect(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", "
                       + std::to_string(v.width) + ", " + std::to_string(v.height) + ")");
}
static std::string Color_to_string(const Color& v) {
    return std::string("Color(" + std::to_string(v.r) + ", " + std::to_string(v.g) + ", "
                       + std::to_string(v.b) + ", " + std::to_string(v.a) + ")");
}

void bind_widgets(py::module_& m);
static void bind_geometry(py::module_& m) {
    class_<Vec2i>(m, "Vec2i")
        .def(py::init())
        .def(py::init<int, int>())
        .def_readwrite("x", &Vec2i::x)
        .def_readwrite("y", &Vec2i::y)
        .def("__repr__", Vec2i_to_string)
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def(py::self + py::self)
        .def(py::self - py::self)
        .def(py::self * int())
        .def(py::self * float())
        .def(py::self / int())
        .def(py::self / float());
    class_<Vec2f>(m, "Vec2f")
        .def(py::init())
        .def(py::init<float, float>())
        .def_readwrite("x", &Vec2f::x)
        .def_readwrite("y", &Vec2f::y)
        .def("__repr__", Vec2f_to_string)
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def(py::self + py::self)
        .def(py::self - py::self)
        .def(py::self * int())
        .def(py::self * float())
        .def(py::self / int())
        .def(py::self / float());

    class_<Size>(m, "Size")
        .def(py::init())
        .def(py::init<int, int>())
        .def_readwrite("width", &Size::width)
        .def_readwrite("height", &Size::height)
        .def("__repr__", Size_to_string)
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def(py::self + py::self)
        .def(py::self - py::self)
        .def(py::self * int())
        .def(py::self * float())
        .def(py::self / int())
        .def(py::self / float());

    m.attr("Point") = m.attr("Vec2i");

    class_<Rect>(m, "Rect")
        .def(py::init())
        .def(py::init<int, int, int, int>())
        .def(py::init<Point, Size>())
        .def(py::init<Point, Point>())
        .def("__repr__", Rect_to_string)
        .def_readwrite("x", &Rect::x)
        .def_readwrite("y", &Rect::y)
        .def_readwrite("width", &Rect::width)
        .def_readwrite("height", &Rect::height)
        .def("pos", &Rect::pos)
        .def("size", &Rect::size)
        .def("top", &Rect::top)
        .def("bottom", &Rect::bottom)
        .def("left", &Rect::left)
        .def("right", &Rect::right)
        .def("centre", &Rect::centre)
        .def("middle", &Rect::middle)
        .def("left_top", &Rect::left_top)
        .def("left_bottom", &Rect::left_bottom)
        .def("right_top", &Rect::right_top)
        .def("right_bottom", &Rect::right_bottom)
        .def("centre_top", &Rect::centre_top)
        .def("centre_bottom", &Rect::centre_bottom)
        .def("left_middle", &Rect::left_middle)
        .def("right_middle", &Rect::right_middle)
        .def("centre_middle", &Rect::centre_middle)
        .def("contain", static_cast<bool (Rect::*)(int, int) const noexcept>(&Rect::contain))
        .def("contain", static_cast<bool (Rect::*)(const Point&) const noexcept>(&Rect::contain))
        .def("intersect", &Rect::intersect)
        .def("united", &Rect::united)
        .def("intersected", &Rect::intersected);

    class_<Color>(m, "Color")
        .def(py::init())
        .def(py::init<uint8_t, uint8_t, uint8_t>())
        .def(py::init<uint8_t, uint8_t, uint8_t, uint8_t>())
        .def_readwrite("r", &Color::r)
        .def_readwrite("g", &Color::g)
        .def_readwrite("b", &Color::b)
        .def_readwrite("a", &Color::a)
        .def("__repr__", Color_to_string)
        .def(py::self == py::self)
        .def(py::self != py::self);
}

PYBIND11_MODULE(PyRolUI, m) {
    m.doc() = "RolUI Python bind.";

    bind_geometry(m);

    m.def("load_font", load_font, py::arg("name"), py::arg("filename"));

    class_<Image>(m, "Image")
        .def(py::init())
        .def("handle", &Image::handle)
        .def_static("load", &load_image);

    class_<EventType>(m, "EventType")
        .def("name", &EventType::name);

    class_<IEvent>(m, "IEvent")
        .def("is", &IEvent::is)
        .def("event_type", &IEvent::event_type, return_value_policy::reference)
        .def("target", &IEvent::target, return_value_policy::reference);

    class_<IPainter, PyIPainter>(m, "IPainter")
        .def("load_font", &IPainter::load_font)
        .def("text_size", static_cast<Size (IPainter::*)(const char*, uint32_t) const>(&IPainter::text_size))
        .def("text_size", static_cast<Size (IPainter::*)(const char*) const>(&IPainter::text_size))
        .def("create_image_with_rgba", &IPainter::create_image_with_rgba)
        .def("delete_image", &IPainter::delete_image)
        .def("image_size", &IPainter::image_size)
        .def("scissor", &IPainter::scissor)
        .def("get_scissor", &IPainter::get_scissor)
        .def("set_font_size", &IPainter::set_font_size)
        .def("set_font_color", &IPainter::set_font_color)
        .def("set_font", &IPainter::set_font)
        .def("set_stroke_color", &IPainter::set_stroke_color)
        .def("set_fill_color", &IPainter::set_fill_color)
        .def("set_stroke_width", &IPainter::set_stroke_width)
        .def("draw_text", &IPainter::draw_text)
        .def("draw_image", &IPainter::draw_image)
        .def("draw_line", &IPainter::draw_line)
        .def("draw_rect", &IPainter::draw_rect)
        .def("draw_circle", &IPainter::draw_circle)
        .def("draw_ellipse", &IPainter::draw_ellipse)
        .def("draw_roundedrect", &IPainter::draw_roundedrect)
        .def("fill_rect", &IPainter::fill_rect)
        .def("fill_circle", &IPainter::fill_circle)
        .def("fill_ellipse", &IPainter::fill_ellipse)
        .def("fill_roundedrect", &IPainter::fill_roundedrect)
        .def("draw_hline", &IPainter::draw_hline)
        .def("draw_vline", &IPainter::draw_vline);

    class_<Window, PyWindow>(m, "Window")
        .def(py::init())
        .def("pos", &Window::pos)
        .def("size", &Window::size)
        .def("painter", &Window::painter, return_value_policy::reference)
        .def("begin_draw", &Window::begin_draw)
        .def("end_draw", &Window::end_draw)
        .def("dispatch_event", &Window::dispatch_event);

    class_<Widget>(m, "Widget")
        .def(py::init())
        .def("pos", &Widget::pos)
        .def("size", &Widget::size)
        .def("rect", &Widget::rect)
        .def("abs_pos", &Widget::abs_pos)
        .def("abs_rect", &Widget::abs_rect)
        .def("parent", &Widget::parent, return_value_policy::reference)
        .def("layout", &Widget::layout)
        .def("get_child_by_pos", &Widget::get_child_by_pos, return_value_policy::reference)
        .def("handle_event", &Widget::handle_event)
        .def("draw", &Widget::draw)
        .def("perform_layout", &Widget::perform_layout)
        .def("update_pos", &Widget::update_pos)
        .def("hit_test", &Widget::hit_test);
    class_<SingleChildWidget, Widget>(m, "SingleChildWidget")
        .def(py::init())
        .def("child", &SingleChildWidget::child, return_value_policy::reference)
        .def("set_child", &SingleChildWidget::set_child, return_value_policy::reference);
    class_<MultiChildWidget, Widget>(m, "MultiChildWidget")
        .def(py::init())
        .def("child", &MultiChildWidget::child, return_value_policy::reference)
        .def("add_child", &MultiChildWidget::add_child, return_value_policy::reference)
        .def("set_child", &MultiChildWidget::set_child, return_value_policy::reference)
        .def("remove_child", [](MultiChildWidget& self, Widget* child) { self.remove_child(child); })
        .def("remove_child", [](MultiChildWidget& self, int index) { self.remove_child(index); });

    class_<Application>(m, "Application")
        .def_static("init", Application::init)
        .def_static("window", Application::window, return_value_policy::reference)
        .def_static("root_widget", RootWidget::root_widget, return_value_policy::reference)
        .def_static("has_focus_widget", Application::has_focus_widget)
        .def_static("set_focus_widget", Application::set_focus_widget)
        .def_static("focus_widget", Application::focus_widget, return_value_policy::reference)
        .def_static("get_widget_by_pos", Application::get_widget_by_pos, return_value_policy::reference)
        .def_static("flush_layout", Application::flush_layout)
        .def_static("flush_draw", Application::flush_draw)
        .def_static("flush_frame", Application::flush_frame)
        .def_static("run", static_cast<void (*)()>(&Application::run))
        .def_static("run", static_cast<void (*)(Widget*)>(&Application::run))
        .def_static("exit", Application::exit);

    bind_widgets(m);
}
