from typing import List, Callable, Iterable, Iterator


import PyRolUI
from PyRolUI import *
from PyRolUI import widgets

from functional_component import *


def popup(*, child: Widget, position=None):
    margin_w = None
    if isinstance(position, Point):
        margin_w = margin(child=child, left=position.x, top=position.y)
    elif isinstance(position, tuple) and len(position) == 2 and isinstance(position[0], int) and isinstance(position[1], int):
        margin_w = margin(child=child, left=position[0], top=position[1])
    else:
        margin_w = margin(child=child, left=0, top=0)

    def _on_down(a, b):
        print("_on_down:", a, b)
        Application.root_widget().remove_child(mouse_l)

    size_w = sized(child=margin_w)
    mouse_l = mouse_listener(child=size_w)
    mouse_l.on_down.connect(_on_down)
    Application.root_widget().add_child(mouse_l)