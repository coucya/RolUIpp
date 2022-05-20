
from typing import Callable, overload

from . import widgets, signals, propertys, events


class Vec2i:
    x: int
    y: int


class Vec2f:
    x: float
    y: float


class Size:
    width: int
    height: int


Point = Vec2i


class Rect:
    x: int
    y: int
    width: int
    height: int
    def pos(self) -> int: ...
    def size(self) -> int: ...
    def top(self) -> int: ...
    def bottom(self) -> int: ...
    def left(self) -> int: ...
    def right(self) -> int: ...
    def centre(self) -> int: ...
    def middle(self) -> int: ...
    def left_top(self) -> Point: ...
    def left_bottom(self) -> Point: ...
    def right_top(self) -> Point: ...
    def right_bottom(self) -> Point: ...
    def centre_top(self) -> Point: ...
    def centre_bottom(self) -> Point: ...
    def left_middle(self) -> Point: ...
    def right_middle(self) -> Point: ...
    def centre_middle(self) -> Point: ...
    @overload
    def contain(self, p: Point) -> bool: ...
    @overload
    def contain(self, x: int, y: int) -> bool: ...
    def intersect(self, rect: Rect) -> bool: ...
    def united(self, rect: Rect) -> Rect: ...
    def intersected(self, rect: Rect) -> Rect | None: ...


class Color:
    r: int
    g: int
    b: int
    a: int


class Image:
    def handle(self) -> int: ...
    @staticmethod
    def load(filename: str) -> Image: ...


class EventType:
    def name(self) -> str: ...


class IEvent:
    def is_(self, et: EventType) -> bool: ...
    def event_type(self) -> EventType: ...
    def target(self) -> Widget: ...


class IPainter:
    def load_font(self, name: str, filename: str): ...
    def text_size(self, text: str, text_len: int): ...
    def create_image_with_rgba(self): ...
    def delete_image(self, handle: int): ...
    def image_size(self, handle: int): ...
    def set_scissor(self, rect: Rect): ...
    def get_scissor(self) -> Rect: ...
    def set_font_size(self, size: int): ...
    def set_font_color(self, color: Color): ...
    def set_font(self, name: str): ...
    def set_stroke_color(self, color: Color): ...
    def set_fill_color(self, color: Color): ...
    def set_stroke_width(self, width: int): ...
    def draw_text(self, pos: Point, text: str, text_size: int): ...
    def draw_image(self, pos: Point, handle: int): ...
    def draw_line(self, a: Point, b: Point): ...
    def draw_rect(self, rect: Rect): ...
    def draw_circle(self, centre: Point, r: int): ...
    def draw_ellipse(self, rect: Rect): ...
    def draw_roundedrect(self, rect: Rect, r: int): ...
    def fill_rect(self, rect: Rect): ...
    def fill_circle(self, centre: Point, r: int): ...
    def fill_ellipse(self, rect: Rect): ...
    def fill_roundedrect(self, rect: Rect, r: int): ...
    def draw_hline(self, pos: Point, len: int): ...
    def draw_vline(self, pos: Point, len: int): ...


class Window:
    def pos(self) -> Point: ...
    def size(self) -> Size: ...
    def painter(self) -> IPainter: ...
    def begin_draw(self): ...
    def end_draw(self): ...
    def dispatch_event(self, timeout: float): ...


class Constraint:
    @overload
    def __init__(self) -> None: ...
    @overload
    def __init__(self, min: Size, max: Size) -> None: ...
    @overload
    def __init__(self, min_w: int, min_h: int, max_w: int, max_h: int) -> None: ...

    def min(self) -> Size: ...
    def max(self) -> Size: ...
    def min_width(self) -> int: ...
    def min_height(self) -> int: ...
    def max_width(self) -> int: ...
    def max_height(self) -> int: ...

    @staticmethod
    def zero() -> Constraint: ...
    @overload
    @staticmethod
    def zero_to(max: Size) -> Constraint: ...
    @overload
    @staticmethod
    def zero_to(w: int, h: int) -> Constraint: ...
    @overload
    @staticmethod
    def limit(s: Size) -> Constraint: ...
    @overload
    @staticmethod
    def limit(w: int, h: int) -> Constraint: ...
    @overload
    @staticmethod
    def limit(min: Size, max: Size) -> Constraint: ...
    @overload
    @staticmethod
    def limit(min_w: int, min_h: int, max_w: int, max_h: int) -> Constraint: ...
    @staticmethod
    def unlimit() -> Constraint: ...


class Widget:
    def pos(self) -> Point: ...
    def size(self) -> Size: ...
    def rect(self) -> Rect: ...
    def abs_pos(self) -> Point: ...
    def abs_rect(self) -> Rect: ...
    def opaque(self) -> bool: ...
    def is_hit(self) -> bool: ...
    def mounted(self) -> bool: ...
    def depth(self) -> int: ...
    def parent(self) -> Widget: ...
    def set_opaque(self, val: bool): ...
    def mark_hit(self): ...
    def clear_hit(self): ...
    def clear_hit_self(self): ...
    def layout(self, constraint) -> Size: ...
    def draw(self, painter: IPainter): ...
    def update_pos(self): ...
    def child_count(self) -> int: ...
    def child(self, index: int = 0) -> Widget: ...
    def set_child(self, child: Widget, index: int = 0): ...
    def rm_child(self, index: int = 0): ...
    def hit_test(self, pos: Point) -> bool: ...
    def hit_test_self(self, pos: Point) -> bool: ...
    def hit_test_children(self, pos: Point) -> Widget: ...
    def handle_event(self, e: IEvent) -> bool: ...
    def perform_layout(self, constraint: Constraint) -> Size: ...
    def perform_draw(self, painter: IPainter) -> Size: ...


class SingleChildWidget(Widget):
    def child_count(self) -> int: ...
    def child(self, index: int = 0) -> Widget: ...
    def set_child(self, child: Widget, index: int = 0): ...
    def rm_child(self, index: int = 0): ...


class MultiChildWidget(Widget):
    def child_count(self) -> int: ...
    def child(self, index: int) -> Widget: ...
    def add_child(self, child: Widget): ...
    def set_child(self, child: Widget, index: int): ...
    def insert_child(self, index: int, child: Widget): ...
    @overload
    def rm_child(self, w: Widget): ...
    @overload
    def rm_child(self, index: int): ...
    def rm_child_all(self): ...


class RootWdiget(MultiChildWidget):
    def content_widget(self) -> Widget: ...
    def set_content_widget(self, w: Widget): ...


class Application:
    @staticmethod
    def init(win: Window): ...
    @staticmethod
    def window() -> Window: ...
    @staticmethod
    def root_widget() -> RootWdiget: ...
    @staticmethod
    def has_focus_widget() -> bool: ...
    @staticmethod
    def set_focus_widget(w: Widget): ...
    @staticmethod
    def focus_widget() -> Widget: ...
    @staticmethod
    def set_timeout(timeout: float, cb: Callable[[float], object]) -> int: ...
    @staticmethod
    def set_interval(timeout: float, cb: Callable[[float], object]) -> int: ...
    @staticmethod
    def clear_timeout(handle: int): ...
    @staticmethod
    def clear_interval(handle: int): ...
    @staticmethod
    def get_widget_by_pos(pos: Point) -> Widget: ...
    @staticmethod
    def flush_layout(): ...
    @staticmethod
    def flush_draw(): ...
    @staticmethod
    def flush_frame(): ...
    @staticmethod
    def run(w: Widget): ...
    @staticmethod
    def exit(): ...


def load_font(name: str, filename: str) -> bool: ...

