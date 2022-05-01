from typing import List, Callable, Iterable, Iterator, Union


import PyRolUI
from PyRolUI import *
from PyRolUI import widgets


class State:
    def __init__(self, val=None) -> None:
        self._val = val
        self._callbacks = {}
        self._handles = 0

    def __call__(self, *args):
        if len(args) == 1:
            self.set(args[0])
        elif len(args) >= 1:
            raise ValueError("at most one parameter is accepted.")
        return self._val

    def get(self):
        return self._val

    def set(self, val):
        self._val = val
        for cb in filter(lambda cb: callable(cb), self._callbacks.values()):
            cb(self._val)

    def add_callback(self, cb: Callable[[object], object]) -> int:
        self._handles += 1
        self._callbacks[self._handles] = cb
        return self._handles

    def remove_callback(self, handle: int):
        if handle in self._callbacks:
            self._callbacks.pop(handle)


_gc_widgets = set()


def drop_unmounted():
    global _gc_widgets
    _gc_widgets = set(filter(lambda w: w.mounted(), _gc_widgets))


def mk_widget(widget_type: type, **kw_args) -> Widget:
    if not issubclass(widget_type, Widget):
        raise ValueError("widget_type must be subclass of Widget.")

    widget = widget_type()
    _gc_widgets.add(widget)

    def _event_filter(pair):
        return pair[0].startswith("on_") and callable(pair[1])

    def _props_filter(pair):
        return pair[0] != "child" and pair[0] != "children" and not _event_filter(pair)
    events = {k: v for k, v in filter(_event_filter, kw_args.items())}
    props = {k: v for k, v in filter(_props_filter, kw_args.items())}
    child = kw_args.get("child", None)
    children = kw_args.get("children", None)

    def _build_cb(w: Widget, prop_name):
        return lambda val: getattr(w, prop_name).set(val) if w.mounted() else None

    for prop_name, prop_value in props.items():
        if hasattr(widget, prop_name):
            if isinstance(prop_value, State):
                getattr(widget, prop_name).set(prop_value.get())
                prop_value.add_callback(_build_cb(widget, prop_name))
            else:
                getattr(widget, prop_name).set(prop_value)

    for event_name, event_value in events.items():
        if hasattr(widget, event_name):
            getattr(widget, event_name).connect(event_value)

    if isinstance(children, list) and all(map(lambda w: isinstance(w, Widget), children)) and isinstance(widget, MultiChildWidget):
        for c in children:
            widget.add_child(c)
    elif isinstance(child, Widget) and isinstance(widget, SingleChildWidget):
        widget.set_child(child)
    elif child is not None or children is not None:
        raise ValueError("Invalid child: (%s) or children: (%s)." % (str(child), str(children)))

    return widget


def textspan(text, *, font_size=16, font_name="default", font_color=Color(64, 64, 64)) -> widgets.TextSpanWidget:
    args = locals()
    return mk_widget(widgets.TextSpanWidget, **args)


def textbox(text="", *, font_size=16, font_name="default", font_color=Color(64, 64, 64)) -> widgets.TextBoxWidget:
    args = locals()
    return mk_widget(widgets.TextBoxWidget, **args)


def box(*, child: Widget = None, round=0, border_width=0, border_color=Color(), background_color=Color(0, 0, 0, 0)) -> widgets.BoxWidget:
    args = locals()
    return mk_widget(widgets.BoxWidget, **args)


def align(*, child: Widget = None, align_x=0, align_y=0) -> widgets.AlignWidget:
    args = locals()
    return mk_widget(widgets.AlignWidget, **args)


def sized(*, child: Widget = None, width=1.0, height=1.0) -> widgets.SizedWidget:
    w = widgets.SizeUnit(width) if isinstance(width, (int, float)) else width
    h = widgets.SizeUnit(height) if isinstance(height, (int, float)) else height
    return mk_widget(widgets.SizedWidget, child=child, width=w, height=h)


def margin(*, child: Widget = None, top=0, bottom=0, left=0, right=0) -> widgets.MarginWidget:
    args = locals()
    return mk_widget(widgets.MarginWidget, **args)


def scroll_view(*, child: Widget = None):
    return mk_widget(widgets.ScrollView, child=child)


def vscroll_view(*, child: Widget = None, scroll_step=10.0) -> widgets.VScrollView:
    args = locals()
    return mk_widget(widgets.VScrollView, **args)


def hscroll_view(*, child: Widget = None, scroll_step=10.0) -> widgets.HScrollView:
    args = locals()
    return mk_widget(widgets.HScrollView, **args)


def stack(*, children: List[Widget], align_x=0, align_y=0) -> widgets.StackWidget:
    args = locals()
    return mk_widget(widgets.StackWidget, **args)


def deck(*, children: List[Widget], selected=0) -> widgets.DeckWidget:
    args = locals()
    return mk_widget(widgets.DeckWidget, **args)


def row(*, children: List[Widget], gap=0, cross_axis_alignment=0.0) -> widgets.RowWidget:
    args = locals()
    return mk_widget(widgets.RowWidget, **args)


def column(*, children: List[Widget], gap=0, cross_axis_alignment=0.0) -> widgets.ColumnWidget:
    args = locals()
    return mk_widget(widgets.ColumnWidget, **args)


def flexable(*, child: Widget = None, fixed=None, percentage=None, flex=None, expand=None) -> widgets.FlexableWidget:
    if((fixed, percentage, flex, expand).count(None) < 3):
        raise ValueError("fixed. percentage. flex and expand are mutually exclusive")

    if fixed is not None:
        return mk_widget(widgets.FlexableWidget, child=child, fit=widgets.FlexFit.fixed, fixed=fixed)
    elif percentage is not None:
        return mk_widget(widgets.FlexableWidget, child=child, fit=widgets.FlexFit.percentage, percentage=percentage)
    elif flex is not None:
        return mk_widget(widgets.FlexableWidget, child=child, fit=widgets.FlexFit.flex, flex=flex)
    elif expand is not None:
        return mk_widget(widgets.FlexableWidget, child=child, fit=widgets.FlexFit.expand)
    else:
        return mk_widget(widgets.FlexableWidget, child=child, fit=widgets.FlexFit.flex, flex=1.0)


def row_grid(*, children: List[Widget]) -> widgets.RowGridWidget:
    return mk_widget(widgets.RowGridWidget, children=children)


def column_grid(*, children: List[Widget]) -> widgets.ColumnGridWidget:
    return mk_widget(widgets.ColumnGridWidget, children=children)


def vseparator(*, color=Color(0, 0, 0, 255), width=1) -> widgets.VSeparatorWidget:
    return mk_widget(widgets.VSeparatorWidget, color=color, width=width)


def hseparator(*, color=Color(0, 0, 0, 255), width=1) -> widgets.HSeparatorWidget:
    return mk_widget(widgets.HSeparatorWidget, color=color, width=width)


def mouse_listener(*, child: Widget = None, **kw_args) -> widgets.MouseListener:
    return mk_widget(widgets.MouseListener, child=child, **kw_args)


def keyboard_linstener(*, child: Widget = None, on_key: Callable) -> widgets.KeyboardListener:
    return mk_widget(widgets.KeyboardListener, child=child, on_key=on_key)


def char_listener(*, child: Widget = None, on_input: Callable) -> widgets.CharInputListener:
    return mk_widget(widgets.CharInputListener, child=child, on_input=on_input)


def focus_listener(*, child: Widget = None, on_focus: Callable) -> widgets.FocusListener:
    return mk_widget(widgets.FocusListener, child=child, on_focus=on_focus)


def basic_button(*, content_widget: Widget = None,
                 match_content=True,
                 bk_normal_color: Color = Color(247, 247, 247),
                 bk_hover_color: Color = Color(229, 243, 255),
                 bk_press_color: Color = Color(204, 232, 255),
                 round: int = 0, border_color: Color = Color(64, 64, 64, 255), border_width=0,
                 on_click=None, on_hover=None):

    margin_w = content_widget
    if not match_content:
        margin_w = align(child=margin_w, align_x=-1, align_y=-1)

    box_w = box(child=margin_w, round=round, border_color=border_color, border_width=border_width)
    box_w.background_color(bk_normal_color)

    def _on_hover(b):
        box_w.background_color(bk_hover_color if b else bk_normal_color)
        if callable(on_hover):
            on_hover(b)

    def _on_down(a, b):
        box_w.background_color(bk_press_color)

    def _on_up(a, b):
        box_w.background_color(bk_hover_color)

    def _on_click(a, b):
        if callable(on_click):
            on_click()

    mouse_l = mouse_listener(child=box_w, on_click=_on_click, on_hover=_on_hover, on_down=_on_down, on_up=_on_up)
    return mouse_l


def label_button(*, text="", text_size: int = 16, text_color: Color = Color(32, 32, 32),
                 padding: Union[tuple, int, None] = 0,
                 bk_normal_color: Color = Color(74, 165, 240),
                 bk_hover_color: Color = Color(229, 243, 255),
                 bk_press_color: Color = Color(204, 232, 255),
                 round: int = 0, border_color: Color = Color(64, 64, 64, 255), border_width=0,
                 on_click=None, on_hover=None):
    text_w = textspan(text=text, font_size=text_size, font_color=text_color)
    if isinstance(padding, int):
        margin_w = margin(child=text_w, top=padding, bottom=padding, left=padding, right=padding)
    elif isinstance(padding, tuple) and len(padding) == 1:
        margin_w = margin(child=text_w, top=padding[0], bottom=padding[0], left=padding[0], right=padding[0])
    elif isinstance(padding, tuple) and len(padding) == 2:
        mx, my = padding
        margin_w = margin(child=text_w, top=my, bottom=my, left=mx, right=mx)
    elif isinstance(padding, tuple) and len(padding) == 4:
        mt, mr, mb, ml = padding
        margin_w = margin(child=text_w, top=mt, bottom=mb, left=ml, right=mr)
    else:
        raise TypeError("invalid margin value: %s" % str(padding))

    w = basic_button(content_widget=margin_w, match_content=True,
                     bk_normal_color=bk_normal_color, bk_hover_color=bk_hover_color, bk_press_color=bk_press_color,
                     round=round, border_color=border_color, border_width=border_width,
                     on_click=on_click, on_hover=on_hover)
    return w


def list_view(*, template_func: Callable, datas: list, gap: int = 0, cross_axis_alignment: float = 0.0) -> Widget:
    if not callable(template_func):
        raise TypeError("template_func must be callable.")

    w: MultiChildWidget = column(children=[], gap=gap, cross_axis_alignment=cross_axis_alignment)

    def _cb(new_datas):
        w.rm_child_all()
        for data_item in new_datas:
            cw = template_func(data_item)
            w.add_child(cw)

    if isinstance(datas, State):
        datas.add_callback(_cb)
        _cb(datas.get())
    else:
        _cb(datas)

    return w


def _tree_view(*, template_func: Callable, datas: dict, head_height: int = 30, indent: int, level: int) -> Widget:
    if not callable(template_func):
        raise TypeError("template_func must be callable.")

    w = template_func(datas)
    children_datas = datas.get("children", [])
    children_nodes = []

    for child_datas in children_datas:
        cw = _tree_view(template_func=template_func, datas=child_datas, indent=indent, level=level + 1)
        children_nodes.append(cw)

    head_w = margin(child=w, left=indent * level)
    head_w = sized(child=head_w, width=1.0, height=head_height)
    head_w = basic_button(content_widget=head_w)
    children_node_w = column(children=children_nodes, cross_axis_alignment=-1)
    node_w = column(children=[head_w, children_node_w], cross_axis_alignment=-1)
    return node_w


def tree_view(*, template_func: Callable, datas: dict, head_height: int = 30, indent: int = 5) -> Widget:
    w: widgets.BoxWidget = box()

    def _cb(new_datas):
        w.rm_child()
        cw = _tree_view(template_func=template_func, datas=new_datas, indent=indent, level=0)
        w.set_child(cw)

    if isinstance(datas, State):
        datas.add_callback(_cb)
        _cb(datas.get())
    else:
        _cb(datas)
    return w
