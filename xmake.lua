set_policy("build.warning", true)
add_rules("mode.debug", "mode.release")
set_languages("cxx20")
set_toolchains("gcc")

add_requires("frozen")
add_requires("catch2")

if is_mode("debug") then
    add_defines("DEBUG")
else 
    add_defines("NDEBUG")
end

add_includedirs("includes")
set_warnings("all")
add_cflags("-Werror=incompatible-pointer-types")
add_cflags("-Wno-unused-function")

target("w")
    set_kind("binary")
    add_packages("frozen")
    add_files("src/**.cpp")

target("tests")
    set_kind("binary")
    add_packages("frozen")
    add_packages("catch2")
    add_files("src/**.cpp")
    add_files("tests/**.cpp")
    remove_files("src/main.cpp")
