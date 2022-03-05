
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

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

#include <RolUI/widgets/Text.hpp>
#include <RolUI/widgets/container.hpp>
#include <RolUI/widgets/flow.hpp>
#include <RolUI/widgets/layer.hpp>
#include <RolUI/widgets/focus.hpp>
#include <RolUI/widgets/PointerListener.hpp>
#include <RolUI/widgets/widgets.hpp>

using namespace pybind11;
using namespace RolUI;
using namespace RolUI::widgets;

namespace py = pybind11;

#define BIND_PROPERTY(WT, VT)                                                                \
    {                                                                                        \
        typedef Property<WT, VT> Prop;                                                       \
        class_<Prop>(widgets, "Property<" #WT ", " #VT ">")                                  \
            .def("get", [](Prop& self) { return self.get(); })                               \
            .def(                                                                            \
                "set", [](Prop& self, VT arg) { return self.set(arg); },                     \
                return_value_policy::reference)                                              \
            .def(                                                                            \
                "set_no_notify", [](Prop& self, VT arg) { return self.set_no_notify(arg); }, \
                return_value_policy::reference)                                              \
            .def("__call__", [](Prop& self) { return self(); })                              \
            .def(                                                                            \
                "__call__", [](Prop& self, VT arg) { return self(arg); },                    \
                return_value_policy::reference);                                             \
    }

void bind_widgets(py::module_& m) {
    py::module_ widgets{"widgets", "RolUI widgets."};
    m.attr("widgets") = widgets;

    BIND_PROPERTY(TextWidget, unsigned);
    class_<TextWidget, Widget>(widgets, "TextWidget")
        .def(py::init<std::string>())
        .def_readonly("font_size", &TextWidget::font_size, return_value_policy::reference_internal)
        .def("pos_to_index", &TextWidget::pos_to_index)
        .def("index_to_pos", &TextWidget::index_to_pos)
        .def("line_height", &TextWidget::line_height);

    BIND_PROPERTY(BoxWidget, unsigned);
    BIND_PROPERTY(BoxWidget, Color);
    class_<BoxWidget, SingleChildWidget>(widgets, "BoxWidget")
        .def(py::init())
        .def(py::init<unsigned>())
        .def_readonly("round", &BoxWidget::round, return_value_policy::reference_internal)
        .def_readonly("border_widget", &BoxWidget::border_width, return_value_policy::reference_internal)
        .def_readonly("border_color", &BoxWidget::border_color, return_value_policy::reference_internal)
        .def_readonly("background_color", &BoxWidget::background_color, return_value_policy::reference_internal);

    enum_<SizeUnitType>(widgets, "SizeUnitType")
        .value("pixel", SizeUnitType::pixel)
        .value("percentage", SizeUnitType::percentage)
        .export_values();
    class_<SizeUnit>(widgets, "SizeUnit")
        .def(py::init())
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def("type", &SizeUnit::type)
        .def("pixel_value", static_cast<int (SizeUnit::*)() const noexcept>(&SizeUnit::pixel))
        .def("percentage_value", static_cast<float (SizeUnit::*)() const noexcept>(&SizeUnit::percentage))
        .def_static("pixel", static_cast<SizeUnit (*)(int) noexcept>(SizeUnit::pixel))
        .def_static("percentage", static_cast<SizeUnit (*)(float) noexcept>(SizeUnit::percentage));

    BIND_PROPERTY(SizedBoxWidget, SizeUnit);
    class_<SizedBoxWidget, SingleChildWidget>(widgets, "SizedBoxWidget")
        .def(py::init())
        .def(py::init<SizeUnit, SizeUnit>())
        .def_readonly("width", &SizedBoxWidget::width, return_value_policy::reference_internal)
        .def_readonly("height", &SizedBoxWidget::height, return_value_policy::reference_internal);

    widgets.def("sized", static_cast<Widget* (*)(int, int, Widget*)>(widgets::sized),
                return_value_policy::reference);
    widgets.def("sized", static_cast<Widget* (*)(int, float, Widget*)>(widgets::sized),
                return_value_policy::reference);
    widgets.def("sized", static_cast<Widget* (*)(float, int, Widget*)>(widgets::sized),
                return_value_policy::reference);
    widgets.def("sized", static_cast<Widget* (*)(float, float, Widget*)>(widgets::sized),
                return_value_policy::reference);
}
