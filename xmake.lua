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
add_requires("catch2");

target("000_Hello")
    set_group("Demos")
    set_kind("binary")
    add_packages("catch2")
    add_files("SPEngine/Samples/000_Hello/*.cpp|Test_*.cpp")

target("Engine")
    set_group("Engine")
    set_kind("shared")
    add_defines("SP_DYNAMIC_DLL", "SP_BUILD_DLL")

    add_files("SPEngine/Source/Engine/*.cpp|Test_*.cpp")

    add_headerfiles("SPEngine/Source/Engine/*.h")

target("Sandbox")
    set_kind("binary")
    add_deps("Engine")
    add_links("Engine")
    add_defines("SP_DYNAMIC_DLL")

    add_files("SPEngine/Sandbox/*.cpp|Test_*.cpp")

    add_includedirs("SPEngine/Source/Engine/")


---TestCase-----
for _, file in ipairs(os.files("SPEngine/**/Test_*.cpp")) do
    local name = path.basename(file)
    target(name)
        set_kind("binary")
        set_default(false)
        -- add_files("src/" .. name .. ".cpp")
        add_files(file)
        add_tests(name)
        add_packages("catch2")
        -- add_tests("args", {runargs = {"foo", "bar"}})
        -- add_tests("pass_output", {trim_output = true, runargs = "foo", pass_outputs = "hello foo"})
        -- add_tests("fail_output", {fail_outputs = {"hello2 .*", "hello xmake"}})
end