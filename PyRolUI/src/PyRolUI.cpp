
#include "pybind11/operators.h"
#include "pybind11/pybind11.h"
#include "pybind11/pytypes.h"
#include "pybind11/stl.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iterator>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <unordered_map>

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
#include <RolUI/events/MouseEvent.hpp>
#include <RolUI/events/CharEvent.hpp>
#include <RolUI/events/KeyboardEvent.hpp>

using namespace pybind11;
using namespace RolUI;

namespace py = pybind11;

class RolUIPyObject : public RolUI::Object {
  public:
    virtual const ObjectType* object_type() const noexcept override {
        PYBIND11_OVERRIDE_PURE(const ObjectType*, Object, );
    }
};

class PyWindow : public Window {
  public:
    virtual Point pos() const override { PYBIND11_OVERRIDE_PURE(Point, Window, pos, ); }
    virtual Size size() const override { PYBIND11_OVERRIDE_PURE(Size, Window, size, ); }

    virtual IPainter* painter() override { PYBIND11_OVERRIDE_PURE(IPainter*, Window, painter, ); }
    virtual void begin_draw() override { PYBIND11_OVERRIDE_PURE(void, Window, begin_draw, ); }
    virtual void end_draw() override { PYBIND11_OVERRIDE_PURE(void, Window, end_draw, ); }

    virtual void dispatch_event(double timeout) override { PYBIND11_OVERRIDE_PURE(void, Window, dispatch_event, timeout); }
};

class PyIPainter : public IPainter {

    Size text_size(const char* text, uint32_t len) const override {
        PYBIND11_OVERRIDE_PURE(Size, IPainter, text_size, text, len);
    }

    int32_t create_image(const uint8_t* data, uint32_t w, uint32_t h) override {
        PYBIND11_OVERRIDE_PURE(int32_t, IPainter, create_image, data, w, h);
    }
    void delete_image(int32_t handle) override {
        PYBIND11_OVERRIDE_PURE(void, IPainter, delete_image, handle);
    }
    Size image_size(int32_t handle) override {
        PYBIND11_OVERRIDE_PURE(Size, IPainter, image_size, handle);
    }

    int32_t create_font(const uint8_t* data, uint32_t len) override {
        PYBIND11_OVERRIDE_PURE(int32_t, IPainter, create_font, data, len);
    }
    void set_font(int32_t handle) override {
        PYBIND11_OVERRIDE_PURE(void, IPainter, set_font, handle);
    }
    void set_font_size(uint32_t s) override {
        PYBIND11_OVERRIDE_PURE(void, IPainter, set_font_size, s);
    }
    void set_font_color(Color color) override {
        PYBIND11_OVERRIDE_PURE(void, IPainter, set_font_color, color);
    }

    void clip_rect(Rect rect) override {
        PYBIND11_OVERRIDE_PURE(void, IPainter, clip_rect, rect);
    }
    Rect get_clip_rect() const override {
        PYBIND11_OVERRIDE_PURE(Rect, IPainter, get_clip_rect, );
    }
    void save_clip() override {
        PYBIND11_OVERRIDE_PURE(void, IPainter, save_clip, );
    }
    void restore_clip() override {
        PYBIND11_OVERRIDE_PURE(void, IPainter, restore_clip, );
    }

    void set_fill_color(Color color) override {
        PYBIND11_OVERRIDE_PURE(void, IPainter, set_fill_color, color);
    }

    void set_line_color(Color color) override {
        PYBIND11_OVERRIDE_PURE(void, IPainter, set_line_color, color);
    }
    void set_line_width(uint32_t w) override {
        PYBIND11_OVERRIDE_PURE(void, IPainter, set_line_width, w);
    }
    void set_line_cap(int32_t cap) override {
        PYBIND11_OVERRIDE_PURE(void, IPainter, set_line_cap, cap);
    }
    void set_line_join(int32_t join) override {
        PYBIND11_OVERRIDE_PURE(void, IPainter, set_line_join, join);
    }

    void fill_text(Point pos, const char* text, uint32_t len) override {
        PYBIND11_OVERRIDE_PURE(void, IPainter, fill_text, text, len);
    }

    void draw_image(Point pos, Size size, int handle) override {
        PYBIND11_OVERRIDE_PURE(void, IPainter, draw_image, pos, size, handle);
    }
    void draw_line(Point a, Point b) override {
        PYBIND11_OVERRIDE_PURE(void, IPainter, draw_line, a, b);
    }

    void stroke_rect(Rect rect) override {
        PYBIND11_OVERRIDE_PURE(void, IPainter, stroke_rect, rect);
    }
    void stroke_circle(Point centre, uint32_t r) override {
        PYBIND11_OVERRIDE_PURE(void, IPainter, stroke_circle, centre, r);
    }
    void stroke_ellipse(Rect rect) override {
        PYBIND11_OVERRIDE_PURE(void, IPainter, stroke_ellipse, rect);
    }
    void stroke_roundedrect(Rect rect, uint32_t round) override {
        PYBIND11_OVERRIDE_PURE(void, IPainter, stroke_roundedrect, rect, round);
    }

    void fill_rect(Rect rect) override {
        PYBIND11_OVERRIDE_PURE(void, IPainter, fill_rect, rect);
    }
    void fill_ellipse(Rect rect) override {
        PYBIND11_OVERRIDE_PURE(void, IPainter, fill_ellipse, rect);
    }
    void fill_circle(Point centre, uint32_t r) override {
        PYBIND11_OVERRIDE_PURE(void, IPainter, fill_circle, centre, r);
    }
    void fill_roundedrect(Rect rect, uint32_t round) override {
        PYBIND11_OVERRIDE_PURE(void, IPainter, fill_roundedrect, rect, round);
    }
};

template <typename WidgetBase>
class PyWidget : public WidgetBase {
  public:
    int child_count() const noexcept override {
        PYBIND11_OVERRIDE(int, WidgetBase, child_count, );
    }
    Widget* child(int index) const noexcept override {
        PYBIND11_OVERRIDE(Widget*, WidgetBase, child, index);
    }
    void set_child(Widget* child, int index) noexcept override {
        PYBIND11_OVERRIDE(void, WidgetBase, set_child, child, index);
    }
    void rm_child(int index = 0) noexcept override {
        PYBIND11_OVERRIDE(void, WidgetBase, rm_child, index);
    }

    bool hit_test(Point pos) noexcept override {
        PYBIND11_OVERRIDE(bool, WidgetBase, hit_test, pos);
    }
    bool hit_test_self(Point pos) noexcept override {
        PYBIND11_OVERRIDE(bool, WidgetBase, hit_test_self, pos);
    }
    Widget* hit_test_children(Point pos) noexcept override {
        PYBIND11_OVERRIDE(Widget*, WidgetBase, hit_test_children, pos);
    }

    bool handle_event(IEvent* e) noexcept override {
        PYBIND11_OVERRIDE(bool, WidgetBase, handle_event, e);
    }

    void perform_draw(IPainter* painter) noexcept override {
        PYBIND11_OVERRIDE(void, WidgetBase, perform_draw, painter);
    }

    Size perform_layout(Constraint constraint) noexcept override {
        PYBIND11_OVERRIDE(Size, WidgetBase, perform_layout, constraint);
    }
};

static Image load_image_mem(py::bytes bytes, uint32_t w, uint32_t h) {
    if (!PyBytes_Check(bytes.ptr()))
        throw std::runtime_error("load_image_mem_file(): invalid image data.");
    auto byte_size = PyBytes_Size(bytes.ptr());
    auto byte_data = PyBytes_AsString(bytes.ptr());

    if (w * h * 4 > byte_size)
        throw std::runtime_error("load_image_mem(): w * h * 4 > bytes.size().");
    RolUI::Image img = RolUI::Image::create_rgba_mem((const uint8_t*)byte_data, w, h);
    return img;
}
static Image load_image_mem_file(py::bytes bytes) {
    int image_w, image_h, image_c;

    if (!PyBytes_Check(bytes.ptr()))
        throw std::runtime_error("load_image_mem_file(): invalid image data.");
    auto byte_size = PyBytes_Size(bytes.ptr());
    auto byte_data = PyBytes_AsString(bytes.ptr());

    auto image_data = stbi_load_from_memory((const uint8_t*)byte_data, byte_size, &image_w, &image_h, &image_c, 4);

    if (!image_data)
        throw std::runtime_error("load_image_mem_file(): invalid image data.");
    RolUI::Image img = RolUI::Image::create_rgba_mem((const uint8_t*)image_data, image_w, image_h);
    stbi_image_free(image_data);
    return img;
}
static Image load_image(const char* filename) {
    int image_w, image_h, image_c;
    uint8_t* image_data = stbi_load(filename, &image_w, &image_h, &image_c, 4);
    if (!image_data)
        throw std::runtime_error("load_image(): invalid image file.");
    RolUI::Image img = RolUI::Image::create_rgba_mem(image_data, image_w, image_h);
    stbi_image_free(image_data);
    return img;
}

std::unordered_map<std::string, int32_t> _fonts;

static int32_t load_font(const char* filename) {
    auto it = _fonts.find(filename);
    if (it != _fonts.end()) return it->second;

    std::ifstream in(filename);
    std::string buf{std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
    int32_t handle = Application::window()->painter()->create_font((const uint8_t*)buf.data(), buf.size());
    if (handle >= 0)
        _fonts[filename] = handle;
    return handle;
}
static int32_t load_font_mem_file(const char* name, py::bytes bytes) {
    if (!PyBytes_Check(bytes.ptr()))
        throw std::runtime_error("load_font_mem_file(): invalid font data.");

    auto byte_size = PyBytes_Size(bytes.ptr());
    auto byte_data = PyBytes_AsString(bytes.ptr());
    int32_t handle = Application::window()->painter()->create_font((const uint8_t*)byte_data, byte_size);
    if (handle >= 0)
        _fonts[name] = handle;
    return handle;
}

static std::string Vec2i_to_string(const Vec2i& v) {
    char buf[64]{0};
    sprintf(buf, "vec2i(%d, %d)", v.x, v.y);
    return buf;
}
static std::string Vec2f_to_string(const Vec2f& v) {
    char buf[64]{0};
    sprintf(buf, "vec2f(%.2f, %.2f)", v.x, v.y);
    return buf;
}
static std::string Size_to_string(const Size& v) {
    char buf[64]{0};
    sprintf(buf, "Size(%d, %d)", v.width, v.height);
    return buf;
}
static std::string Rect_to_string(const Rect& v) {
    char buf[128]{0};
    sprintf(buf, "Rect(%d, %d, %d, %d)", v.x, v.y, v.width, v.height);
    return buf;
}
static std::string Color_to_string(const Color& v) {
    char buf[128]{0};
    sprintf(buf, "Color(%d, %d, %d, %d)", v.r, v.g, v.b, v.a);
    return buf;
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

static void bind_mouse_event(py::module_& m) {
    {
        enum_<MouseKeyMode>(m, "MouseKeyMode")
            .value("press", MouseKeyMode::press)
            .value("release", MouseKeyMode::release);
        enum_<MouseKey>(m, "MouseKey")
            .value("unkown", MouseKey::unkown)
            .value("key1", MouseKey::key1)
            .value("key2", MouseKey::key2)
            .value("key3", MouseKey::key3)
            .value("key4", MouseKey::key4)
            .value("key5", MouseKey::key5)
            .value("key6", MouseKey::key6)
            .value("key7", MouseKey::key7)
            .value("key8", MouseKey::key8)
            .value("left", MouseKey::left)
            .value("right", MouseKey::right)
            .value("middle", MouseKey::middle);
    }

    class_<MouseEvent, IEvent>(m, "MouseEvent")
        .def("pos", &MouseEvent::pos)
        .def("offset", &MouseEvent::offset)
        .def("action", &MouseEvent::action)
        .def("button", &MouseEvent::button);

    class_<MouseWheelEvent, IEvent>(m, "MouseWheelEvent")
        .def("offset", &MouseWheelEvent::offset);

    m.def("MouseMoveEvent_type", &RolUI::object_type_of<MouseMoveEvent>, py::return_value_policy::reference);
    m.def("MousePressEvent_type", &RolUI::object_type_of<MousePressEvent>, py::return_value_policy::reference);
    m.def("MouseReleaseEvent_type", &RolUI::object_type_of<MouseReleaseEvent>, py::return_value_policy::reference);
    m.def("MouseEnterEvent_type", &RolUI::object_type_of<MouseEnterEvent>, py::return_value_policy::reference);
    m.def("MouseLeaveEvent_type", &RolUI::object_type_of<MouseLeaveEvent>, py::return_value_policy::reference);
    m.def("MouseWheelEvent_type", &RolUI::object_type_of<MouseWheelEvent>, py::return_value_policy::reference);
}

static void bind_char_event(py::module_& m) {
    class_<CharEvent, IEvent>(m, "CharEvent")
        .def("codepoint", &CharEvent::codepoint)
        .def("c_char", &CharEvent::c_char);

    m.def("CharEvent_type", &RolUI::object_type_of<CharEvent>, py::return_value_policy::reference);
}

static void bind_keyboard_event(py::module_& m) {
    {
        enum_<KeyboardKeyMode>(m, "KeyboardKeyMode")
            .value("press", KeyboardKeyMode::press)
            .value("release", KeyboardKeyMode::release);
        enum_<KeyboardKey>(m, "KeyboardKey")
            .value("unknown", KeyboardKey::unknown)
            .value("space", KeyboardKey::space)
            .value("apostrophe", KeyboardKey::apostrophe, "'") /* ' */
            .value("comma", KeyboardKey::comma, ",")           /* , */
            .value("minus", KeyboardKey::minus, "-")           /* - */
            .value("period", KeyboardKey::period, ".")         /* . */
            .value("slash", KeyboardKey::slash, "/")           /* / */
            .value("_0", KeyboardKey::_0)
            .value("_1", KeyboardKey::_1)
            .value("_2", KeyboardKey::_2)
            .value("_3", KeyboardKey::_3)
            .value("_4", KeyboardKey::_4)
            .value("_5", KeyboardKey::_5)
            .value("_6", KeyboardKey::_6)
            .value("_7", KeyboardKey::_7)
            .value("_8", KeyboardKey::_8)
            .value("_9", KeyboardKey::_9)
            .value("semicolon", KeyboardKey::semicolon, ";") /* ; */
            .value("equal", KeyboardKey::equal, "=")         /* = */
            .value("a", KeyboardKey::a)
            .value("b", KeyboardKey::b)
            .value("c", KeyboardKey::c)
            .value("d", KeyboardKey::d)
            .value("e", KeyboardKey::e)
            .value("f", KeyboardKey::f)
            .value("g", KeyboardKey::g)
            .value("h", KeyboardKey::h)
            .value("i", KeyboardKey::i)
            .value("j", KeyboardKey::j)
            .value("k", KeyboardKey::k)
            .value("l", KeyboardKey::l)
            .value("m", KeyboardKey::m)
            .value("n", KeyboardKey::n)
            .value("o", KeyboardKey::o)
            .value("p", KeyboardKey::p)
            .value("q", KeyboardKey::q)
            .value("r", KeyboardKey::r)
            .value("s", KeyboardKey::s)
            .value("t", KeyboardKey::t)
            .value("u", KeyboardKey::u)
            .value("v", KeyboardKey::v)
            .value("w", KeyboardKey::w)
            .value("x", KeyboardKey::x)
            .value("y", KeyboardKey::y)
            .value("z", KeyboardKey::z)
            .value("left_bracket", KeyboardKey::left_bracket, "[")   /* [ */
            .value("backslash", KeyboardKey::backslash, "\\")        /* \ */
            .value("right_bracket", KeyboardKey::right_bracket, "]") /* ] */
            .value("grave_accent", KeyboardKey::grave_accent, "`")   /* ` */
            .value("escape", KeyboardKey::escape)
            .value("enter", KeyboardKey::enter)
            .value("tab", KeyboardKey::tab)
            .value("backspace", KeyboardKey::backspace)
            .value("insert", KeyboardKey::insert)
            .value("delete_", KeyboardKey::delete_)
            .value("right", KeyboardKey::right)
            .value("left", KeyboardKey::left)
            .value("down", KeyboardKey::down)
            .value("up", KeyboardKey::up)
            .value("page_up", KeyboardKey::page_up)
            .value("page_down", KeyboardKey::page_down)
            .value("home", KeyboardKey::home)
            .value("end", KeyboardKey::end)
            .value("caps_lock", KeyboardKey::caps_lock)
            .value("scroll_lock", KeyboardKey::scroll_lock)
            .value("num_lock", KeyboardKey::num_lock)
            .value("print_screen", KeyboardKey::print_screen)
            .value("pause", KeyboardKey::pause)
            .value("f1", KeyboardKey::f1)
            .value("f2", KeyboardKey::f2)
            .value("f3", KeyboardKey::f3)
            .value("f4", KeyboardKey::f4)
            .value("f5", KeyboardKey::f5)
            .value("f6", KeyboardKey::f6)
            .value("f7", KeyboardKey::f7)
            .value("f8", KeyboardKey::f8)
            .value("f9", KeyboardKey::f9)
            .value("f10", KeyboardKey::f10)
            .value("f11", KeyboardKey::f11)
            .value("f12", KeyboardKey::f12)
            .value("f13", KeyboardKey::f13)
            .value("f14", KeyboardKey::f14)
            .value("f15", KeyboardKey::f15)
            .value("f16", KeyboardKey::f16)
            .value("f17", KeyboardKey::f17)
            .value("f18", KeyboardKey::f18)
            .value("f19", KeyboardKey::f19)
            .value("f20", KeyboardKey::f20)
            .value("f21", KeyboardKey::f21)
            .value("f22", KeyboardKey::f22)
            .value("f23", KeyboardKey::f23)
            .value("f24", KeyboardKey::f24)
            .value("f25", KeyboardKey::f25)
            .value("kp_0", KeyboardKey::kp_0)
            .value("kp_1", KeyboardKey::kp_1)
            .value("kp_2", KeyboardKey::kp_2)
            .value("kp_3", KeyboardKey::kp_3)
            .value("kp_4", KeyboardKey::kp_4)
            .value("kp_5", KeyboardKey::kp_5)
            .value("kp_6", KeyboardKey::kp_6)
            .value("kp_7", KeyboardKey::kp_7)
            .value("kp_8", KeyboardKey::kp_8)
            .value("kp_9", KeyboardKey::kp_9)
            .value("kp_decimal", KeyboardKey::kp_decimal)
            .value("kp_divide", KeyboardKey::kp_divide)
            .value("kp_multiply", KeyboardKey::kp_multiply)
            .value("kp_subtract", KeyboardKey::kp_subtract)
            .value("kp_add", KeyboardKey::kp_add)
            .value("kp_enter", KeyboardKey::kp_enter)
            .value("kp_equal", KeyboardKey::kp_equal)
            .value("left_shift", KeyboardKey::left_shift)
            .value("left_control", KeyboardKey::left_control)
            .value("left_alt", KeyboardKey::left_alt)
            .value("left_super", KeyboardKey::left_super)
            .value("right_shift", KeyboardKey::right_shift)
            .value("right_control", KeyboardKey::right_control)
            .value("right_alt", KeyboardKey::right_alt)
            .value("right_super", KeyboardKey::right_super)
            .value("menu", KeyboardKey::menu);
    }

    class_<KeyboardEvent, IEvent>(m, "KeyboardEvent")
        .def("action", &KeyboardEvent::action)
        .def("key_mode", [](const KeyboardEvent& self) { return self.key_mode(); })
        .def(
            "key_mode", [](const KeyboardEvent& self, KeyboardKey k) { return self.key_mode(k); },
            py::arg("key"));

    m.def("KeyboardEvent_type", &RolUI::object_type_of<KeyboardEvent>, py::return_value_policy::reference);
}

PYBIND11_MODULE(PyRolUI, m) {
    m.doc() = "RolUI Python bind.";

    bind_geometry(m);

    py::module_ events_module = m.def_submodule("events", "RolUI events Python bind.");

    m.def("load_font", load_font, py::arg("filename"));
    m.def("load_font_mem_file", load_font_mem_file, py::arg("name"), py::arg("bytes"));
    m.def("load_image", load_image, py::arg("filename"));
    m.def("load_image_mem_file", load_image_mem_file, py::arg("bytes"));
    m.def("load_image_mem", load_image_mem, py::arg("bytes"), py::arg("w"), py::arg("h"));

    class_<ObjectType>(m, "ObjectType")
        .def("type_name", &ObjectType::type_name)
        .def("is_superclass", [](const ObjectType& self, const ObjectType* other) {
            return self.is_superclass(other);
        });

    class_<Object, RolUIPyObject>(m, "Object")
        .def("object_type", &Object::object_type, return_value_policy::reference)
        .def("object_type_is", [](const Object& self, const ObjectType* other) {
            return self.object_type_is(other);
        })
        .def("object_ref", &Object::object_ref, return_value_policy::reference_internal)
        .def("object_unref", &Object::object_unref, return_value_policy::reference_internal);

    class_<Image>(m, "Image")
        .def(py::init())
        .def("handle", &Image::handle)
        .def_static("load", &load_image);

    class_<IEvent, Object>(m, "IEvent")
        .def("target", &IEvent::target, return_value_policy::reference);

    class_<IPainter, PyIPainter>(m, "IPainter");

    class_<Window, PyWindow>(m, "Window")
        .def(py::init())
        .def("pos", &Window::pos)
        .def("size", &Window::size)
        .def("painter", &Window::painter, return_value_policy::reference)
        .def("begin_draw", &Window::begin_draw)
        .def("end_draw", &Window::end_draw)
        .def("dispatch_event", &Window::dispatch_event);

    class_<Constraint>(m, "Constraint")
        .def(py::init())
        .def(py::init<Size, Size>())
        .def(py::init<int, int, int, int>())
        .def("min", &Constraint::min)
        .def("max", &Constraint::max)
        .def("min_width", &Constraint::min_width)
        .def("min_height", &Constraint::min_height)
        .def("max_width", &Constraint::max_width)
        .def("max_height", &Constraint::max_height)
        .def_static("zero", Constraint::zero)
        .def_static("zero_to", static_cast<Constraint (*)(Size)>(Constraint::zero_to))
        .def_static("zero_to", static_cast<Constraint (*)(int, int)>(Constraint::zero_to))
        .def_static("limit", static_cast<Constraint (*)(Size)>(Constraint::limit))
        .def_static("limit", static_cast<Constraint (*)(int, int)>(Constraint::limit))
        .def_static("limit", static_cast<Constraint (*)(Size, Size)>(Constraint::limit))
        .def_static("limit", static_cast<Constraint (*)(int, int, int, int)>(Constraint::limit))
        .def_static("unlimit", Constraint::unlimit);

    class_<Widget, PyWidget<Widget>, Object>(m, "Widget")
        .def(py::init())
        .def("pos", &Widget::pos)
        .def("size", &Widget::size)
        .def("rect", &Widget::rect)
        .def("abs_pos", &Widget::abs_pos)
        .def("abs_rect", &Widget::abs_rect)
        .def("opaque", &Widget::opaque)
        .def("is_hit", &Widget::is_hit)
        .def("mounted", &Widget::mounted)
        .def("depth", &Widget::depth)
        .def("parent", &Widget::parent, return_value_policy::reference)
        .def("set_opaque", &Widget::set_opaque)
        .def("mark_hit", &Widget::mark_hit)
        .def("clear_hit", &Widget::clear_hit)
        .def("clear_hit_self", &Widget::clear_hit_self)
        .def("layout", &Widget::layout)
        .def("draw", &Widget::draw)
        .def("update_pos", &Widget::update_pos)
        .def("child_count", &Widget::child_count)
        .def("child", &Widget::child, py::arg("index") = 0, return_value_policy::reference)
        .def("set_child", &Widget::set_child, py::arg("child"), py::arg("index") = 0, return_value_policy::reference_internal)
        .def("rm_child", &Widget::rm_child, py::arg("index") = 0)
        .def("hit_test", &Widget::hit_test)
        .def("hit_test_self", &Widget::hit_test_self)
        .def("hit_test_children", &Widget::hit_test_children, return_value_policy::reference)
        .def("handle_event", &Widget::handle_event)
        .def("perform_layout", &Widget::perform_layout)
        .def("perform_draw", &Widget::perform_draw);
    class_<SingleChildWidget, PyWidget<SingleChildWidget>, Widget>(m, "SingleChildWidget")
        .def(py::init())
        .def("child_count", &SingleChildWidget::child_count)
        .def("child", &SingleChildWidget::child, py::arg("index") = 0, return_value_policy::reference)
        .def("set_child", &SingleChildWidget::set_child, py::arg("child"), py::arg("index") = 0, return_value_policy::reference_internal)
        .def("rm_child", &SingleChildWidget::rm_child, py::arg("index") = 0);
    class_<MultiChildWidget, PyWidget<MultiChildWidget>, Widget>(m, "MultiChildWidget")
        .def(py::init())
        .def("child", &MultiChildWidget::child, return_value_policy::reference)
        .def("child_count", &MultiChildWidget::child_count)
        .def("add_child", &MultiChildWidget::add_child, py::arg("child"), return_value_policy::reference_internal)
        .def("set_child", &MultiChildWidget::set_child, py::arg("child"), py::arg("index"), return_value_policy::reference_internal)
        .def("insert_child", &MultiChildWidget::insert_child, py::arg("index"), py::arg("child"), return_value_policy::reference_internal)
        .def("rm_child", [](MultiChildWidget& self, Widget* child) { self.rm_child(child); })
        .def("rm_child", [](MultiChildWidget& self, int index) { self.rm_child(index); })
        .def("rm_child_all", &MultiChildWidget::rm_child_all);

    class_<RootWidget, PyWidget<RootWidget>, MultiChildWidget>(m, "RootWidget")
        .def("content_widget", &RootWidget::content_widget, return_value_policy::reference)
        .def("set_content_widget", &RootWidget::set_content_widget);

    class_<Application>(m, "Application")
        .def_static("init", Application::init)
        .def_static("window", Application::window, return_value_policy::reference)
        .def_static("root_widget", Application::root_widget, return_value_policy::reference)
        .def_static("has_focus_widget", Application::has_focus_widget)
        .def_static("set_focus_widget", Application::set_focus_widget)
        .def_static("focus_widget", Application::focus_widget, return_value_policy::reference)
        .def_static("set_timeout", Application::set_timeout)
        .def_static("set_interval", Application::set_interval)
        .def_static("clear_timeout", Application::clear_timeout)
        .def_static("clear_interval", Application::clear_interval)
        .def_static("get_widget_by_pos", Application::get_widget_by_pos, return_value_policy::reference)
        .def_static("flush_layout", Application::flush_layout)
        .def_static("flush_draw", Application::flush_draw)
        .def_static("flush_frame", Application::flush_frame)
        .def_static("run", static_cast<void (*)()>(&Application::run))
        .def_static("run", static_cast<void (*)(Widget*)>(&Application::run))
        .def_static("exit", Application::exit);

    bind_widgets(m);
    bind_mouse_event(events_module);
    bind_char_event(events_module);
    bind_keyboard_event(events_module);
}
