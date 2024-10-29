cmake_minimum_required(VERSION 3.28)
project(InfinityPackageBuilder)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)

file(GLOB_RECURSE SRC_FILES
        "src/*.c"
        "src/*.cpp"
        "src/*.h"
        "src/*.hpp"
        "dependencies/glm/glm/*.c"
        "dependencies/glm/glm/*.cpp"
        "dependencies/glm/glm/*.h"
        "dependencies/glm/glm/*.hpp"
        "dependencies/stb_image/*.h"
)

set(IMGUI_SOURCES
        dependencies/imgui/imgui.cpp
        dependencies/imgui/imgui_draw.cpp
        dependencies/imgui/imgui_widgets.cpp
        dependencies/imgui/imgui_tables.cpp
        dependencies/imgui/misc/cpp/imgui_stdlib.cpp
        dependencies/imgui/backends/imgui_impl_glfw.cpp
        dependencies/imgui/backends/imgui_impl_vulkan.cpp

)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLFW/include
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/stb_image
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/minizip
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/HDiff
        "C:/VulkanSDK/1.3.283.0/Include"
        ${IncludeDir.glm}
)

add_executable(InfinityPackageBuilder ${SRC_FILES}
        src/renderer/Layer.hpp
        src/renderer/buffer/buffer.hpp
        src/renderer/serialization/StreamWriter.cpp
        src/renderer/serialization/StreamWriter.hpp
        src/renderer/serialization/StreamReader.cpp
        src/renderer/serialization/StreamReader.hpp
        src/renderer/serialization/FileStream.cpp
        src/renderer/serialization/FileStream.hpp
        src/renderer/serialization/BufferStream.cpp
        src/renderer/serialization/BufferStream.hpp)
