from typing import Callable, overload

from .signals import Signal


class Property:
    on_change: Signal
    def get(self) -> object: ...
    def set(self, arg: object) -> Property: ...
    def set_no_notify(self, arg: object) -> Property: ...
    @overload
    def __call__() -> object: ...
    @overload
    def __call__(arg: object) -> Property: ...
