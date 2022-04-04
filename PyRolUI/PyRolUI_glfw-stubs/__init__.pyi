from typing import Tuple, Callable
from PyRolUI import Point, Size, IPainter


class GLFWppWindow:
    def __init__(self, x: int, y: int, title: float): ...
    def make_opengl_context() -> bool: ...
    def poll_events(): ...
    def wait_events(): ...
    def wait_events_timeout(timeout: float): ...
    def set_title(title: str): ...
    def set_size(x: int, y: int): ...
    def window_pos() -> Tuple[int, int]: ...
    def x() -> int: ...
    def y() -> int: ...
    def window_size() -> Tuple[int, int]: ...
    def width() -> int: ...
    def height() -> int: ...
    def cursor_pos() -> Tuple[float, float]: ...
    def should_close() -> bool: ...
    def hide(): ...
    def show(): ...
    def swap_buffer(): ...
    def close(): ...
    on_exit: Callable
    on_size: Callable[[int, int]]
    on_char: Callable[[int]]
    on_cursor_enter: Callable
    on_cursor_leave: Callable
    on_cursor_pos: Callable[[float, float]]
    on_mouse_button: Callable[[int, int, int]]
    on_scroll: Callable[[float, float]]


class GLFWWindow(GLFWppWindow):
    def __init__(self, x: int, y: int, title: float): ...
    def pos() -> Point: ...
    def size() -> Size: ...
    def painter() -> IPainter: ...
    def begin_draw(): ...
    def end_draw(): ...
    def dispatch_event(timeout: float): ...