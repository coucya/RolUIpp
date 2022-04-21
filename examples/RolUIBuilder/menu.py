from typing import List, Callable, Iterable, Iterator


import PyRolUI
from PyRolUI import *
from PyRolUI import widgets

from functional_component import *
from popup import *


def _menu_action(title: str, on_click: Callable, text_size=16):
    content_w = sized(child=align(child=margin(child=textspan(title, font_size=text_size), left=8), align_x=-1), height=int(text_size * 1.5))
    button_w = basic_button(content_widget=content_w, match_content=True, on_click=on_click)
    return button_w


def menu(actions: list[tuple[str, Callable]], gap=3, width: int = 128) -> Widget:
    popup_item_widgets = []
    for t, cb in actions:
        if all(map(lambda s: s == "-", t)):
            popup_item_widgets.append(margin(child=hseparator(color=Color(128, 128, 128)), top=5, bottom=5))
        else:
            popup_item_widgets.append(_menu_action(t, cb))
    menu_popup_w = column(children=popup_item_widgets, gap=gap, cross_axis_alignment=-1)
    menu_popup_w = box(child=menu_popup_w, background_color=Color(250, 250, 250))
    menu_popup_w = sized(child=menu_popup_w, width=width)
    return menu_popup_w


def _menu_bar_item_button(title: str, popup_widget: Widget, menu_bar_state):

    def _popup(w, pos):
        current_popup_widget = menu_bar_state["popup_widget"]
        unpopuper = menu_bar_state.get("popup_widget_unpopuper", None)

        if current_popup_widget is w:
            return

        if callable(unpopuper):
            unpopuper()

        def _on_unpopup():
            menu_bar_state["popup_widget"] = None
            menu_bar_state["popup_widget_unpopuper"] = None

        menu_bar_state["popup_widget"] = w
        unpopuper = popup(child=w, position=pos, on_unpopup=_on_unpopup)
        menu_bar_state["popup_widget_unpopuper"] = unpopuper

    def _on_hover(b):
        pos = button.abs_pos()
        size = button.size()
        pos.y += size.height
        current_popup_widget = menu_bar_state["popup_widget"]
        if b and current_popup_widget is not None:
            _popup(popup_widget, pos)

    def _on_click():
        pos = button.abs_pos()
        size = button.size()
        pos.y += size.height
        _popup(popup_widget, pos)

    button: Widget = label_button(text=title, padding=(8, 5), on_click=_on_click, on_hover=_on_hover)
    return button


def menu_bar(children: list[tuple[str, Widget]]) -> Widget:

    # def menu_bar(menu_items: list[tuple[str, dict[str, Callable]]]) -> Widget:
    # popup_widgets = {k: menu_popup_widget(v) for k, v in children}
    menu_bar_state = {
        "popup_widget": None,
        "popup_widget_unpopuper": None,
        "hover": False
    }

    bar_buttons = []
    for k, v in children:
        button: Widget = _menu_bar_item_button(k, v, menu_bar_state)
        bar_buttons.append(button)

    row_w = row(children=bar_buttons,gap=2)
    return row_w
