from typing import overload

from PyRolUI import EventType, IEvent, Point, Size, Vec2i, Vec2f


class MouseKeyMode:
    press: MouseKeyMode
    release: MouseKeyMode


class MouseKey:
    unkown: MouseKey
    key1: MouseKey
    key2: MouseKey
    key3: MouseKey
    key4: MouseKey
    key5: MouseKey
    key6: MouseKey
    key7: MouseKey
    key8: MouseKey
    left: MouseKey
    right: MouseKey
    middle: MouseKey


class MouseEvent(IEvent):
    def pos(self) -> Point: ...
    def offset(self) -> Vec2i: ...
    def action(self) -> MouseKey: ...
    def button(self, key: MouseKey) -> MouseKeyMode: ...


class MouseWheelEvent(IEvent):
    def offset(self) -> Vec2i: ...


def MousePosEvent_type() -> EventType: ...
def MousePressEvent_type() -> EventType: ...
def MouseReleaseEvent_type() -> EventType: ...
def MouseEnterEvent_type() -> EventType: ...
def MouseLeaveEvent_type() -> EventType: ...
def MouseWheelEvent_type() -> EventType: ...


class CharEvent(IEvent):
    @staticmethod
    def type() -> EventType: ...
    def codepoint(self) -> int: ...
    def c_str() -> str: ...


def CharEvent_type() -> EventType: ...


class KeyboardKeyMode:
    press: KeyboardKeyMode
    release: KeyboardKeyMode


class KeyboardKey:
    unknown: KeyboardKey
    space: KeyboardKey
    apostrophe: KeyboardKey     # '
    comma: KeyboardKey          # ,
    minus: KeyboardKey          # -
    period: KeyboardKey         # .
    slash: KeyboardKey          # /
    _0: KeyboardKey
    _1: KeyboardKey
    _2: KeyboardKey
    _3: KeyboardKey
    _4: KeyboardKey
    _5: KeyboardKey
    _6: KeyboardKey
    _7: KeyboardKey
    _8: KeyboardKey
    _9: KeyboardKey
    semicolon: KeyboardKey
    equal: KeyboardKey          # =
    a: KeyboardKey
    b: KeyboardKey
    c: KeyboardKey
    d: KeyboardKey
    e: KeyboardKey
    f: KeyboardKey
    g: KeyboardKey
    h: KeyboardKey
    i: KeyboardKey
    j: KeyboardKey
    k: KeyboardKey
    l: KeyboardKey
    m: KeyboardKey
    n: KeyboardKey
    o: KeyboardKey
    p: KeyboardKey
    q: KeyboardKey
    r: KeyboardKey
    s: KeyboardKey
    t: KeyboardKey
    u: KeyboardKey
    v: KeyboardKey
    w: KeyboardKey
    x: KeyboardKey
    y: KeyboardKey
    z: KeyboardKey
    left_bracket: KeyboardKey   # [
    backslash: KeyboardKey      # \
    right_bracket: KeyboardKey  # ]
    grave_accent: KeyboardKey   # `
    escape: KeyboardKey
    enter: KeyboardKey
    tab: KeyboardKey
    backspace: KeyboardKey
    insert: KeyboardKey
    delete_: KeyboardKey
    right: KeyboardKey
    left: KeyboardKey
    down: KeyboardKey
    up: KeyboardKey
    page_up: KeyboardKey
    page_down: KeyboardKey
    home: KeyboardKey
    end: KeyboardKey
    caps_lock: KeyboardKey
    scroll_lock: KeyboardKey
    num_lock: KeyboardKey
    print_screen: KeyboardKey
    pause: KeyboardKey
    f1: KeyboardKey
    f2: KeyboardKey
    f3: KeyboardKey
    f4: KeyboardKey
    f5: KeyboardKey
    f6: KeyboardKey
    f7: KeyboardKey
    f8: KeyboardKey
    f9: KeyboardKey
    f10: KeyboardKey
    f11: KeyboardKey
    f12: KeyboardKey
    f13: KeyboardKey
    f14: KeyboardKey
    f15: KeyboardKey
    f16: KeyboardKey
    f17: KeyboardKey
    f18: KeyboardKey
    f19: KeyboardKey
    f20: KeyboardKey
    f21: KeyboardKey
    f22: KeyboardKey
    f23: KeyboardKey
    f24: KeyboardKey
    f25: KeyboardKey
    kp_0: KeyboardKey
    kp_1: KeyboardKey
    kp_2: KeyboardKey
    kp_3: KeyboardKey
    kp_4: KeyboardKey
    kp_5: KeyboardKey
    kp_6: KeyboardKey
    kp_7: KeyboardKey
    kp_8: KeyboardKey
    kp_9: KeyboardKey
    kp_decimal: KeyboardKey
    kp_divide: KeyboardKey
    kp_multiply: KeyboardKey
    kp_subtract: KeyboardKey
    kp_add: KeyboardKey
    kp_enter: KeyboardKey
    kp_equal: KeyboardKey
    left_shift: KeyboardKey
    left_control: KeyboardKey
    left_alt: KeyboardKey
    left_super: KeyboardKey
    right_shift: KeyboardKey
    right_control: KeyboardKey
    right_alt: KeyboardKey
    right_super: KeyboardKey
    menu: KeyboardKey


class KeyboardEvent(IEvent):
    @staticmethod
    def type() -> EventType: ...
    def action(self) -> KeyboardKey: ...
    @overload
    def key_mode(self) -> KeyboardKeyMode: ...
    @overload
    def key_mode(self, key: KeyboardKey) -> KeyboardKeyMode: ...


def KeyboardEvent_type() -> EventType: ...
