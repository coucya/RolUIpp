#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include <RolUI/Vector.hpp>
#include <RolUI/Point.hpp>
#include <RolUI/Size.hpp>
#include <RolUI/Rect.hpp>
#include <RolUI/Color.hpp>

#include <RolUI/Application.hpp>
#include <RolUI/Window.hpp>
#include <RolUI/Widget.hpp>
#include <RolUI/IPainter.hpp>

#include "glfw_backend/GLFWWindow.h"

using namespace pybind11;
using namespace RolUI;
using namespace RolUIBackend;

namespace py = pybind11;

PYBIND11_MODULE(PyRolUI_glfw, m) {
    m.doc() = "RolUI GLFW backend Python bind.";

    class_<GLFWppWindow>(m, "GLFWppWindow")
        .def(py::init<int, int, const char*>())
        .def("make_opengl_context", &GLFWppWindow::make_opengl_context)
        .def("poll_events", &GLFWppWindow::poll_events)
        .def("wait_events", &GLFWppWindow::wait_events)
        .def("wait_events_timeout", &GLFWppWindow::wait_events_timeout)
        .def("set_title", &GLFWppWindow::set_title)
        .def("set_size", &GLFWppWindow::set_size)
        .def("window_pos", &GLFWppWindow::window_pos)
        .def("x", &GLFWppWindow::x)
        .def("y", &GLFWppWindow::y)
        .def("window_size", &GLFWppWindow::window_size)
        .def("width", &GLFWppWindow::width)
        .def("height", &GLFWppWindow::height)
        .def("cursor_pos", &GLFWppWindow::cursor_pos)
        .def("should_close", &GLFWppWindow::should_close)
        .def("hide", &GLFWppWindow::hide)
        .def("show", &GLFWppWindow::show)
        .def("swap_buffer", &GLFWppWindow::swap_buffer)
        .def("close", &GLFWppWindow::close)
        .def_readwrite("on_exit", &GLFWppWindow::on_exit)
        .def_readwrite("on_size", &GLFWppWindow::on_size)
        .def_readwrite("on_char", &GLFWppWindow::on_char)
        .def_readwrite("on_cursor_enter", &GLFWppWindow::on_cursor_enter)
        .def_readwrite("on_cursor_leave", &GLFWppWindow::on_cursor_leave)
        .def_readwrite("on_cursor_pos", &GLFWppWindow::on_cursor_pos)
        .def_readwrite("on_mouse_button", &GLFWppWindow::on_mouse_button)
        .def_readwrite("on_scroll", &GLFWppWindow::on_scroll);

    class_<GLFWWindow, GLFWppWindow, RolUI::Window>(m, "GLFWWindow")
        .def(py::init<int, int, const char*>())
        .def("pos", &GLFWWindow::pos)
        .def("size", &GLFWWindow::size)
        .def("painter", &GLFWWindow::painter, return_value_policy::reference)
        .def("begin_draw", &GLFWWindow::begin_draw)
        .def("end_draw", &GLFWWindow::end_draw)
        .def("dispatch_event", &GLFWWindow::dispatch_event);
}