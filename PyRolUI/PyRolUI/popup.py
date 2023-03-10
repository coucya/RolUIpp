from typing import List, Callable, Iterable, Iterator


from . import *
from . import widgets
from .widgets import *

from . import functional_component as fc
# from .functional_component import *


def popup(*, child: Widget, position=None, on_unpopup: Callable = None):
    margin_w = None
    if isinstance(position, Point):
        margin_w = fc.margin(child=child, left=position.x, top=position.y)
    elif isinstance(position, tuple) and len(position) == 2 and isinstance(position[0], int) and isinstance(position[1], int):
        margin_w = fc.margin(child=child, left=position[0], top=position[1])
    else:
        margin_w = fc.margin(child=child, left=0, top=0)

    def _unpopup():
        if callable(on_unpopup):
            on_unpopup()
        Application.root_widget().rm_child(mouse_l)

    size_w = fc.sized(child=margin_w)
    mouse_l = fc.mouse_listener(child=size_w)
    mouse_l.on_down.connect(lambda a, b: _unpopup())
    Application.root_widget().add_child(mouse_l)

    return _unpopup
