
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

#include <RolUI/Vector.hpp>
#include <RolUI/Point.hpp>
#include <RolUI/Size.hpp>
#include <RolUI/Rect.hpp>
#include <RolUI/Color.hpp>

#include <RolUI/Application.hpp>
#include <RolUI/Window.hpp>
#include <RolUI/Widget.hpp>
#include <RolUI/IPainter.hpp>

using namespace pybind11;
using namespace RolUI;

namespace py = pybind11;

// class PyWidget : public Widget {
//     Widget* get_child_by_pos(Point pos) const noexcept override {
//         PYBIND11_OVERRIDE(Widget*, Widget, get_child_by_pos, pos);
//     }
//     bool handle_event(IEvent* e) noexcept override {
//         PYBIND11_OVERRIDE(bool, Widget, handle_event, e);
//     }
//     void draw(IPainter* painter) noexcept override {
//         PYBIND11_OVERRIDE(void, Widget, draw, painter);
//     }
//     Size perform_layout(Constraint constraint) noexcept override {
//         PYBIND11_OVERRIDE(Size, Widget, perform_layout, constraint);
//     }
//     void update_pos() noexcept override {
//         PYBIND11_OVERRIDE(void, Widget, update_pos, );
//     }
//     bool hit_test(Point pos) const noexcept override {
//         PYBIND11_OVERRIDE(bool, Widget, hit_test, pos);
//     }
// };

static void bind_geometry(py::module_& m) {
    class_<Vec2i>(m, "Vec2i")
        .def(py::init())
        .def(py::init<int, int>())
        .def_readwrite("x", &Vec2i::x)
        .def_readwrite("y", &Vec2i::y)
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
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def(py::self + py::self)
        .def(py::self - py::self)
        .def(py::self * int())
        .def(py::self * float())
        .def(py::self / int())
        .def(py::self / float());

    // class_<Point, Vec2i>(m, "Point")
    //     .def(py::init());

    class_<Rect>(m, "Rect")
        .def(py::init())
        .def(py::init<int, int, int, int>())
        .def(py::init<Point, Size>())
        .def(py::init<Point, Size>())
        .def_static("create_with_two_point", Rect::create_with_two_point)
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
        .def(py::init<uint8_t, uint8_t, uint8_t, uint8_t>())
        .def_readwrite("r", &Color::r)
        .def_readwrite("g", &Color::g)
        .def_readwrite("b", &Color::b)
        .def_readwrite("a", &Color::a)
        .def(py::self == py::self)
        .def(py::self != py::self);
}

PYBIND11_MODULE(PyRolUI, m) {
    m.doc() = "RolUI Python bind."; // optional module docstring

    bind_geometry(m);

    class_<Window>(m, "Window");

    class_<IPainter>(m, "IPainter");

    class_<EventType>(m, "EventType")
        .def("name", &EventType::name);

    class_<IEvent>(m, "IEvent")
        .def("is", &IEvent::is)
        .def("event_type", &IEvent::event_type, return_value_policy::reference)
        .def("target", &IEvent::target, return_value_policy::reference);

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

    class_<Application>(m, "Application")
        .def_static("init", Application::init)
        .def_static("window", Application::window, return_value_policy::reference)
        .def_static("set_content_widget", Application::set_content_widget)
        .def_static("content_widget", Application::content_widget, return_value_policy::reference)
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
}
