
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
        .def(py::init<unsigned>(), py::arg("margin"))
        .def(py::init<unsigned, unsigned>(), py::arg("x"), py::arg("y"))
        .def(py::init<unsigned, unsigned, unsigned, unsigned>(),
             py::arg("top"), py::arg("bottom"), py::arg("left"), py::arg("right"))
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

static void bind_flow_widgets(py::module_& widgets) {

    enum_<CrossAxisAlignment>(widgets, "CrossAxisAlignment")
        .value("start", CrossAxisAlignment::start)
        .value("center", CrossAxisAlignment::center)
        .value("end", CrossAxisAlignment::end)
        .export_values();

    BIND_PROPERTY(ColumnWidget, CrossAxisAlignment);
    BIND_PROPERTY(ColumnWidget, int);
    class_<ColumnWidget, MultiChildWidget>(widgets, "ColumnWidget")
        .def(py::init())
        .def_readonly("cross_axis_alignment", &ColumnWidget::cross_axis_alignment,
                      return_value_policy::reference_internal)
        .def_readonly("gap", &ColumnWidget::gap,
                      return_value_policy::reference_internal);

    BIND_PROPERTY(RowWidget, CrossAxisAlignment);
    BIND_PROPERTY(RowWidget, int);
    class_<RowWidget, MultiChildWidget>(widgets, "RowWidget")
        .def(py::init())
        .def_readonly("cross_axis_alignment", &RowWidget::cross_axis_alignment,
                      return_value_policy::reference_internal)
        .def_readonly("gap", &RowWidget::gap,
                      return_value_policy::reference_internal);

    class_<ColumnGridWidget, MultiChildWidget>(widgets, "ColumnGridWidget")
        .def(py::init())
        .def("flex_of", &ColumnGridWidget::flex_of)
        .def("flex_sum", &ColumnGridWidget::flex_sum)
        .def("add_child", &ColumnGridWidget::add_child, return_value_policy::reference)
        .def("set_child", &ColumnGridWidget::set_child, return_value_policy::reference)
        .def("remove_child", static_cast<void (ColumnGridWidget::*)(Widget*)>(&ColumnGridWidget::remove_child))
        .def("remove_child", static_cast<void (ColumnGridWidget::*)(int)>(&ColumnGridWidget::remove_child));
    class_<RowGridWidget, MultiChildWidget>(widgets, "RowGridWidget")
        .def(py::init())
        .def("flex_of", &RowGridWidget::flex_of)
        .def("flex_sum", &RowGridWidget::flex_sum)
        .def("add_child", &RowGridWidget::add_child, return_value_policy::reference)
        .def("set_child", &RowGridWidget::set_child, return_value_policy::reference)
        .def("remove_child", static_cast<void (RowGridWidget::*)(Widget*)>(&RowGridWidget::remove_child))
        .def("remove_child", static_cast<void (RowGridWidget::*)(int)>(&RowGridWidget::remove_child));
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

static void bind_widgets_widgets(py::module_& widgets) {

    widgets.def("text", widgets::text, py::arg("text"),
                py::arg("size") = 16, py::arg("color") = Color{0, 0, 0},
                return_value_policy::reference_internal);

    widgets.def("label", widgets::label,
                py::arg("text"), py::arg("size") = 16, py::arg("color") = Color{0, 0, 0},
                py::arg("background_color") = Color{255, 255, 255}, py::arg("round") = 0,
                return_value_policy::reference);

    widgets.def("image", widgets::image, py::arg("image"), return_value_policy::reference);

    typedef Widget* (*ButtonFuncType1)(const char*, std::function<void()>, unsigned, Color, Color, Color, Color, unsigned);
    typedef Widget* (*ButtonFuncType2)(Widget*, Widget*, Widget*, std::function<void()>);
    widgets.def("button", static_cast<ButtonFuncType1>(widgets::button),
                py::arg("text"), py::arg("callback"), py::arg("text_size") = 16, py::arg("text_color") = Color{0, 0, 0},
                py::arg("normal") = Color{240, 240, 240}, py::arg("hover") = Color{230, 230, 230}, py::arg("press") = Color{250, 250, 250},
                py::arg("round") = 0, return_value_policy::reference);
    widgets.def("button", static_cast<ButtonFuncType2>(widgets::button),
                py::arg("normal"), py::arg("hover"), py::arg("press"), py::arg("callback"), return_value_policy::reference);

    widgets.def(
        "box", [](Widget* c, unsigned r, unsigned border_w, Color border_c, Color bc) {
            return widgets::box(r, border_w, border_c, bc, c);
        },
        py::arg("child"), py::arg("round") = 0, py::arg("border_width") = 0, py::arg("border_color") = Color{0, 0, 0}, py::arg("background_color") = Color{0, 0, 0, 0}, return_value_policy::reference);

    widgets.def(
        "align", [](Widget* c, float x, float y) {
            return widgets::align(x, y, c);
        },
        py::arg("child"), py::arg("x") = 0.5f, py::arg("y") = 0.5f, return_value_policy::reference);

    auto _sized = [](Widget* c, py::object w, py::object h) {
        SizeUnit sw, sh;
        SizedBoxWidget* widget = nullptr;
        if (py::isinstance<py::int_>(w) && py::isinstance<py::int_>(h))
            widget = widgets::sized(w.cast<int>(), h.cast<int>(), c);
        else if (py::isinstance<py::int_>(w) && py::isinstance<py::float_>(h))
            widget = widgets::sized(w.cast<int>(), h.cast<float>(), c);
        else if (py::isinstance<py::float_>(w) && py::isinstance<py::int_>(h))
            widget = widgets::sized(w.cast<float>(), h.cast<int>(), c);
        else if (py::isinstance<py::float_>(w) && py::isinstance<py::float_>(h))
            widget = widgets::sized(w.cast<float>(), h.cast<float>(), c);
        else
            throw py::value_error("w and h must be of type int or float.");
        return widget;
    };

    widgets.def("sized", _sized, py::arg("child"),
                py::arg("width") = 1.0f, py::arg("height") = 1.0f,
                return_value_policy::reference);

    widgets.def("margin", static_cast<MarginWidget* (*)(unsigned, Widget*)>(widgets::margin),
                py::arg("margin"), py::arg("child"),
                return_value_policy::reference);
    widgets.def("margin", static_cast<MarginWidget* (*)(unsigned, unsigned, Widget*)>(widgets::margin),
                py::arg("x"), py::arg("y"), py::arg("child"),
                return_value_policy::reference);
    widgets.def("margin", static_cast<MarginWidget* (*)(unsigned, unsigned, unsigned, unsigned, Widget*)>(widgets::margin),
                py::arg("top"), py::arg("right"), py::arg("bottom"), py::arg("left"), py::arg("child"),
                return_value_policy::reference);

    widgets.def(
        "stack", [](py::args args) {
            StackWidget* widget = widgets::stack();
            for (auto w : args)
                widget->add_child(w.cast<Widget*>());
            return widget;
        },
        return_value_policy::reference);
    widgets.def(
        "deck", [](py::args args) {
            DeckWidget* widget = widgets::deck();
            for (auto w : args)
                widget->add_child(w.cast<Widget*>());
            return widget;
        },
        return_value_policy::reference);

    widgets.def(
        "row", [](py::args args, int gap, CrossAxisAlignment caa) {
            RowWidget* widget = widgets::row();
            widget->gap(gap)->cross_axis_alignment(caa);
            for (auto w : args)
                widget->add_child(w.cast<Widget*>());
            return widget;
        },
        py::kw_only(), py::arg("gap") = 0, py::arg("cross_axis_alignment") = CrossAxisAlignment::center, return_value_policy::reference);

    widgets.def(
        "column", [](py::args args, int gap, CrossAxisAlignment caa) {
            ColumnWidget* widget = widgets::column();
            widget->gap(gap)->cross_axis_alignment(caa);
            for (auto w : args)
                widget->add_child(w.cast<Widget*>());
            return widget;
        },
        py::kw_only(), py::arg("gap") = 0, py::arg("cross_axis_alignment") = CrossAxisAlignment::center, return_value_policy::reference);

    widgets.def(
        "row_grid", [](py::args args, py::tuple flexs) {
            RowGridWidget* widget = widgets::row_grid();
            if (flexs.size() >= args.size()) {
                for (int i = 0; i < args.size(); i++)
                    widget->add_child(args[i].cast<Widget*>(), flexs[i].cast<float>());
            } else {
                int i = 0;
                for (; i < flexs.size(); i++)
                    widget->add_child(args[i].cast<Widget*>(), flexs[i].cast<float>());
                for (; i < args.size(); i++)
                    widget->add_child(args[i].cast<Widget*>(), 1.0f);
            }
            return widget;
        },
        py::kw_only(), py::arg("flexs") = py::tuple(), return_value_policy::reference);
    widgets.def(
        "column_grid", [](py::args args, py::tuple flexs) {
            ColumnGridWidget* widget = widgets::column_grid();
            if (flexs.size() >= args.size()) {
                for (int i = 0; i < args.size(); i++)
                    widget->add_child(args[i].cast<Widget*>(), flexs[i].cast<float>());
            } else {
                int i = 0;
                for (; i < flexs.size(); i++)
                    widget->add_child(args[i].cast<Widget*>(), flexs[i].cast<float>());
                for (; i < args.size(); i++)
                    widget->add_child(args[i].cast<Widget*>(), 1.0f);
            }
            return widget;
        },
        py::kw_only(), py::arg("flexs") = py::tuple(), return_value_policy::reference);

    widgets.def("pointer_listener", widgets::pointer_listener,
                py::arg("child"), return_value_policy::reference);
}

void bind_widgets(py::module_& m) {
    py::module_ widgets{"widgets", "RolUI widgets."};
    m.attr("widgets") = widgets;

    bind_signals(m);
    bind_container_widgets(widgets);
    bind_layer_Widgets(widgets);
    bind_flow_widgets(widgets);
    bind_ScrollWidget(widgets);
    bind_misc_widgets(widgets);
    bind_widgets_widgets(widgets);
}
