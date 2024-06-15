add_rules("mode.debug", "mode.release")
set_optimize("none")
-- add_requires("vcpkg::rttr", {alias = "rttr"}) 
-- add_requires("conan::zlib/1.2.11", {alias = "zlib", debug = true})
-- add_requires("glfw", "glew", "glm", "spdlog", "rapidjson")
-- add_requires("imgui 1.90", {configs = {glfw = true, opengl3=true, freetype=true}})
-- add_requires("freetype");
-- add_requires("lunasvg");
set_languages("clatest", "cxx20")
-- add_defines( "UNICODE", "_UNICODE")
add_cxflags("/execution-charset:utf-8", {force=true})
add_cxflags("/wd4819", {force=true})

target("000_Hello")
    set_group("Demos")
    set_kind("binary")
    add_files("SPEngine/Samples/000_Hello/*.cpp")

target("Engine")
    set_group("Engine")
    set_kind("binary")
    set_kind("static")
    add_files("SPEngine/Source/Engine/*.cpp")

    add_headerfiles("SPEngine/Source/Engine/*.h")

target("Sandbox")
    set_kind("binary")
    add_deps("Engine")
    add_links("Engine")
    add_files("SPEngine/Sandbox/*.cpp")

    add_includedirs("SPEngine/Source/Engine/")