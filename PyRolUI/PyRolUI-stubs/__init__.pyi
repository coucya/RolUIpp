
from typing import overload

widgets: object


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
    def load(self, filename: str) -> Image: ...


class EventType:
    def name(self) -> str: ...


class IEvent:
    def is_(self, et: EventType) -> bool: ...
    def event_type(self) -> EventType: ...
    def target(self) -> Widget: ...


class IPainter:
    def load_font(self): ...
    def text_size(self): ...
    def create_image_with_rgba(self): ...
    def delete_image(self): ...
    def image_size(self): ...
    def scissor(self): ...
    def get_scissor(self): ...
    def set_font_size(self): ...
    def set_font_color(self): ...
    def set_stroke_color(self): ...
    def set_fill_color(self): ...
    def set_stroke_width(self): ...


class Window:
    def pos(self) -> Point: ...
    def size(self) -> Size: ...
    def painter(self) -> IPainter: ...
    def begin_draw(self): ...
    def end_draw(self): ...
    def dispatch_event(self, timeout: float): ...


class Widget:
    def pos(self) -> Point: ...
    def size(self) -> Size: ...
    def rect(self) -> Rect: ...
    def abs_pos(self) -> Point: ...
    def abs_rect(self) -> Rect: ...
    def parent(self) -> Widget: ...
    def layout(self, constraint) -> Size: ...
    def get_child_by_pos(self, pos: Point) -> Widget: ...
    def handle_event(self, e: IEvent) -> bool: ...
    def draw(self, painter: IPainter): ...
    def perform_layout(self, constraint) -> Size: ...
    def update_pos(self): ...
    def hit_test(self, pos: Point) -> bool: ...


class SingleChildWidget(Widget):
    def child(self) -> Widget: ...
    def set_child(self, child: Widget) -> SingleChildWidget: ...


class MultiChildWidget(Widget):
    def child(self) -> Widget: ...
    def add_child(self, child: Widget) -> MultiChildWidget: ...
    def set_child(self, idx: int, child: Widget) -> MultiChildWidget: ...


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
    def set_timeout(timeout: float, cb: function) -> int: ...
    @staticmethod
    def set_interval(timeout: float, cb: function) -> int: ...
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
