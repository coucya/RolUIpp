
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
#include <RolUI/widgets/listener.hpp>
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
                .def("disconnect", static_cast<void (Sign::*)(size_t)>(&Sign::disconnect))              \
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

    BIND_PROPERTY(propertys, signals, SizedBoxWidget, SizeUnit);
    class_<SizedBoxWidget, SingleChildWidget>(widgets, "SizedBoxWidget")
        .def(py::init())
        .def(py::init<SizeUnit, SizeUnit>())
        .def_readonly("width", &SizedBoxWidget::width, return_value_policy::reference_internal)
        .def_readonly("height", &SizedBoxWidget::height, return_value_policy::reference_internal);

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

static void bind_listener_widgets(py::module_& widgets, py::module_& signals, py::module_& propertys) {
    BIND_SINGAL(signals, Point);
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

    class_<FocusListener, SingleChildWidget>(widgets, "FocusListener")
        .def(py::init())
        .def("focus", &FocusListener::focus)
        .def("unfocus", &FocusListener::unfocus)
        .def_readonly("on_focus", &FocusListener::on_focus, return_value_policy::reference_internal);

    class_<CharInputListener, SingleChildWidget>(widgets, "CharInputListener")
        .def(py::init())
        .def_readonly("on_input", &CharInputListener::on_input, return_value_policy::reference_internal);
}

static void bind_ScrollWidget(py::module_& widgets, py::module_& signals, py::module_& propertys) {
    BIND_PROPERTY(propertys, signals, ScrollView, Point);
    class_<ScrollView, SingleChildWidget>(widgets, "ScrollView")
        .def(py::init())
        .def_readonly("offset", &ScrollView::offset, return_value_policy::reference_internal)
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

    class_<VScrollView, ScrollView>(widgets, "VScrollView")
        .def(py::init())
        .def_readonly("scroll_step", &VScrollView::scroll_step, py::return_value_policy::reference_internal);
    class_<HScrollView, ScrollView>(widgets, "HScrollView")
        .def(py::init())
        .def_readonly("scroll_step", &HScrollView::scroll_step, py::return_value_policy::reference_internal);
}

static void bind_misc_widgets(py::module_& widgets, py::module_& signals, py::module_& propertys) {
    BIND_PROPERTY(propertys, signals, TextWidget, unsigned);
    BIND_PROPERTY(propertys, signals, TextWidget, Color);
    BIND_PROPERTY(propertys, signals, TextWidget, std::string);
    class_<TextWidget, Widget>(widgets, "TextWidget")
        .def(py::init())
        .def(py::init<std::string>())
        .def_readonly("font_size", &TextWidget::font_size, return_value_policy::reference_internal)
        .def_readonly("font_color", &TextWidget::font_color, return_value_policy::reference_internal)
        .def_readonly("font_name", &TextWidget::font_name, return_value_policy::reference_internal)
        .def_readonly("text", &TextWidget::text, return_value_policy::reference_internal)
        .def("pos_to_index", &TextWidget::pos_to_index)
        .def("index_to_pos", &TextWidget::index_to_pos)
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

static void bind_widgets_widgets(py::module_& widgets, py::module_& signals, py::module_& propertys) {

    widgets.def("text", widgets::text, py::arg("text"),
                py::kw_only(), py::arg("size") = 16, py::arg("color") = Color{0, 0, 0},
                return_value_policy::reference);

    widgets.def(
        "textbox", [](const char* text, int size, Color color) {
            return mk_widget<TextBoxWidget>()
                ->text(text)
                ->font_size(size)
                ->font_color(color);
        },
        py::kw_only(), py::arg("text") = "", py::arg("font_size") = 16, py::arg("color") = Color{0, 0, 0}, py::return_value_policy::reference);

    widgets.def("label", widgets::label, py::arg("text"),
                py::kw_only(), py::arg("size") = 16, py::arg("color") = Color{0, 0, 0},
                py::arg("background_color") = Color{255, 255, 255}, py::arg("round") = 0,
                return_value_policy::reference);

    widgets.def("image", widgets::image, py::arg("image"), return_value_policy::reference);

    typedef Widget* (*ButtonFuncType1)(const char*, std::function<void()>, unsigned, Color, Color, Color, Color, unsigned);
    typedef Widget* (*ButtonFuncType2)(Widget*, Widget*, Widget*, std::function<void()>);
    widgets.def("button", static_cast<ButtonFuncType1>(widgets::button), py::arg("text"),
                py::kw_only(), py::arg("callback"), py::arg("text_size") = 16, py::arg("text_color") = Color{0, 0, 0},
                py::arg("normal") = Color{240, 240, 240}, py::arg("hover") = Color{230, 230, 230}, py::arg("press") = Color{250, 250, 250},
                py::arg("round") = 0, return_value_policy::reference);
    widgets.def("button", static_cast<ButtonFuncType2>(widgets::button),
                py::kw_only(), py::arg("normal"), py::arg("hover"), py::arg("press"), py::arg("callback"), return_value_policy::reference);

    widgets.def(
        "box", [](Widget* c, unsigned r, unsigned border_w, Color border_c, Color bc) {
            return widgets::box(r, border_w, border_c, bc, c);
        },
        py::kw_only(), py::arg("child"), py::arg("round") = 0, py::arg("border_width") = 0, py::arg("border_color") = Color{0, 0, 0}, py::arg("background_color") = Color{0, 0, 0, 0}, return_value_policy::reference);

    widgets.def(
        "align", [](Widget* c, float x, float y) {
            return widgets::align(x, y, c);
        },
        py::kw_only(), py::arg("child"), py::arg("x") = 0.0f, py::arg("y") = 0.0f, return_value_policy::reference);

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

    widgets.def("sized", _sized,
                py::kw_only(), py::arg("child"),
                py::arg("width") = 1.0f, py::arg("height") = 1.0f,
                return_value_policy::reference);

    widgets.def("margin", static_cast<MarginWidget* (*)(unsigned, Widget*)>(widgets::margin),
                py::kw_only(), py::arg("margin") = 0, py::arg("child"),
                return_value_policy::reference);
    widgets.def("margin", static_cast<MarginWidget* (*)(unsigned, unsigned, Widget*)>(widgets::margin),
                py::kw_only(), py::arg("x") = 0, py::arg("y") = 0, py::arg("child"),
                return_value_policy::reference);
    widgets.def("margin", static_cast<MarginWidget* (*)(unsigned, unsigned, unsigned, unsigned, Widget*)>(widgets::margin),
                py::kw_only(), py::arg("top") = 0, py::arg("right") = 0, py::arg("bottom") = 0, py::arg("left") = 0, py::arg("child"),
                return_value_policy::reference);

    widgets.def(
        "vscroll_view", [](Widget* child, float step) {
            return mk_widget<VScrollView>()
                ->scroll_step(step)
                ->set_child(child);
        },
        py::kw_only(), py::arg("child"), py::arg("step") = 10.0, return_value_policy::reference);
    widgets.def(
        "hscroll_view", [](Widget* child, float step) {
            return mk_widget<HScrollView>()
                ->scroll_step(step)
                ->set_child(child);
        },
        py::kw_only(), py::arg("child"), py::arg("step") = 10.0, return_value_policy::reference);

    widgets.def(
        "stack", [](py::list children, float align_x, float align_y) {
            StackWidget* widget = widgets::stack();
            for (auto w : children)
                widget->add_child(w.cast<Widget*>());
            widget->align_x(align_x);
            widget->align_y(align_y);
            return widget;
        },
        py::kw_only(), py::arg("children") = py::list(), py::arg("align_x") = 0.5f, py::arg("align_y") = 0.5f, return_value_policy::reference);
    widgets.def(
        "deck", [](py::list children, int selected) {
            DeckWidget* widget = widgets::deck();
            for (auto w : children)
                widget->add_child(w.cast<Widget*>());
            widget->selected(selected);
            return widget;
        },
        py::kw_only(), py::arg("children") = py::list(), py::arg("selected") = 0, return_value_policy::reference);

    widgets.def(
        "row", [](py::list children, int gap, float caa) {
            RowWidget* widget = widgets::row();
            widget->gap(gap)->cross_axis_alignment(caa);
            for (auto w : children)
                widget->add_child(w.cast<Widget*>());
            return widget;
        },
        py::kw_only(), py::arg("children") = py::list(), py::arg("gap") = 0, py::arg("cross_axis_alignment") = 0.0f, return_value_policy::reference);

    widgets.def(
        "column", [](py::list children, int gap, float caa) {
            ColumnWidget* widget = widgets::column();
            widget->gap(gap)->cross_axis_alignment(caa);
            for (auto w : children)
                widget->add_child(w.cast<Widget*>());
            return widget;
        },
        py::kw_only(), py::arg("children") = py::list(), py::arg("gap") = 0, py::arg("cross_axis_alignment") = 0.0f, return_value_policy::reference);

    widgets.def(
        "row_grid", [](py::list children, py::tuple flexs) {
            RowGridWidget* widget = widgets::row_grid();
            if (flexs.size() >= children.size()) {
                for (int i = 0; i < children.size(); i++)
                    widget->add_child(children[i].cast<Widget*>(), flexs[i].cast<float>());
            } else {
                int i = 0;
                for (; i < flexs.size(); i++)
                    widget->add_child(children[i].cast<Widget*>(), flexs[i].cast<float>());
                for (; i < children.size(); i++)
                    widget->add_child(children[i].cast<Widget*>(), 1.0f);
            }
            return widget;
        },
        py::kw_only(), py::arg("children") = py::list(), py::arg("flexs") = py::tuple(), return_value_policy::reference);
    widgets.def(
        "column_grid", [](py::list children, py::tuple flexs) {
            ColumnGridWidget* widget = widgets::column_grid();
            if (flexs.size() >= children.size()) {
                for (int i = 0; i < children.size(); i++)
                    widget->add_child(children[i].cast<Widget*>(), flexs[i].cast<float>());
            } else {
                int i = 0;
                for (; i < flexs.size(); i++)
                    widget->add_child(children[i].cast<Widget*>(), flexs[i].cast<float>());
                for (; i < children.size(); i++)
                    widget->add_child(children[i].cast<Widget*>(), 1.0f);
            }
            return widget;
        },
        py::kw_only(), py::arg("children") = py::list(), py::arg("flexs") = py::tuple(), return_value_policy::reference);

    widgets.def("pointer_listener", widgets::pointer_listener,
                py::kw_only(), py::arg("child"), return_value_policy::reference);

    widgets.def(
        "mouse_listener", [](Widget* child) { return mk_widget<MouseListener>()->set_child(child); },
        py::kw_only(), py::arg("child"), return_value_policy::reference);

    widgets.def(
        "focus_listener", [](Widget* child) { return mk_widget<FocusListener>()->set_child(child); },
        py::kw_only(), py::arg("child"), return_value_policy::reference);

    widgets.def(
        "char_input_listener", [](Widget* child) { return mk_widget<CharInputListener>()->set_child(child); },
        py::kw_only(), py::arg("child"), return_value_policy::reference);
}

void bind_widgets(py::module_& m) {
    py::module_ widgets = m.def_submodule("widgets", "RolUI widgets.");
    py::module_ signals = m.def_submodule("signals", "RolUI signals.");
    py::module_ propertys = m.def_submodule("propertys", "RolUI propertys.");

    bind_container_widgets(widgets, signals, propertys);
    bind_layer_Widgets(widgets, signals, propertys);
    bind_flow_widgets(widgets, signals, propertys);
    bind_listener_widgets(widgets, signals, propertys);
    bind_ScrollWidget(widgets, signals, propertys);
    bind_misc_widgets(widgets, signals, propertys);
    bind_widgets_widgets(widgets, signals, propertys);
}
