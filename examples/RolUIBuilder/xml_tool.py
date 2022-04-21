

import re
import xml.etree.ElementTree as ET

from functional_component import *
from menu import *

_widget_build_funcs = {}
_widget_ids = {}


def register_widget(widget_nema: str, build_func: Callable):
    _widget_build_funcs[widget_nema] = build_func


def unregister_widget(widget_name: str):
    _widget_build_funcs.pop(widget_name)


def get_widget_by_id(id: str) -> Widget:
    return _widget_ids.get(id, None)


class InlineVar:
    name = ""

    def __init__(self, name) -> None:
        self.name = name


color_hex_pattern = re.compile("#(([0-9a-fA-F]{3,4})|([0-9a-fA-F]{6})|([0-9a-fA-F]{8}))")
color_pattern = re.compile("(color|Color|COLOR)\( *(\d{1,3}) *, *(\d{1,3}) *, *(\d{1,3}) *(, *(\d{1,3}) *)?\)")
float_pattern = re.compile("\d+\.\d+")
integer_pattern = re.compile("[1-9]\d*")
inline_var_pattern = re.compile("\$([a-zA-Z_][0-9a-zA-Z_]*)")
inline_expr_pattern = re.compile("\$\{.*\}")


def _str_to_value(s: str):
    result = re.fullmatch(color_hex_pattern, s)
    if result is not None:
        if len(s) == 4:
            c = Color(int(s[1], 16), int(s[2], 16), int(s[3], 16))
        elif len(s) == 5:
            c = Color(int(s[1], 16), int(s[2], 16), int(s[3], 16), int(s[4], 16))
        elif len(s) == 7:
            c = Color(int(s[1:3], 16), int(s[3:5], 16), int(s[5:7], 16))
        elif len(s) == 9:
            c = Color(int(s[1:3], 16), int(s[3:5], 16), int(s[5:7], 16), int(s[7:9], 16))
        else:
            raise ValueError("invalid color string: " + s)
        return c

    result = re.fullmatch(color_pattern, s)
    if result is not None:
        r = int(result.group(2))
        g = int(result.group(3))
        b = int(result.group(4))
        a = 255 if result.group(6) is None else int(result.group(6))
        if r > 255 or g > 255 or b > 255 or a > 255:
            raise ValueError("invalid color string: " + s)
        return Color(r, g, b, a)

    result = re.fullmatch(float_pattern, s)
    if result is not None:
        return float(s)

    result = re.fullmatch(integer_pattern, s)
    if result is not None:
        return int(s)

    result = re.fullmatch(inline_var_pattern, s)
    if result is not None:
        name = result.group(1)
        return InlineVar(name)

    return s


def _replace_inline_var(props: dict, ctx: dict) -> dict:
    new_props = {}
    for k, v in props.items():
        if isinstance(v, InlineVar):
            new_props[k] = ctx.get(v.name, None)
        else:
            new_props[k] = v
    return new_props


def _make_build_func(widget_type: type, widget_type_name: str):
    def _bf(obj: dict, ctx: dict) -> Widget:
        if obj.get("type", None) != widget_type_name:
            raise ValueError("invalid %s object." & widget_type_name)

        props = _replace_inline_var(obj.get("props", {}), ctx)

        w = mk_widget(widget_type, **props)

        children = obj.get("children", [])

        if isinstance(w, SingleChildWidget) and len(children) == 1:
            w.set_child(build_widget_from_object(children[0], ctx))
        elif isinstance(w, SingleChildWidget) and len(children) > 1:
            raise RuntimeError("%s widget must can only have one child widget." % widget_type_name)
        elif isinstance(w, MultiChildWidget):
            for child in map(lambda c: build_widget_from_object(c, ctx), children):
                w.add_child(child)

        id_ = obj.get("id", None)
        if id_ is not None:
            _widget_ids[id_] = w

        return w

    return _bf


def _build_from_template_object(template_obj: dict, ctx: dict) -> Widget:
    children = template_obj.get("children", [])
    if len(children) != 1:
        raise RuntimeError("template_obj must be have one and only one child.")

    child = children[0]
    w = build_widget_from_object(child, ctx)
    return w


def xml_element_to_object(elem: ET.Element) -> dict[str, object]:

    tag = elem.tag
    id_ = elem.get("id", None)
    props = elem.attrib
    children = []

    inner_text = elem.text.strip() if elem.text is not None else ""
    if tag == "text" and inner_text != "":
        props["text"] = inner_text
    elif inner_text != "":
        children.append({
            "type": "text",
            "props": {"text": inner_text},
            "children": []
        })

    props = {k: _str_to_value(v) for k, v in props.items() if k != "id"}

    children.extend(map(xml_element_to_object, elem))

    res = {} if id_ is None else {"id": id_}
    res.update(type=tag, props=props, children=children)

    return res


def xml_to_object(xml_str: str) -> dict[str, object]:
    elem = ET.fromstring(xml_str)
    return xml_element_to_object(elem)


def build_widget_from_object(obj: dict, ctx: dict) -> Widget:
    tn = obj.get("type", None)
    if tn is None:
        raise ValueError("invalid obj: " + str(obj))

    build_func = _widget_build_funcs.get(tn, None)
    if build_func is None:
        raise RuntimeError("not build function of %s" % str(tn))

    return build_func(obj, ctx)


def build_widget_from_xml(xml_str: str, ctx: dict):
    obj = xml_to_object(xml_str)
    return build_widget_from_object(obj, ctx)


register_widget("text", _make_build_func(widgets.TextSpanWidget, "text"))
register_widget("textbox", _make_build_func(widgets.TextBoxWidget, "textbox"))
register_widget("box", _make_build_func(widgets.BoxWidget, "box"))
register_widget("align", _make_build_func(widgets.AlignWidget, "align"))
register_widget("sized", _make_build_func(widgets.SizedWidget, "sized"))
register_widget("margin", _make_build_func(widgets.MarginWidget, "margin"))
register_widget("flexable", _make_build_func(widgets.FlexableWidget, "flexable"))
register_widget("stack", _make_build_func(widgets.StackWidget, "stack"))
register_widget("deck", _make_build_func(widgets.DeckWidget, "deck"))
register_widget("row", _make_build_func(widgets.RowWidget, "row"))
register_widget("column", _make_build_func(widgets.ColumnWidget, "column"))
register_widget("row_grid", _make_build_func(widgets.RowGridWidget, "row_grid"))
register_widget("column_grid", _make_build_func(widgets.ColumnGridWidget, "column_grid"))
register_widget("scroll_view", _make_build_func(widgets.ScrollView, "scroll_view"))
register_widget("vscroll_view", _make_build_func(widgets.VScrollView, "vscroll_view"))
register_widget("hscroll_view", _make_build_func(widgets.HScrollView, "hscroll_view"))
register_widget("vseparator", _make_build_func(widgets.VSeparatorWidget, "vseparator"))
register_widget("hseparator", _make_build_func(widgets.HSeparatorWidget, "hseparator"))
register_widget("poiner_listener", _make_build_func(widgets.PointerListener, "poiner_listener"))
register_widget("mouse_listener", _make_build_func(widgets.MouseListener, "mouse_listener"))
register_widget("char_listener", _make_build_func(widgets.CharInputListener, "char_listener"))
register_widget("focus_listener", _make_build_func(widgets.FocusListener, "focus_listener"))


def _label_button_build_func(obj: dict, ctx: dict) -> Widget:
    props = _replace_inline_var(obj.get("props", {}), ctx)
    w = label_button(**props)
    return w


register_widget("label_button", _label_button_build_func)


def _list_build_func(obj: dict, ctx: dict) -> Widget:
    children = obj.get("children", [])
    template_obj = None
    for c in children:
        if c.get("type", None) == "template":
            template_obj = c

    def _template_func(item_obj) -> Widget:
        return _build_from_template_object(template_obj, item_obj)

    datas = obj.get("props", {}).get("datas", None)
    if isinstance(datas, InlineVar):
        datas = ctx.get(datas.name, [])
    else:
        datas = []
    w = list_view(template_func=_template_func, datas=datas)
    return w


register_widget("list", _list_build_func)


def _menu_build_func(obj: dict, ctx: dict) -> Widget:
    if obj.get("type", None) != "menu":
        raise ValueError("invalid menu object.")

    children = obj.get("children", [])
    props = _replace_inline_var(obj.get("props", {}), ctx)
    gap = props.get("gap", 3)
    width = props.get("width", 128)

    actions = []
    for c in children:
        tn = c.get("type", None)
        if tn == "action":
            c_props = _replace_inline_var(c.get("props", {}), ctx)
            title = c_props.get("title", None)
            cb = c_props.get("on_click", None)
            if title is None:
                raise ValueError("invalid menu object, there is an action without a title.")
            actions.append((title, cb))
        elif tn == "separator":
            actions.append(("--", None))
        else:
            raise ValueError("invalid menu item.")

    w = menu(actions=actions, gap=gap, width=width)
    return w


def _menu_bar_build_func(obj: dict, ctx: dict) -> Widget:
    if obj.get("type", None) != "menu_bar":
        raise ValueError("invalid menu_bar object.")

    children = obj.get("children", [])
    menus = []
    for c in children:
        c_title = _replace_inline_var(c.get("props", {}), ctx).get("title", None)
        if c_title is None:
            raise ValueError("invalid menu bar object, there is an menu without a title.")
        menus.append((c_title, _menu_build_func(c, ctx)))

    w = menu_bar(menus)
    return w


register_widget("menu", _menu_build_func)
register_widget("menu_bar", _menu_bar_build_func)
