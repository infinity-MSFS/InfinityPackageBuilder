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

set(MICROXML_SOURCES
        dependencies/MicroXML/include/MicroXML/MicroXML.hpp
        dependencies/MicroXML/include/MicroXML/NodeSet.hpp
        dependencies/MicroXML/include/MicroXML/NodeXML.hpp
        dependencies/MicroXML/include/MicroXML/utils.hpp
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
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/HDiff/include
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/MicroXML/include
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
        src/renderer/serialization/BufferStream.hpp
        src/renderer/image/Image.cpp
        src/renderer/image/Image.hpp
        src/renderer/GUI/ApplicationGui.cpp
        src/renderer/GUI/ApplicationGui.hpp
        src/ImGuiTheme/ImGuiTheme.cpp
        src/ImGuiTheme/ImGuiTheme.hpp
        src/renderer/GUI/UI/Ui.cpp
        src/renderer/GUI/UI/Ui.hpp
        src/Resources/Images/InfinityAppIcon.h)

target_sources(InfinityPackageBuilder PRIVATE ${IMGUI_SOURCES} ${MICROXML_SOURCES})

add_subdirectory(dependencies/glfw)
add_subdirectory(dependencies/yaml-cpp)

add_library(GLFW INTERFACE)
target_include_directories(GLFW INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLFW/include)
target_link_libraries(GLFW INTERFACE glfw)

link_directories("C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64")

target_link_libraries(
        InfinityPackageBuilder PRIVATE
        yaml-cpp
        glfw
        "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64/fwpuclnt.lib"
        #        zlib
        #        minizip

        ${Vulkan_LIBRARIES}
)

target_compile_definitions(InfinityPackageBuilder PRIVATE
        $<$<CONFIG:Release>:RELEASE_DIST>
)

target_link_options(InfinityPackageBuilder PRIVATE
        $<$<CONFIG:Release>:/SUBSYSTEM:WINDOWS>
        $<$<NOT:$<CONFIG:Release>>:/SUBSYSTEM:CONSOLE>
)