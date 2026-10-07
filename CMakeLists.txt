cmake_minimum_required(VERSION 3.16)
project(menu VERSION 1.0 LANGUAGES CXX)

if(NOT WIN32)
    message(FATAL_ERROR "menu requires Windows")
endif()

add_library(menu SHARED
    Main.cpp MenuItemIntf.cpp WindowMenu.cpp
    ../tp_stub/tp_stub.cpp
)
target_include_directories(menu PRIVATE ../tp_stub)
target_compile_definitions(menu PRIVATE UNICODE _UNICODE WIN32)
target_compile_features(menu PRIVATE cxx_std_11)
target_link_libraries(menu PRIVATE user32)
if(MSVC)
    target_compile_options(menu PRIVATE /utf-8 /Zc:__cplusplus)
    set_property(TARGET menu PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

include(CTest)
if(BUILD_TESTING)
    add_executable(menu_lifetime_test tests/menu_lifetime_test.cpp WindowMenu.cpp ../tp_stub/tp_stub.cpp)
    target_include_directories(menu_lifetime_test PRIVATE . ../tp_stub)
    target_compile_definitions(menu_lifetime_test PRIVATE UNICODE _UNICODE WIN32)
    target_link_libraries(menu_lifetime_test PRIVATE user32)
    if(MSVC)
        target_compile_options(menu_lifetime_test PRIVATE /utf-8)
        set_property(TARGET menu_lifetime_test PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()
    add_test(NAME menu_lifetime COMMAND menu_lifetime_test)
endif()
