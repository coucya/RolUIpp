import PyRolUI
from PyRolUI import Application, widgets
import PyRolUI_glfw

win = PyRolUI_glfw.GLFWWindow(800, 600, "PyRolUI test.")
win.on_exit = lambda: Application.exit()
Application.init(win)

PyRolUI.load_font("default", "C:\\WINDOWS\\FONTS\\MSYHL.TTC")

w = widgets.align(child=widgets.textbox(text="text",
                                        font_size=50)
                  )
w = widgets.column(children=[
    widgets.text("aaa", size=10),
    widgets.text("aaa", size=20),
    widgets.text("aaa", size=30),
    widgets.text("aaa", size=40),
],
    cross_axis_alignment=1)

dv = 0.1


def cb(d):
    global dv
    w.cross_axis_alignment(w.cross_axis_alignment() + dv)
    if(w.cross_axis_alignment() > 1.0 or w.cross_axis_alignment() < -1.0):
        dv = -dv


Application.set_interval(0.020, cb)
# Application.set_timeout(3.0, cb)

Application.run(w)

print("exit")
