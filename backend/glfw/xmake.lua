add_requires("glfw", "glad", "nanovg")

target("rolui-glfw")
set_kind("static")
add_files("./*.cpp")
add_headerfiles("./*.h")
add_includedirs("./", { public = true })
add_deps("rolui", "rolui-events", "rolui-widgets")
add_packages("glfw", "glad", "nanovg", { public = true })
