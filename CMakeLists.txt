cmake_minimum_required(VERSION 3.28)
project(InfinityPackageBuilder)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (DEFINED ENV{VULKAN_SDK})
    message(STATUS "VULKAN_SDK is set to: $ENV{VULKAN_SDK}")
else ()
    message(FATAL_ERROR "VULKAN_SDK environment variable is not set, please add it to your environment variables! The vulkan SDK can be found in the readme")
endif ()

set(VULKAN_SDK $ENV{VULKAN_SDK})

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
        "dependencies/MicroXML/include/MicroXML/*.hpp"
        "dependencies/MicroXML/src/*.cpp"


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
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/MicroXML/include/MicroXML/MicroXML.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/MicroXML/include/MicroXML/NodeSet.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/MicroXML/include/MicroXML/NodeXML.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/MicroXML/include/MicroXML/utils.hpp
)

set(SOL_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/sol/include/sol/config.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/sol/include/sol/forward.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/sol/include/sol/sol.hpp
)


include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glm
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLFW/include
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/stb_image
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/minizip
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/HDiff/include
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/MicroXML/include
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/sol/include
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lua/include
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/ImGuiColorTextEdit
        ${VULKAN_SD}/include
        ${IncludeDir.glm}
)

add_executable(InfinityPackageBuilder ${SRC_FILES} ${MICROXML_SOURCES} ${SOL_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/ImGuiColorTextEdit/TextEditor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/ImGuiColorTextEdit/TextEditor.h
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
        src/Resources/Images/InfinityAppIcon.h
        src/PackageParser/PackageParser.cpp
        src/PackageParser/PackageParser.hpp
        src/Router/Router.cpp
        src/Router/Router.hpp
        src/Components/Menu/Menu.cpp
        src/Components/Menu/Menu.hpp
        src/Pages/LauncherJsonManager/LauncherJsonManager.cpp
        src/Pages/LauncherJsonManager/LauncherJsonManager.hpp
        src/Pages/PackageBuilder/PackageBuilder.cpp
        src/Pages/PackageBuilder/PackageBuilder.hpp
        src/Pages/PackageDiffer/PackageDiffer.cpp
        src/Pages/PackageDiffer/PackageDiffer.hpp
        src/Pages/ReleasePublisher/PackagePublisher.cpp
        src/Pages/ReleasePublisher/PackagePublisher.hpp
        src/Pages/Settings/Settings.cpp
        src/Pages/Settings/Settings.hpp
        src/Pages/Page.hpp
        src/Util/Error.hpp
        src/Util/LuaInterpreter.cpp
        src/Util/LuaInterpreter.hpp
        src/Util/FunctionTraits.hpp)

target_sources(InfinityPackageBuilder PRIVATE ${IMGUI_SOURCES})
target_include_directories(InfinityPackageBuilder PRIVATE ${VULKAN_SDK}/include)

add_subdirectory(dependencies/glfw)
add_subdirectory(dependencies/yaml-cpp)
add_subdirectory(dependencies/glm)
add_subdirectory(dependencies/Boxer)


add_library(GLFW INTERFACE)
target_include_directories(GLFW INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLFW/include)

target_link_libraries(GLFW INTERFACE glfw)


target_compile_definitions(InfinityPackageBuilder PRIVATE
        $<$<CONFIG:Release>:RELEASE_DIST>
)

target_link_options(InfinityPackageBuilder PRIVATE
        $<$<CONFIG:Release>:/SUBSYSTEM:WINDOWS>
        $<$<NOT:$<CONFIG:Release>>:/SUBSYSTEM:CONSOLE>
)

if (WIN32)
    target_link_libraries(InfinityPackageBuilder ${VULKAN_SDK}/Lib/vulkan-1.lib glfw yaml-cpp Boxer ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lua/lua.lib)
else ()
    target_link_libraries(InfinityPackageBuilder -L${VULKAN_SDK}/lib -lvulkan glfw Boxer ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lua/liblua.a)

    add_custom_command(TARGET InfinityPackageBuilder POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lua/liblua.so
            $<TARGET_FILE_DIR:InfinityPackageBuilder>
    )
endif ()
