add_rules("mode.debug", "mode.release")
add_requires("glfw", "glad", "glew", "glm")
set_languages("clatest", "cxx20")
add_defines( "UNICODE", "_UNICODE")
add_cxflags("/execution-charset:utf-8")

target("HazelEngine")
    set_kind("binary")
    add_files("Engine/src/*.cpp")
    add_files("Engine/src/Core/*.cpp")
    add_files("Engine/src/Renderer/*.cpp")
    add_files("Engine/src/Vendor/**/*.cpp")
    add_packages("glfw", "glad", "glew", "glm")
    add_includedirs("C:/Program Files (x86)/Windows Kits/10/Include/10.0.22621.0/um")
    add_includedirs("Engine/src/");
    add_syslinks("kernel32.lib","user32.lib","gdi32.lib","winspool.lib","comdlg32.lib","advapi32.lib")
    add_syslinks("shell32.lib","ole32.lib", "oleaut32.lib","uuid.lib","odbc32.lib","odbccp32.lib")


--
-- If you want to known more usage about xmake, please see https://xmake.io
--
-- ## FAQ
--
-- You can enter the project directory firstly before building project.
--
--   $ cd projectdir
--
-- 1. How to build project?
--
--   $ xmake
--
-- 2. How to configure project?
--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
-- 3. Where is the build output directory?
--
--   The default output directory is `./build` and you can configure the output directory.
--
--   $ xmake f -o outputdir
--   $ xmake
--
-- 4. How to run and debug target after building project?
--
--   $ xmake run [targetname]
--   $ xmake run -d [targetname]
--
-- 5. How to install target to the system directory or other output directory?
--
--   $ xmake install
--   $ xmake install -o installdir
--
-- 6. Add some frequently-used compilation flags in xmake.lua
--
-- @code
--    -- add debug and release modes
--    add_rules("mode.debug", "mode.release")
--
--    -- add macro definition
--    add_defines("NDEBUG", "_GNU_SOURCE=1")
--
--    -- set warning all as error
--    set_warnings("all", "error")
--
--    -- set language: c99, c++11
--    set_languages("c99", "c++11")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--    add_includedirs("/usr/include", "/usr/local/include")
--
--    -- add link libraries and search directories
--    add_links("tbox")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add system link libraries
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
--    add_cxflags("-stdnolib", "-fno-strict-aliasing")
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})
--
-- @endcode
--

