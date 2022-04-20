from PyRolUI.widgets import TextBoxWidget, ColumnWidget, VScrollView
from PyRolUI.events import *
from functional_component import *

import tokenize
import keyword
from io import StringIO
import re

PYTHON_KEYWORDS = keyword.kwlist

PYTHON_OP = (
    "=", "+=", "-=", "*=", "/=", "==", "!=", "&=", "|=", "^=", ">=", "<=", "**=", "//=", "**", "//",
    "+", "-", "*", "/", "|", "[", "]", "{", "}", "(", ")", "~", "!", "@", "%", "^", "&", "<", ">", "?", ":"
)

code_style = {
    "normal": Color(0x33, 0x33, 0x33),
    "keyword": Color(175, 0, 219),
    "id": Color(0, 16, 128),
    "string": Color(163, 21, 21),
    "number": Color(9, 134, 88)
}


def wrap_bracket(s): return "(" + s + ")"


bininteger_pattern_str = "[01](_?[01])*"
octinteger_pattern_str = "\d(_?\d)*"
hexinteger_pattern_str = "[0-9a-fA-F](_?[0-9a-fA-F])*"
integer_pattern_str = "(\+|\-)?([1-9](_?\d)*)|((0x|0X)(_?[0-9a-fA-F])+))|((0b|0B)(_?[01]+))"
t = map(wrap_bracket, [octinteger_pattern_str, "(0b|0B)" + bininteger_pattern_str, "(0x|0X)" + hexinteger_pattern_str])
integer_pattern_str = "(\+|\-)?" + "|".join(t)

float_pattern_str = "(\+|\-)(\d\." + octinteger_pattern_str + ")|(\d\.\d\d[eE][\-\+]\d+)"

id_pattern = re.compile("[a-zA-Z][0-9a-zA-Z_]*")
short_string_pattern = re.compile("(\"((\\\\.)|[^\"])*\")|('((\\.)|[^'])*')")
long_string_pattern = re.compile("(\"\"\"((\\\\.)|\n|[^\"])*\"\"\")|('''((\\\\.)|\n|[^'])*''')")
number_pattern = re.compile("|".join(map(wrap_bracket, [float_pattern_str, integer_pattern_str])))
blank_pattern = re.compile("[ \t\r]+")
other_pattern = re.compile("[^ \n\t\r]+")


def split_token_with_re(code_str: str) -> list[(int, Color)]:
    tokens = []
    code_str_it = code_str
    while code_str_it != "":
        tk = None
        if (match_result := re.match(id_pattern, code_str_it)) is not None:
            tk = (match_result.group(), code_style["keyword"] if match_result.group() in PYTHON_KEYWORDS else code_style["id"])
            code_str_it = code_str_it[match_result.end():]
        elif code_str_it.startswith(PYTHON_OP):
            for op in PYTHON_OP:
                if code_str_it.startswith(op):
                    tk = (op, code_style["normal"])
                    code_str_it = code_str_it[len(op):]
                    break
        elif (match_result := re.match(number_pattern, code_str_it)) is not None:
            tk = (match_result.group(), code_style["number"])
            code_str_it = code_str_it[match_result.end():]
        elif (match_result := re.match(long_string_pattern, code_str_it)) is not None:
            tk = []
            ls = match_result.group().split("\n")
            for l in ls:
                t = (l, code_style["string"])
                tk.append(t)
                tk.append(('\n', code_style["normal"]))
            tk.pop()
            code_str_it = code_str_it[match_result.end():]
        elif (match_result := re.match(short_string_pattern, code_str_it)) is not None:
            tk = (match_result.group(), code_style["string"])
            code_str_it = code_str_it[match_result.end():]
        elif (match_result := re.match(blank_pattern, code_str_it)) is not None:
            tk = (match_result.group(), code_style["normal"])
            code_str_it = code_str_it[match_result.end():]
        elif (match_result := re.match(other_pattern, code_str_it)) is not None:
            tk = (match_result.group(), code_style["normal"])
            code_str_it = code_str_it[match_result.end():]
        elif code_str_it.startswith("\n"):
            tk = ("\n", code_style["normal"])
            code_str_it = code_str_it[1:]
        if isinstance(tk, list):
            tokens += tk
        else:
            tokens.append(tk)
    return tokens


def split_token_with_tokenize(code_str: str) -> list[(int, Color)]:
    token_gen = tokenize.generate_tokens(StringIO(code_str).readline)
    tokens = []
    last_line_pos = 0
    last_column_pos = 0

    def is_muilline_token(tk: tokenize.TokenInfo) -> bool:
        return tk.end[0] > tk.start[0]

    try:
        for tk in token_gen:
            ttk = None

            if tk.type == tokenize.NAME:
                if tk.string in PYTHON_KEYWORDS:
                    ttk = (tk.string, code_style["keyword"])
                else:
                    ttk = (tk.string, code_style["id"])
            elif tk.type == tokenize.STRING:
                if is_muilline_token(tk):
                    ttk = []
                    for s in tk.string.split("\n"):
                        ttk.append((s, code_style["string"]))
                        ttk.append(("\n", code_style["string"]))
                    ttk.pop()
                else:
                    ttk = (tk.string, code_style["string"])
            else:
                ttk = (tk.string, code_style["normal"])

            if tk.start[1] > last_column_pos:
                tokens.append((" " * (tk.start[1] - last_column_pos), code_style["normal"]))

            if isinstance(ttk, list):
                tokens = tokens + ttk
            else:
                tokens.append(ttk)

            last_line_pos, last_column_pos = tk.end

            if tk.string.endswith("\n"):
                last_column_pos = 0
    except:
        code_lines = code_str.split('\n')
        if last_line_pos < len(code_lines):
            code_lines = code_lines[last_line_pos:]
            code_lines[-1] = code_lines[-1][last_column_pos:]
        for cl in code_lines:
            ttk = (cl, code_style["normal"])
            tokens.append(ttk)
            tokens.append(('\n', code_style["normal"]))
        tokens.pop()

    return tokens


def split_lines(tokens: list[(str, Color)]) -> list[list[(str, Color)]]:
    lines = []
    line = []
    for tk in tokens:
        if tk[0] == "\n":
            if len(line) == 0:
                lines.append([("", code_style["normal"])])
            else:
                lines.append(line)
            line = []
        else:
            line.append(tk)
    lines.append(line)
    return lines


def build_code_text_widget(code_str: str, code_widget: MultiChildWidget, *, font_size=16) -> Widget:
    lines = split_lines(split_token_with_tokenize(code_str))
    code_widget.rm_child_all()

    for line in lines:
        line_widget = mk_widget(widgets.RichTextLineWidget)
        for tk in line:
            span_w = textspan(tk[0], font_color=tk[1], font_size=font_size)
            line_widget.add_child(span_w)
        code_widget.add_child(line_widget)
    return code_widget


def text_editor(*, text: str = "", font_size=16) -> Widget:

    text_editor_state = {
        "text": text,
        "cursor_index": 0,
        "cursor_pos": Point(0, 0),
        "widget": None,
        "font_size": font_size,
        "blink_handle": 0
    }

    def _update_cursor_pos():
        cursor_idx = text_editor_state["cursor_index"]
        cursor_pos = rich_text_w.index_to_pos(cursor_idx)

        text_editor_state["cursor_pos"] = cursor_pos
        cursor_w.left(cursor_pos.x)
        cursor_w.top(cursor_pos.y)

    def _on_input(codepoint: int):
        ts: str = text_editor_state["text"]
        cursor_idx = text_editor_state["cursor_index"]

        ts = ts[:cursor_idx] + chr(codepoint) + ts[cursor_idx:]

        text_editor_state["text"] = ts
        text_editor_state["cursor_index"] = cursor_idx + 1

        build_code_text_widget(ts, text_editor_state["widget"], font_size=text_editor_state["font_size"])
        cursor_color.set(Color(0, 0, 0, 0))

    def _on_down(mouse_key, pos):
        focus_l.focus()
        cursor_idx = rich_text_w.pos_to_index(pos - rich_text_w.abs_pos())
        text_editor_state["cursor_index"] = cursor_idx

    def _on_key(key, mode):
        editor_text: str = text_editor_state["text"]
        cursor_idx = text_editor_state["cursor_index"]

        text_editor_state["text"] = editor_text
        if mode == KeyboardKeyMode.press:
            if key == KeyboardKey.enter:
                editor_text = editor_text[:cursor_idx] + "\n" + editor_text[cursor_idx:]
                text_editor_state["text"] = editor_text
                text_editor_state["cursor_index"] = cursor_idx + 1
            elif key == KeyboardKey.tab:
                editor_text = editor_text[:cursor_idx] + "    " + editor_text[cursor_idx:]
                text_editor_state["text"] = editor_text
                text_editor_state["cursor_index"] = cursor_idx + 4
            elif key == KeyboardKey.backspace:
                ti = cursor_idx if cursor_idx == 0 else cursor_idx - 1
                editor_text = editor_text[:ti] + editor_text[cursor_idx:]
                text_editor_state["text"] = editor_text
                text_editor_state["cursor_index"] = ti
            elif key == KeyboardKey.delete_:
                editor_text = editor_text[:cursor_idx] + editor_text[cursor_idx + 1:]
                text_editor_state["text"] = editor_text
            elif key == KeyboardKey.left:
                ti = cursor_idx if cursor_idx == 0 else cursor_idx - 1
                text_editor_state["cursor_index"] = ti
            elif key == KeyboardKey.right:
                ti = len(editor_text) if cursor_idx >= len(editor_text) else cursor_idx + 1
                text_editor_state["cursor_index"] = ti

            build_code_text_widget(editor_text, text_editor_state["widget"], font_size=text_editor_state["font_size"])
            cursor_color.set(Color(0, 0, 0, 0))

    def _on_focus(focusd: bool):
        def _blink_cb(timeout):
            _update_cursor_pos()
            cursor_color.set(Color(0, 0, 0, 255) if cursor_color.get().a == 0 else Color(0, 0, 0, 0))

        if focusd:
            text_editor_state["blink_handle"] = Application.set_interval(0.3333, _blink_cb)
        else:
            Application.clear_interval(text_editor_state["blink_handle"])

    rich_text_w: widgets.RichTextLineWidget = mk_widget(widgets.RichTextWidget)
    rich_text_w.gap(int(font_size * 0.3))

    cursor_color = State(Color(0, 0, 0, 255))
    cursor_pos = text_editor_state["cursor_pos"]
    cursor_vsep_w = vseparator(color=cursor_color).width(2)
    cursor_size_w = sized(child=cursor_vsep_w, width=1.0, height=font_size)
    cursor_w = margin(child=cursor_size_w, left=cursor_pos.x, top=cursor_pos.y)

    stack_w = stack(children=[rich_text_w, cursor_w], align_x=-1, align_y=-1)

    char_l = char_listener(child=stack_w, on_input=_on_input)
    mouse_l = mouse_listener(child=char_l, on_down=_on_down)
    keyboard_l = keyboard_linstener(child=mouse_l, on_key=_on_key)
    focus_l = focus_listener(child=keyboard_l, on_focus=_on_focus)

    text_editor_state["widget"] = rich_text_w

    build_code_text_widget(text, rich_text_w, font_size=font_size)

    return focus_l
