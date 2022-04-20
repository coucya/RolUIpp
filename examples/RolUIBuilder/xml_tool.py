

import re
import xml.etree.ElementTree as ET

from functional_component import *

_widget_build_funcs = {}
_widget_ids = {}


def register_widget(widget_nema: str, build_func: Callable):
    _widget_build_funcs[widget_nema] = build_func


def unregister_widget(widget_name: str):
    _widget_build_funcs.pop(widget_name)


def get_widget_by_id(id: str) -> Widget:
    return _widget_ids.get(id, None)


color_hex_pattern = re.compile("#(([0-9a-fA-F]{3,4})|([0-9a-fA-F]{6})|([0-9a-fA-F]{8}))")
color_pattern = re.compile("(color|Color|COLOR)\( *(\d{1,3}) *, *(\d{1,3}) *, *(\d{1,3}) *(, *(\d{1,3}) *)?\)")
float_pattern = re.compile("\d+\.\d+")
integer_pattern = re.compile("[1-9]\d*")


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

    return s


def _make_build_func(widget_type: type, widget_type_name: str):
    def _bf(obj: dict) -> Widget:
        if obj.get("type", None) != widget_type_name:
            raise ValueError("invalid %s object." & widget_type_name)

        w = mk_widget(widget_type, **obj.get("props", {}))

        children = obj.get("children", [])

        if isinstance(w, SingleChildWidget) and len(children) == 1:
            w.set_child(build_widget_from_object(children[0]))
        elif isinstance(w, SingleChildWidget) and len(children) > 1:
            raise RuntimeError("%s must can only have one child widget." % widget_type_name)
        elif isinstance(w, MultiChildWidget):
            for child in map(build_widget_from_object, children):
                w.add_child(child)

        id_ = obj.get("id", None)
        if id_ is not None:
            _widget_ids[id_] = w

        return w

    return _bf


def xml_element_to_object(elem: ET.Element) -> dict[str, object]:

    tag = elem.tag
    id_ = elem.get("id", None)
    props = {k: _str_to_value(v) for k, v in elem.attrib.items() if k != "id"}
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

    children.extend(map(xml_element_to_object, elem))

    res = {} if id_ is None else {"id": id_}
    res.update(type=tag, props=props, children=children)

    return res


def xml_to_object(xml_str: str) -> dict[str, object]:
    elem = ET.fromstring(xml_str)
    return xml_element_to_object(elem)


def build_widget_from_object(obj: dict) -> Widget:
    tn = obj.get("type", None)
    if tn is None:
        raise ValueError("invalid obj: " + str(obj))

    build_func = _widget_build_funcs.get(tn, None)
    if build_func is None:
        raise RuntimeError("not build function of %s" % str(tn))

    return build_func(obj)


def build_widget_from_xml(xml_str: str):
    obj = xml_to_object(xml_str)
    return build_widget_from_object(obj)


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

register_widget("label_button", lambda obj: label_button(**obj.get("props", {})))
