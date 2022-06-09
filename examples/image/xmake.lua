
target("image")
set_kind("binary")
set_languages("cxx17")
add_files("./*.cpp")
add_deps("rolui", "rolui-events", "rolui-widgets", "rolui-glfw")

