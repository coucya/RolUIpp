
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>

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

#include <RolUI/sigslot/Signal.hpp>
#include <RolUI/sigslot/Slot.hpp>

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

#define BIND_SINGAL(mod, ...)                                                                       \
    {                                                                                               \
        typedef Signal<__VA_ARGS__> Sign;                                                           \
        class_<Sign>(mod, "Signal<" #__VA_ARGS__ ">")                                               \
            .def(py::init())                                                                        \
            .def("slot_count", &Sign::slot_count)                                                   \
            .def("connect", [](Sign& self, std::function<void(__VA_ARGS__)> f) {                    \
                return self.connect(std::move(f));                                                  \
            })                                                                                      \
            .def("disconnect", static_cast<void (Sign::*)(size_t)>(&Sign::disconnect))              \
            .def("disconnect_all", &Sign::disconnect_all)                                           \
            .def("emit", static_cast<void (Sign::*)(__VA_ARGS__) const noexcept>(&Sign::emit))      \
            .def("__call__", static_cast<void (Sign::*)(__VA_ARGS__) const noexcept>(&Sign::emit)); \
    }

static void bind_signals(py::module_& m) {
    py::module_ signals{"signals", "RolUI signals."};
    m.attr("signals") = signals;

    BIND_SINGAL(signals, Vec2i);
    BIND_SINGAL(signals, bool);
}

static void bind_container_widgets(py::module_& widgets) {
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

    BIND_PROPERTY(BoxWidget, unsigned);
    BIND_PROPERTY(BoxWidget, Color);
    class_<BoxWidget, SingleChildWidget>(widgets, "BoxWidget")
        .def(py::init())
        .def(py::init<unsigned>())
        .def_readonly("round", &BoxWidget::round, return_value_policy::reference_internal)
        .def_readonly("border_widget", &BoxWidget::border_width, return_value_policy::reference_internal)
        .def_readonly("border_color", &BoxWidget::border_color, return_value_policy::reference_internal)
        .def_readonly("background_color", &BoxWidget::background_color, return_value_policy::reference_internal);

    BIND_PROPERTY(SizedBoxWidget, SizeUnit);
    class_<SizedBoxWidget, SingleChildWidget>(widgets, "SizedBoxWidget")
        .def(py::init())
        .def(py::init<SizeUnit, SizeUnit>())
        .def_readonly("width", &SizedBoxWidget::width, return_value_policy::reference_internal)
        .def_readonly("height", &SizedBoxWidget::height, return_value_policy::reference_internal);

    BIND_PROPERTY(AlignWidget, float);
    class_<AlignWidget, SingleChildWidget>(widgets, "AlignWidget")
        .def(py::init())
        .def(py::init<float, float>())
        .def_readonly("align_x", &AlignWidget::align_x, return_value_policy::reference_internal)
        .def_readonly("align_y", &AlignWidget::align_y, return_value_policy::reference_internal);

    BIND_PROPERTY(MarginWidget, unsigned);
    class_<MarginWidget, SingleChildWidget>(widgets, "MarginWidget")
        .def(py::init())
        .def(py::init<unsigned>(), "margin"_a)
        .def(py::init<unsigned, unsigned>(), "x"_a, "y"_a)
        .def(py::init<unsigned, unsigned, unsigned, unsigned>(), "top"_a, "bottom"_a, "left"_a, "right"_a)
        .def_readonly("top", &MarginWidget::top, return_value_policy::reference_internal)
        .def_readonly("bottom", &MarginWidget::bottom, return_value_policy::reference_internal)
        .def_readonly("left", &MarginWidget::left, return_value_policy::reference_internal)
        .def_readonly("right", &MarginWidget::right, return_value_policy::reference_internal);
}

static void bind_layer_Widgets(py::module_& widgets) {
    BIND_PROPERTY(StackWidget, float);
    class_<StackWidget, MultiChildWidget>(widgets, "StackWidget")
        .def(py::init())
        .def(py::init<float, float>())
        .def_readonly("align_x", &StackWidget::align_x, return_value_policy::reference_internal)
        .def_readonly("align_y", &StackWidget::align_y, return_value_policy::reference_internal);

    BIND_PROPERTY(DeckWidget, unsigned);
    class_<DeckWidget, MultiChildWidget>(widgets, "DeckWidget")
        .def(py::init())
        .def(py::init<unsigned>())
        .def_readonly("selected", &DeckWidget::selected, return_value_policy::reference_internal);
}

static void bind_ScrollWidget(py::module_& widgets) {

    BIND_PROPERTY(ScrollWidget, Point);
    class_<ScrollWidget, SingleChildWidget>(widgets, "ScrollWidget")
        .def(py::init())
        .def_readonly("offset", &ScrollWidget::offset, return_value_policy::reference_internal)
        .def("widget_pos", &ScrollWidget::widget_pos)
        .def("widget_x", &ScrollWidget::widget_x)
        .def("widget_y", &ScrollWidget::widget_y)
        .def("widget_x_ratio", &ScrollWidget::widget_x_ratio)
        .def("widget_y_ratio", &ScrollWidget::widget_y_ratio)
        .def("scroll_by_px", &ScrollWidget::scroll_by_px)
        .def("scroll_by_ratio", &ScrollWidget::scroll_by_ratio)
        .def("scroll_x_by_px", &ScrollWidget::scroll_x_by_px)
        .def("scroll_y_by_px", &ScrollWidget::scroll_y_by_px)
        .def("scroll_x_by_ratio", &ScrollWidget::scroll_x_by_ratio)
        .def("scroll_y_by_ratio", &ScrollWidget::scroll_y_by_ratio)
        .def("scroll_to_px", &ScrollWidget::scroll_to_px)
        .def("scroll_to_ratio", &ScrollWidget::scroll_to_ratio)
        .def("scroll_x_to_px", &ScrollWidget::scroll_x_to_px)
        .def("scroll_y_to_px", &ScrollWidget::scroll_y_to_px)
        .def("scroll_x_to_ratio", &ScrollWidget::scroll_x_to_ratio)
        .def("scroll_y_to_ratio", &ScrollWidget::scroll_y_to_ratio);
}

static void bind_misc_widgets(py::module_& widgets) {
    BIND_PROPERTY(TextWidget, unsigned);
    class_<TextWidget, Widget>(widgets, "TextWidget")
        .def(py::init<std::string>())
        .def_readonly("font_size", &TextWidget::font_size, return_value_policy::reference_internal)
        .def("pos_to_index", &TextWidget::pos_to_index)
        .def("index_to_pos", &TextWidget::index_to_pos)
        .def("line_height", &TextWidget::line_height);

    class_<PointerListenerWidget, SingleChildWidget>(widgets, "PointerListenerWidget")
        .def(py::init())
        .def_readonly("on_up", &PointerListenerWidget::on_up, return_value_policy::reference_internal)
        .def_readonly("on_down", &PointerListenerWidget::on_down, return_value_policy::reference_internal)
        .def_readonly("on_click", &PointerListenerWidget::on_click, return_value_policy::reference_internal)
        .def_readonly("on_move", &PointerListenerWidget::on_move, return_value_policy::reference_internal)
        .def_readonly("on_drag", &PointerListenerWidget::on_drag, return_value_policy::reference_internal)
        .def_readonly("on_scroll", &PointerListenerWidget::on_scroll, return_value_policy::reference_internal)
        .def_readonly("on_hover", &PointerListenerWidget::on_hover, return_value_policy::reference_internal);
}

void bind_widgets(py::module_& m) {
    py::module_ widgets{"widgets", "RolUI widgets."};
    m.attr("widgets") = widgets;

    bind_signals(m);
    bind_container_widgets(widgets);
    bind_layer_Widgets(widgets);
    bind_ScrollWidget(widgets);
    bind_misc_widgets(widgets);

    widgets.def("sized", static_cast<Widget* (*)(int, int, Widget*)>(widgets::sized),
                return_value_policy::reference);
    widgets.def("sized", static_cast<Widget* (*)(int, float, Widget*)>(widgets::sized),
                return_value_policy::reference);
    widgets.def("sized", static_cast<Widget* (*)(float, int, Widget*)>(widgets::sized),
                return_value_policy::reference);
    widgets.def("sized", static_cast<Widget* (*)(float, float, Widget*)>(widgets::sized),
                return_value_policy::reference);

    widgets.def("image", RolUI::widgets::image, return_value_policy::reference);

    typedef Widget* (*ButtonFuncType1)(const char*, std::function<void()>, unsigned, Color, Color, Color, Color, unsigned);
    typedef Widget* (*ButtonFuncType2)(Widget*, Widget*, Widget*, std::function<void()>);
    widgets.def("button", static_cast<ButtonFuncType1>(widgets::button),
                py::arg("str"), py::arg("callback"), "text_size"_a = 16, "text_color"_a = Color{0, 0, 0},
                "normal"_a = Color{240, 240, 240}, "hover"_a = Color{230, 230, 230}, "press"_a = Color{250, 250, 250},
                "round"_a = 0, return_value_policy::reference);
    widgets.def("button", static_cast<ButtonFuncType2>(widgets::button),
                "normal"_a, "hover"_a, "press"_a, "callback"_a, return_value_policy::reference);
}
