
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

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

#include <RolUI/sigslot.hpp>

#include <RolUI/widgets/Text.hpp>
#include <RolUI/widgets/container.hpp>
#include <RolUI/widgets/flow.hpp>
#include <RolUI/widgets/layer.hpp>
#include <RolUI/widgets/listener.hpp>
#include <RolUI/widgets/decoration.hpp>
#include <RolUI/widgets/widgets.hpp>

using namespace pybind11;
using namespace RolUI;
using namespace RolUI::widgets;

namespace py = pybind11;

#define BIND_SINGAL(mod, ...)                                                                           \
    {                                                                                                   \
        typedef Signal<__VA_ARGS__> Sign;                                                               \
        if (!py::hasattr(mod, "Signal<" #__VA_ARGS__ ">"))                                              \
            class_<Sign>(mod, "Signal<" #__VA_ARGS__ ">")                                               \
                .def(py::init())                                                                        \
                .def("slot_count", &Sign::slot_count)                                                   \
                .def("connect", [](Sign& self, std::function<void(__VA_ARGS__)> f) {                    \
                    return self.connect(std::move(f));                                                  \
                })                                                                                      \
                .def("disconnect", static_cast<void (Sign::*)(unsigned)>(&Sign::disconnect))            \
                .def("disconnect_all", &Sign::disconnect_all)                                           \
                .def("emit", static_cast<void (Sign::*)(__VA_ARGS__) const noexcept>(&Sign::emit))      \
                .def("__call__", static_cast<void (Sign::*)(__VA_ARGS__) const noexcept>(&Sign::emit)); \
    }

#define BIND_PROPERTY(mod, smod, WT, VT)                                                              \
    {                                                                                                 \
        typedef Property<WT, VT> Prop;                                                                \
        if (!py::hasattr(mod, "Property<" #WT ", " #VT ">"))                                          \
            class_<Prop>(mod, "Property<" #WT ", " #VT ">")                                           \
                .def_readonly("on_change", &Prop::on_change, return_value_policy::reference_internal) \
                .def("get", [](Prop& self) { return self.get(); })                                    \
                .def(                                                                                 \
                    "set", [](Prop& self, VT arg) { return self.set(arg); },                          \
                    return_value_policy::reference)                                                   \
                .def(                                                                                 \
                    "set_no_notify", [](Prop& self, VT arg) { return self.set_no_notify(arg); },      \
                    return_value_policy::reference)                                                   \
                .def("__call__", [](Prop& self) { return self(); })                                   \
                .def(                                                                                 \
                    "__call__", [](Prop& self, VT arg) { return self(arg); },                         \
                    return_value_policy::reference);                                                  \
    };                                                                                                \
    BIND_SINGAL(smod, const VT&)

static void bind_container_widgets(py::module_& widgets, py::module_& signals, py::module_& propertys) {
    enum_<SizeUnitType>(widgets, "SizeUnitType")
        .value("pixel", SizeUnitType::pixel)
        .value("percentage", SizeUnitType::percentage)
        .export_values();
    class_<SizeUnit>(widgets, "SizeUnit")
        .def(py::init())
        .def(py::init<int>())
        .def(py::init<float>())
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def("type", &SizeUnit::type)
        .def("pixel", &SizeUnit::pixel)
        .def("percentage", &SizeUnit::percentage);

    BIND_PROPERTY(propertys, signals, BoxWidget, unsigned);
    BIND_PROPERTY(propertys, signals, BoxWidget, Color);
    class_<BoxWidget, SingleChildWidget>(widgets, "BoxWidget")
        .def(py::init())
        .def_readonly("round", &BoxWidget::round, return_value_policy::reference_internal)
        .def_readonly("border_width", &BoxWidget::border_width, return_value_policy::reference_internal)
        .def_readonly("border_color", &BoxWidget::border_color, return_value_policy::reference_internal)
        .def_readonly("background_color", &BoxWidget::background_color, return_value_policy::reference_internal);

    BIND_PROPERTY(propertys, signals, SizedWidget, SizeUnit);
    class_<SizedWidget, SingleChildWidget>(widgets, "SizedWidget")
        .def(py::init())
        .def(py::init<SizeUnit, SizeUnit>())
        .def_readonly("width", &SizedWidget::width, return_value_policy::reference_internal)
        .def_readonly("height", &SizedWidget::height, return_value_policy::reference_internal);

    BIND_PROPERTY(propertys, signals, AlignWidget, float);
    class_<AlignWidget, SingleChildWidget>(widgets, "AlignWidget")
        .def(py::init())
        .def(py::init<float, float>())
        .def_readonly("align_x", &AlignWidget::align_x, return_value_policy::reference_internal)
        .def_readonly("align_y", &AlignWidget::align_y, return_value_policy::reference_internal);

    BIND_PROPERTY(propertys, signals, MarginWidget, unsigned);
    class_<MarginWidget, SingleChildWidget>(widgets, "MarginWidget")
        .def(py::init())
        .def(py::init<unsigned>(), py::arg("margin"))
        .def(py::init<unsigned, unsigned>(), py::arg("x"), py::arg("y"))
        .def(py::init<unsigned, unsigned, unsigned, unsigned>(),
             py::arg("top"), py::arg("bottom"), py::arg("left"), py::arg("right"))
        .def_readonly("top", &MarginWidget::top, return_value_policy::reference_internal)
        .def_readonly("bottom", &MarginWidget::bottom, return_value_policy::reference_internal)
        .def_readonly("left", &MarginWidget::left, return_value_policy::reference_internal)
        .def_readonly("right", &MarginWidget::right, return_value_policy::reference_internal);
}

static void bind_layer_Widgets(py::module_& widgets, py::module_& signals, py::module_& propertys) {
    BIND_PROPERTY(propertys, signals, StackWidget, float);
    class_<StackWidget, MultiChildWidget>(widgets, "StackWidget")
        .def(py::init())
        .def(py::init<float, float>())
        .def_readonly("align_x", &StackWidget::align_x, return_value_policy::reference_internal)
        .def_readonly("align_y", &StackWidget::align_y, return_value_policy::reference_internal);

    BIND_PROPERTY(propertys, signals, DeckWidget, unsigned);
    class_<DeckWidget, MultiChildWidget>(widgets, "DeckWidget")
        .def(py::init())
        .def(py::init<unsigned>())
        .def_readonly("selected", &DeckWidget::selected, return_value_policy::reference_internal);
}

static void bind_flow_widgets(py::module_& widgets, py::module_& signals, py::module_& propertys) {

    BIND_PROPERTY(propertys, signals, ColumnWidget, float);
    BIND_PROPERTY(propertys, signals, ColumnWidget, int);
    class_<ColumnWidget, MultiChildWidget>(widgets, "ColumnWidget")
        .def(py::init())
        .def_readonly("cross_axis_alignment", &ColumnWidget::cross_axis_alignment,
                      return_value_policy::reference_internal)
        .def_readonly("gap", &ColumnWidget::gap,
                      return_value_policy::reference_internal);

    BIND_PROPERTY(propertys, signals, RowWidget, float);
    BIND_PROPERTY(propertys, signals, RowWidget, int);
    class_<RowWidget, MultiChildWidget>(widgets, "RowWidget")
        .def(py::init())
        .def_readonly("cross_axis_alignment", &RowWidget::cross_axis_alignment,
                      return_value_policy::reference_internal)
        .def_readonly("gap", &RowWidget::gap,
                      return_value_policy::reference_internal);

    enum_<FlexFit>(widgets, "FlexFit")
        .value("fixed", FlexFit::fixed)
        .value("percentage", FlexFit::percentage)
        .value("flex", FlexFit::flex)
        .value("expand", FlexFit::expand);

    BIND_PROPERTY(propertys, signals, FlexableWidget, FlexFit);
    BIND_PROPERTY(propertys, signals, FlexableWidget, float);
    BIND_PROPERTY(propertys, signals, FlexableWidget, int);
    class_<FlexableWidget, SingleChildWidget>(widgets, "FlexableWidget")
        .def(py::init())
        .def_readonly("fit", &FlexableWidget::fit, return_value_policy::reference_internal)
        .def_readonly("flex", &FlexableWidget::flex, return_value_policy::reference_internal)
        .def_readonly("percentage", &FlexableWidget::percentage, return_value_policy::reference_internal)
        .def_readonly("fixed", &FlexableWidget::fixed, return_value_policy::reference_internal);

    class_<ColumnGridWidget, MultiChildWidget>(widgets, "ColumnGridWidget")
        .def(py::init());
    class_<RowGridWidget, MultiChildWidget>(widgets, "RowGridWidget")
        .def(py::init());

    enum_<Direction>(widgets, "Direction")
        .value("row", Direction::row)
        .value("column", Direction::column)
        .value("row_reverse", Direction::row_reverse)
        .value("column_reverse", Direction::column_reverse);

    BIND_PROPERTY(propertys, signals, FlexWidget, float);
    BIND_PROPERTY(propertys, signals, FlexWidget, Direction);
    class_<FlexWidget, MultiChildWidget>(widgets, "FlexWidget")
        .def_readonly("cross_axis_alignment", &FlexWidget::cross_axis_alignment, return_value_policy::reference_internal)
        .def_readonly("direction", &FlexWidget::direction, return_value_policy::reference_internal);
}

static void bind_listener_widgets(py::module_& widgets, py::module_& signals, py::module_& propertys) {
    BIND_SINGAL(signals, Vec2i);
    BIND_SINGAL(signals, bool);
    BIND_SINGAL(signals, unsigned);
    class_<PointerListener, SingleChildWidget>(widgets, "PointerListener")
        .def(py::init())
        .def_readonly("on_up", &PointerListener::on_up, return_value_policy::reference_internal)
        .def_readonly("on_down", &PointerListener::on_down, return_value_policy::reference_internal)
        .def_readonly("on_click", &PointerListener::on_click, return_value_policy::reference_internal)
        .def_readonly("on_move", &PointerListener::on_move, return_value_policy::reference_internal)
        .def_readonly("on_drag", &PointerListener::on_drag, return_value_policy::reference_internal)
        .def_readonly("on_hover", &PointerListener::on_hover, return_value_policy::reference_internal);

    BIND_SINGAL(signals, MouseKey, Vec2i);
    class_<MouseListener, SingleChildWidget>(widgets, "MouseListener")
        .def(py::init())
        .def_readonly("on_up", &MouseListener::on_up, return_value_policy::reference_internal)
        .def_readonly("on_down", &MouseListener::on_down, return_value_policy::reference_internal)
        .def_readonly("on_click", &MouseListener::on_click, return_value_policy::reference_internal)
        .def_readonly("on_move", &MouseListener::on_move, return_value_policy::reference_internal)
        .def_readonly("on_drag", &MouseListener::on_drag, return_value_policy::reference_internal)
        .def_readonly("on_hover", &MouseListener::on_hover, return_value_policy::reference_internal)
        .def_readonly("on_wheel", &MouseListener::on_wheel, return_value_policy::reference_internal);

    BIND_SINGAL(signals, bool);
    class_<FocusListener, SingleChildWidget>(widgets, "FocusListener")
        .def(py::init())
        .def("focus", &FocusListener::focus)
        .def("unfocus", &FocusListener::unfocus)
        .def_readonly("on_focus", &FocusListener::on_focus, return_value_policy::reference_internal);

    BIND_SINGAL(signals, unsigned);
    class_<CharInputListener, SingleChildWidget>(widgets, "CharInputListener")
        .def(py::init())
        .def_readonly("on_input", &CharInputListener::on_input, return_value_policy::reference_internal);

    BIND_SINGAL(signals, KeyboardKey, KeyboardKeyMode);
    class_<KeyboardListener, SingleChildWidget>(widgets, "KeyboardListener")
        .def(py::init())
        .def_readonly("on_key", &KeyboardListener::on_key, return_value_policy::reference_internal);
}

static void bind_scroll_widgets(py::module_& widgets, py::module_& signals, py::module_& propertys) {
    BIND_PROPERTY(propertys, signals, ScrollView, Point);
    BIND_SINGAL(signals, Vec2i);
    class_<ScrollView, SingleChildWidget>(widgets, "ScrollView")
        .def(py::init())
        .def_readonly("offset", &ScrollView::offset, return_value_policy::reference_internal)
        .def_readonly("on_scroll", &ScrollView::on_scroll, return_value_policy::reference_internal)
        .def("widget_x_ratio", &ScrollView::widget_x_ratio)
        .def("widget_y_ratio", &ScrollView::widget_y_ratio)
        .def("scroll_by_px", &ScrollView::scroll_by_px)
        .def("scroll_by_ratio", &ScrollView::scroll_by_ratio)
        .def("scroll_x_by_px", &ScrollView::scroll_x_by_px)
        .def("scroll_y_by_px", &ScrollView::scroll_y_by_px)
        .def("scroll_x_by_ratio", &ScrollView::scroll_x_by_ratio)
        .def("scroll_y_by_ratio", &ScrollView::scroll_y_by_ratio)
        .def("scroll_to_px", &ScrollView::scroll_to_px)
        .def("scroll_to_ratio", &ScrollView::scroll_to_ratio)
        .def("scroll_x_to_px", &ScrollView::scroll_x_to_px)
        .def("scroll_y_to_px", &ScrollView::scroll_y_to_px)
        .def("scroll_x_to_ratio", &ScrollView::scroll_x_to_ratio)
        .def("scroll_y_to_ratio", &ScrollView::scroll_y_to_ratio);

    BIND_PROPERTY(propertys, signals, VScrollView, float);
    class_<VScrollView, ScrollView>(widgets, "VScrollView")
        .def(py::init())
        .def_readonly("scroll_step", &VScrollView::scroll_step, py::return_value_policy::reference_internal);
    BIND_PROPERTY(propertys, signals, HScrollView, float);
    class_<HScrollView, ScrollView>(widgets, "HScrollView")
        .def(py::init())
        .def_readonly("scroll_step", &HScrollView::scroll_step, py::return_value_policy::reference_internal);
}

static void bind_text_widgets(py::module_& widgets, py::module_& signals, py::module_& propertys) {

    BIND_PROPERTY(propertys, signals, TextWidget, int);
    BIND_PROPERTY(propertys, signals, TextWidget, unsigned);
    BIND_PROPERTY(propertys, signals, TextWidget, Color);
    BIND_PROPERTY(propertys, signals, TextWidget, std::string);
    class_<TextWidget, Widget>(widgets, "TextWidget")
        .def(py::init())
        .def(py::init<std::string>())
        .def_readonly("font", &TextWidget::font, return_value_policy::reference_internal)
        .def_readonly("text_size", &TextWidget::text_size, return_value_policy::reference_internal)
        .def_readonly("text_color", &TextWidget::text_color, return_value_policy::reference_internal)
        .def_readonly("text", &TextWidget::text, return_value_policy::reference_internal)
        .def("pos_to_index", &TextWidget::pos_to_index)
        .def("index_to_pos", &TextWidget::index_to_pos)
        .def("char_count", &TextWidget::char_count)
        .def("char_index_to_byte_index", &TextWidget::char_index_to_byte_index)
        .def("char_byte_size", &TextWidget::char_byte_size)
        .def("byte_index_to_char_index", &TextWidget::byte_index_to_char_index)
        .def("line_height", &TextWidget::line_height);

    BIND_PROPERTY(propertys, signals, EditableTextWidget, unsigned);
    class_<EditableTextWidget, TextWidget>(widgets, "EditableTextWidget")
        .def(py::init())
        .def_readonly("cursor_index", &EditableTextWidget::cursor_index, return_value_policy::reference_internal)
        .def("is_blinking", &EditableTextWidget::is_blinking)
        .def("set_blink", &EditableTextWidget::set_blink)
        .def("delete_front", &EditableTextWidget::delete_front)
        .def("delete_back", &EditableTextWidget::delete_back)
        .def("insert_char", &EditableTextWidget::insert_char, py::arg("index"), py::arg("codepoint"))
        .def("insert_str",
             static_cast<void (EditableTextWidget::*)(unsigned, const char*)>(&EditableTextWidget::insert_str),
             py::arg("index"), py::arg("str"))
        .def("insert_str", static_cast<void (EditableTextWidget::*)(unsigned, const char*, unsigned)>(&EditableTextWidget::insert_str),
             py::arg("index"), py::arg("str"), py::arg("len"));

    class_<TextBoxWidget, EditableTextWidget>(widgets, "TextBoxWidget")
        .def(py::init());
}
static void bind_image_widgets(py::module_& widgets, py::module_& signals, py::module_& propertys) {
    BIND_PROPERTY(propertys, signals, ImageWidget, RolUI::Image);
    BIND_PROPERTY(propertys, signals, ImageWidget, ImageWidget::Fit);
    auto py_image_widget_class =
        class_<ImageWidget, Widget>(widgets, "ImageWidget")
            .def_readonly("image", &ImageWidget::image, return_value_policy::reference_internal)
            .def_readonly("fit", &ImageWidget::fit, return_value_policy::reference_internal)
            .def(py::init())
            .def(py::init<RolUI::Image, ImageWidget::Fit>());

    enum_<ImageWidget::Fit>(py_image_widget_class, "Fit")
        .value("fill", ImageWidget::fill)
        .value("cover", ImageWidget::cover)
        .value("contain", ImageWidget::contain)
        .export_values();
}

static void bind_decoration_widgets(py::module_& widgets, py::module_& signals, py::module_& propertys) {
    BIND_PROPERTY(propertys, signals, HSeparatorWidget, Color);
    BIND_PROPERTY(propertys, signals, HSeparatorWidget, int);
    class_<HSeparatorWidget, Widget>(widgets, "HSeparatorWidget")
        .def_readonly("color", &HSeparatorWidget::color, return_value_policy::reference_internal)
        .def_readonly("width", &HSeparatorWidget::width, return_value_policy::reference_internal)
        .def(py::init());

    BIND_PROPERTY(propertys, signals, VSeparatorWidget, Color);
    BIND_PROPERTY(propertys, signals, VSeparatorWidget, int);
    class_<VSeparatorWidget, Widget>(widgets, "VSeparatorWidget")
        .def_readonly("color", &VSeparatorWidget::color, return_value_policy::reference_internal)
        .def_readonly("width", &VSeparatorWidget::width, return_value_policy::reference_internal)
        .def(py::init());
}

static void bind_widgets_widgets(py::module_& widgets, py::module_& signals, py::module_& propertys) {
}

void bind_widgets(py::module_& m) {
    py::module_ widgets = m.def_submodule("widgets", "RolUI widgets.");
    py::module_ signals = m.def_submodule("signals", "RolUI signals.");
    py::module_ propertys = m.def_submodule("propertys", "RolUI propertys.");

    bind_container_widgets(widgets, signals, propertys);
    bind_layer_Widgets(widgets, signals, propertys);
    bind_flow_widgets(widgets, signals, propertys);
    bind_listener_widgets(widgets, signals, propertys);
    bind_scroll_widgets(widgets, signals, propertys);
    bind_text_widgets(widgets, signals, propertys);
    bind_image_widgets(widgets, signals, propertys);
    bind_decoration_widgets(widgets, signals, propertys);
    bind_widgets_widgets(widgets, signals, propertys);
}
