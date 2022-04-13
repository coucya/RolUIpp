from typing import List, Callable, Iterable, Iterator


import PyRolUI
from PyRolUI import *
from PyRolUI import widgets

from functional_component import *
from popup import *


def _menu_action(title: str, on_click: Callable, text_size=16):
    botton_w = widgets.button(title, text_size=text_size, callback=on_click)
    return sized(child=botton_w, height=int(text_size * 1.5))


def menu_button(title: str, menu_items: dict[str, Callable]):
    def _on_click():
        pos = button.abs_pos()
        size = button.size()
        pos.y += size.height
        popup(child=menu_popup_w, position=pos)

    menu_item_ws = []
    for t, cb in menu_items.items():
        menu_item_ws.append(_menu_action(t, cb))
    menu_popup_w = column(children=menu_item_ws, cross_axis_alignment=-1)
    menu_popup_w = sized(child=menu_popup_w, width=100)

    button: Widget = label_button(text=title, on_click=_on_click)
    return button


def menu_bar(menu_items: dict[str, dict[str, Callable]]) -> Widget:
    buttons = []
    for title, items in menu_items.items():
        bt = menu_button(title, items)
        buttons.append(bt)

    row_w = row(children=buttons)
    return row_w
