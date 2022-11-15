cmake_minimum_required (VERSION 3.20) # https://cmake.org/download/ (3.20 https://github.com/Kitware/CMake/releases/tag/v3.20.0)
include(FetchContent)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(BUILD_SHARED_LIBS OFF)
#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
set(CMAKE_FIND_LIBRARY_SUFFIXES .a .lib)

project(Injector VERSION 1.0 LANGUAGES C CXX ASM_MASM)
file(GLOB_RECURSE SRC_FILES src/**.hpp src/**.cpp src/**.asm src/**.h src/**.c)

add_executable(Injector ${SRC_FILES})
target_precompile_headers(Injector PRIVATE "src/pch.hpp")
target_include_directories(Injector PRIVATE "src/")

target_compile_definitions(Injector PRIVATE
    "_CRT_SECURE_NO_WARNINGS"
    "WIN32_LEAN_AND_MEAN"
    "_WIN32_WINNT=0x601"
)

set_property(TARGET Injector PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time") # Shows compile time of each file/command