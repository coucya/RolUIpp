from typing import List, Callable, Iterable, Iterator
from functools import singledispatch

import sys
import os

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


def render(obj: dict):
    if not issubclass(obj.get("type", None), Widget):
        raise ValueError("obj must be Widget type.")
    WidgetType = obj["type"]
    props = obj.get("props", {})

    widget: Widget = WidgetType()
    _gc_widgets.add(widget)

    def _build_cb(w: Widget, k):
        return lambda val: getattr(w, k).set(val) if w.mounted() else None

    for k, v in props.items():
        if hasattr(widget, k):
            if isinstance(v, State):
                getattr(widget, k).set(v.get())
                v.add_callback(_build_cb(widget, k))
            else:
                getattr(widget, k).set(v)

    if "children" in obj and isinstance(obj["children"], list):
        if isinstance(widget, MultiChildWidget):
            children = map(render, obj["children"])
            for c in children:
                widget.add_child(c)
        else:
            raise ValueError("Invalid children: " + str(obj["children"]))
    elif "child" in obj and isinstance(obj["child"], dict):
        if isinstance(widget, SingleChildWidget):
            w = render(obj["child"])
            widget.set_child(w)
        else:
            raise ValueError("Invalid child: " + str(obj["child"]))

    return widget


def textspan(text, *, font_size=16, font_name="default", font_color=Color(64, 64, 64)) -> Widget:
    args = locals()
    return mk_widget(widgets.TextSpanWidget, **args)


def textbox(text="", *, font_size=16, font_name="default", font_color=Color(64, 64, 64)) -> Widget:
    args = locals()
    return mk_widget(widgets.TextBoxWidget, **args)


def box(*, child: Widget = None, round=0, border_width=0, border_color=Color(), background_color=Color(0, 0, 0, 0)) -> Widget:
    args = locals()
    return mk_widget(widgets.BoxWidget, **args)


def align(*, child: Widget = None, x=0, y=0) -> Widget:
    args = locals()
    return mk_widget(widgets.AlignWidget, **args)


def sized(*, child: Widget = None, width=1.0, height=1.0) -> Widget:
    w = widgets.SizeUnit(width) if isinstance(width, (int, float)) else width
    h = widgets.SizeUnit(height) if isinstance(height, (int, float)) else height
    return mk_widget(widgets.SizedBoxWidget, child=child, width=w, height=h)


def margin(*, child: Widget = None, top=0, rbottom=0, rleft=0, right=0) -> Widget:
    args = locals()
    return mk_widget(widgets.MarginWidget, **args)


def vscroll_view(*, child: Widget = None, scroll_step=10.0) -> Widget:
    args = locals()
    return mk_widget(widgets.VScrollView, **args)


def hscroll_view(*, child: Widget = None, scroll_step=10.0) -> Widget:
    args = locals()
    return mk_widget(widgets.HScrollView, **args)


def stack(*, children: List[Widget], align_x=0, align_y=0) -> Widget:
    args = locals()
    return mk_widget(widgets.StackWidget, **args)


def deck(*, children: List[Widget], selected=0) -> Widget:
    args = locals()
    return mk_widget(widgets.DeckWidget, **args)


def row(*, children: List[Widget], gap=0, cross_axis_alignment=0.0) -> Widget:
    args = locals()
    return mk_widget(widgets.RowWidget, **args)


def column(*, children: List[Widget], gap=0, cross_axis_alignment=0.0) -> Widget:
    args = locals()
    return mk_widget(widgets.ColumnWidget, **args)


def mouse_listener(*, child: Widget = None, **kw_args) -> Widget:
    return mk_widget(widgets.MouseListener, child=child, **kw_args)


def keyboard_linstener(*, child: Widget = None, on_key: Callable) -> Widget:
    return mk_widget(widgets.KeyboardListener, child=child, on_key=on_key)


def char_listener(*, child: Widget = None, on_input: Callable) -> Widget:
    return mk_widget(widgets.CharInputListener, child=child, on_input=on_input)
