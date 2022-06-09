
target("01")
set_kind("binary")
add_files("./*.cpp")
add_deps("rolui", "rolui-events", "rolui-widgets", "rolui-glfw")

