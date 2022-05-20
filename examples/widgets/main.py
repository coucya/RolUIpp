import os
import sys

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__) + "../../../PyRolUI")))

import PyRolUI
from PyRolUI import *
from PyRolUI import widgets
from PyRolUI import functional_component as fc
from PyRolUI import popup, menu, xml_tool as xt
from PyRolUI.xml_tool import *


from PyRolUI_glfw import *

import vscroll_panel


def load_file(path: str) -> str:
    with open(path, "r", encoding="utf8") as f:
        return f.read()

win = GLFWWindow(800, 600, "Python test.")
win.on_exit = lambda: Application.exit()

Application.init(win)
load_font("default", "C:\\WINDOWS\\FONTS\\MSYHL.TTC")


context = {
    "progress_value": fc.State(0),
    "button1": lambda: fc.dialog("按下按钮1"),
    "button2": lambda: fc.dialog("按下按钮2"),
    "button3": lambda: fc.dialog("按下按钮3"),
    "button4": lambda: fc.dialog("按下按钮4"),
}

xml_str = load_file(os.path.join(os.path.dirname(__file__), "ui.xml"))
w = build_widget_from_xml(xml_str, context)


def _cb(d):
    ps = context["progress_value"]
    ps.set(ps.get() + 1)
    if ps.get() >= 100:
        ps.set(0)


Application.set_interval(0.03, _cb)


Application.run(w)
