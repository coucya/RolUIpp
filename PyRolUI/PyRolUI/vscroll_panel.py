import math

import PyRolUI
from PyRolUI import *
from PyRolUI import popup
from PyRolUI.functional_component import *
from PyRolUI import xml_tool as xt
from PyRolUI.widgets import VScrollView


class VScrollPanel(SingleChildWidget):
    def __init__(self, content_widget=None) -> None:
        super().__init__()

        self._content_w: Widget = content_widget

        box_w = box(background_color=Color(205, 205, 205))
        self._scroll_bar_mask_unpopup = None
        self._scroll_bar_mask_l = mouse_listener(child=box(), 
                                                 on_move=self._on_mask_move, on_up=self._on_mask_up,
                                                 on_hover=self._on_mask_hover)
        self._scroll_slider_mouse_l = mouse_listener(child=box_w, on_down=self._on_slider_down)
        self._scroll_slider_w = sized(child=self._scroll_slider_mouse_l, width=1.0, height=1.0)
        self._scroll_scroll_v: VScrollView = vscroll_view(child=self._scroll_slider_w)
        self._scroll_bar_w = flexable(child=self._scroll_scroll_v, fixed=18)

        self._content_scroll_v: VScrollView = vscroll_view(child=None, scroll_step=30)
        self._content_scroll_v.on_scroll.connect(self._on_scroll)

        self._row_grid_w: widgets.RowGridWidget = row_grid(children=[self._content_scroll_v, self._scroll_bar_w])
        self.set_child(self._row_grid_w)

    def _on_scroll(self, offset):
        self._scroll_scroll_v.scroll_to_ratio(self._content_scroll_v.widget_x_ratio(), self._content_scroll_v.widget_y_ratio())

    def _on_slider_down(self, *args):
        self._scroll_bar_mask_unpopup = popup.popup(child=self._scroll_bar_mask_l)

    def _on_mask_hover(self, hover):
        if not hover and callable(self._scroll_bar_mask_unpopup):
            self._scroll_bar_mask_unpopup()
            
    def _on_mask_up(self, *args):
        if callable(self._scroll_bar_mask_unpopup):
            self._scroll_bar_mask_unpopup()

    def _on_mask_move(self, offset: Point):
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
        
        bar_size = self._scroll_bar_w.size()
        nc = Constraint.zero_to(bar_size)
        self._scroll_bar_w.layout(nc)

        return layout_res


def vscroll_panel(*, child: Widget = None, on_scroll=None):
    scroll_p: VScrollPanel = mk_widget(VScrollPanel, on_scroll=on_scroll)
    scroll_p.set_content_widget(child)
    return scroll_p


def _vscroll_panel_build_func(obj, ctx):
    children = obj.get("children", [])
    props = xt._replace_props_with_context(obj.get("props", {}), ctx)

    child = None
    if len(children) == 1:
        child = xt.build_widget_from_object(children[0], ctx)
    elif len(children) == 0:
        pass
    else:
        raise RuntimeError("vscroll_panel must be one child.")

    w = vscroll_panel(child=child)

    return w


xt.register_widget("vscroll_panel", _vscroll_panel_build_func)
