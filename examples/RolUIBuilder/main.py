from typing import List, Callable, Iterable, Iterator

import sys
import os

import PyRolUI
from PyRolUI import *
from PyRolUI import widgets


from PyRolUI_glfw import *

import functional_component
from functional_component import *
from text_editor import *
from popup import *
from menu import *
from xml_tool import *


def load_file(path) -> str:
    with open(path, "r", encoding="utf8") as f:
        return f.read()


class VScrollPanel(SingleChildWidget):
    def __init__(self, content_widget=None) -> None:
        super().__init__()

        self._content_w: Widget = content_widget

        box_w = box(background_color=Color(205, 205, 205))
        self._scroll_slider_mouse_l = mouse_listener(child=box_w, on_drag=self._on_drag)
        self._scroll_slider_w = sized(child=self._scroll_slider_mouse_l, width=1.0, height=1.0)
        self._scroll_scroll_v = vscroll_view(child=self._scroll_slider_w)
        self._scroll_bar_w = flexable(child=self._scroll_scroll_v, fixed=20)

        self._content_scroll_v: VScrollView = vscroll_view(child=None, scroll_step=30)

        self._row_grid_w: widgets.RowGridWidget = row_grid(children=[self._content_scroll_v, self._scroll_bar_w])
        self.set_child(self._row_grid_w)

    def _on_drag(self, mouse_key, offset: Point):
        self._scroll_scroll_v.scroll_y_by_px(offset.y)
        if self._scroll_scroll_v.widget_y_ratio() < 0.0:
            self._scroll_scroll_v.scroll_y_to_ratio(0)
        elif self._scroll_scroll_v.widget_y_ratio() > 1.0:
            self._scroll_scroll_v.scroll_y_to_ratio(1)
        self._content_scroll_v.scroll_y_to_ratio(self._scroll_scroll_v.widget_y_ratio())

    def set_content_widget(self, w: Widget):
        self._content_w = w
        self._content_scroll_v.set_child(w)

    def perform_layout(self, constraint) -> Size:
        layout_res = super().perform_layout(constraint)

        self_height = self.size().height
        content_height = 0 if self._content_w is None else self._content_w.size().height
        if content_height <= self_height:
            self._scroll_slider_w.height(widgets.SizeUnit(int(self_height)))
        else:
            self._scroll_slider_w.height(widgets.SizeUnit(int(self_height / content_height * self_height)))
        yr = self._content_scroll_v.widget_y_ratio()
        self._scroll_scroll_v.scroll_y_to_ratio(yr)

        # bar_size = self._scroll_bar_w.size()
        # nc = Constraint.zero_to(bar_size)
        # self._scroll_bar_w.layout(nc)

        # print("self_height:", self_height, "content_height:", content_height, "yr:", yr)
        # print("self._scroll_slider_w.size():", self._scroll_slider_w.size())
        # print("self._scroll_scroll_v.size():", self._scroll_scroll_v.size())
        # print("self._scroll_bar_w.size():", self._scroll_bar_w.size())
        # print()

        return layout_res


def vscroll_panel(*, child: Widget):
    scroll_p: VScrollPanel = mk_widget(VScrollPanel)
    scroll_p.set_content_widget(child)
    return scroll_p


win = GLFWWindow(400, 300, "Python test.")
win.on_exit = lambda: Application.exit()

Application.init(win)
load_font("default", "C:\\WINDOWS\\FONTS\\MSYHL.TTC")

# label_widget = textspan("label", font_size=20)
# button_widget = label_button(text="button", text_size=20, padding=10, round=5)
# column_widget = column(children=[label_widget, button_widget])
# w = align(child=column_widget)
context = {"label_str": "lebel", "button_str": "button"}
xml_str = load_file(os.path.join(os.path.dirname(__file__), "ui.xml"))
w = build_widget_from_xml(xml_str, context)

Application.run(w)


_text = load_file(__file__)

ui_xml_path = os.path.join(os.path.dirname(__file__), "ui.xml")
ui_xml_str = load_file(ui_xml_path)

RolUIBuilder_xml_path = os.path.join(os.path.dirname(__file__), "RolUIBuilder.xml")
RolUIBuilder_xml_str = load_file(RolUIBuilder_xml_path)


tree_nodes = {
    "text": "aaa",
    "children": [
        {"text": "aaaaaa", "children": [
            {"text": "aaaaaaaaa"},
            {"text": "aaaaaaaaa"},
            {"text": "aaaaaaaaa"},
            {"text": "aaaaaaaaa"},
        ]},
        {"text": "aaaaaa", "children": [
            {"text": "aaaaaaaaa"},
            {"text": "aaaaaaaaa"},
            {"text": "aaaaaaaaa"},
            {"text": "aaaaaaaaa"},
        ]},
        {"text": "aaaaaa", "children": [
            {"text": "aaaaaaaaa"},
            {"text": "aaaaaaaaa"},
            {"text": "aaaaaaaaa"},
            {"text": "aaaaaaaaa"},
        ]},
    ]
}


def _dir_to_resource_datas(path: str) -> dict:
    def _name(path):
        return os.path.basename(path)

    children = []
    for p in os.listdir(path):
        if os.path.isdir(p):
            children.append(_dir_to_resource_datas(p))
        else:
            children.append({"name": _name(p)})

    res = {"name": _name(path), "children": children}
    return res


ctx = {
    "resource_datas": _dir_to_resource_datas(os.path.dirname(__file__)),
    "bt_click": lambda: print("bt_click"),
    "/menu/file/new": lambda: print("/menu/file/new"),
    "/menu/file/open": lambda: print("/menu/file/open"),
    "/menu/file/save": lambda: print("/menu/file/save"),
    "python_text": _text,
}


w = build_widget_from_xml(RolUIBuilder_xml_str, ctx)


# Application.run(w)
