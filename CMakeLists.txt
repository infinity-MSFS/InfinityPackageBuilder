cmake_minimum_required(VERSION 3.28)
project(InfinityPackageBuilder)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

string(ASCII 27 Esc)
set(ColorReset "${Esc}[m")
set(ColourBold "${Esc}[1m")
set(Red "${Esc}[31m")
set(Green "${Esc}[32m")
set(Yellow "${Esc}[33m")
set(Blue "${Esc}[34m")
set(Magenta "${Esc}[35m")
set(Cyan "${Esc}[36m")
set(White "${Esc}[37m")
set(BoldRed "${Esc}[1;31m")
set(BoldGreen "${Esc}[1;32m")
set(BoldYellow "${Esc}[1;33m")
set(BoldBlue "${Esc}[1;34m")
set(BoldMagenta "${Esc}[1;35m")
set(BoldCyan "${Esc}[1;36m")
set(BoldWhite "${Esc}[1;37m")


message(STATUS "${BoldMagenta}Checking build Environment: ${ColorReset} ")

message("${Blue}Compiler: ${Green}${CMAKE_CXX_COMPILER_ID}${ColorReset}")
message("${Blue}Compiler Version: ${Green}${CMAKE_CXX_COMPILER_VERSION}${ColorReset}")
option(INFINITY_USE_X11 "Build for X11" OFF)
option(INFINITY_USE_WAYLAND "Build for Wayland" OFF)

if (DEFINED ENV{VULKAN_SDK})
    message("${Blue}VULKAN_SDK is set to: ${Green}$ENV{VULKAN_SDK}${ColorReset}")
else ()
    message(FATAL_ERROR "VULKAN_SDK environment variable is not set, please add it to your environment variables! The vulkan SDK can be found in the readme")
endif ()

set(VULKAN_SDK $ENV{VULKAN_SDK})

function(check_and_init_submodule submodule_path submodule_name)
    if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${submodule_path}/.git")
        message("${Yellow}${submodule_name} submodule not initialized. Cloning it now...${ColorReset}")
        execute_process(COMMAND git submodule update --init --recursive ${submodule_path}
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMODULE_RESULT)
        if (NOT GIT_SUBMODULE_RESULT EQUAL "0")
            message(FATAL_ERROR "Failed to initialize submodule: ${submodule_name}. Please ensure Git is available and try again.")
        else ()
            message("${Blue}${submodule_name} initialized successfully.${ColorReset}")
        endif ()
    else ()
        message("${Blue}${submodule_name} is initialized.${ColorReset}")
    endif ()
endfunction()

find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)


check_and_init_submodule("dependencies/glfw" "GLFW")
check_and_init_submodule("dependencies/Boxer" "Boxer")
check_and_init_submodule("dependencies/ImGuiColorTextEdit" "ImGuiColorTextEdit")


if (WIN32)
    message("${Green}Detected Windows${ColorReset}")
    add_compile_definitions(INFINITY_WINDOWS)
elseif (UNIX)
    message(STATUS "${BoldMagenta}Checking Linux specific build Environment: ${ColorReset} ")
    message(WARNING "The Package Builder functionality is only available on Windows. This will create a build that omits its functionality.")

    message("${Blue}INFINITY_USE_X11 is: ${Green}${INFINITY_USE_X11}${ColorReset}")
    message("${Blue}INFINITY_USE_WAYLAND is: ${Green}${INFINITY_USE_WAYLAND}${ColorReset}")

    if (INFINITY_USE_X11 AND INFINITY_USE_WAYLAND)
        message(FATAL_ERROR "Cannot build for both X11 and Wayland. Only one can be selected.")
    elseif (NOT INFINITY_USE_X11 AND NOT INFINITY_USE_WAYLAND)
        message(FATAL_ERROR "Must select either X11 or Wayland. Use -DINFINITY_USE_X11=ON -DINFINITY_USE_WAYLAND=OFF or -DINFINITY_USE_X11=OFF -DINFINITY_USE_WAYLAND=ON for Linux build.")
    endif ()

    if (INFINITY_USE_X11)
        message(STATUS "${Green}Detected Linux with X11${ColorReset}")
        add_compile_definitions(INFINITY_X11)
    elseif (INFINITY_USE_WAYLAND)
        message(STATUS "${Green}Detected Linux with Wayland${ColorReset}")
        add_compile_definitions(INFINITY_WAYLAND)
    endif ()
else ()
    message(FATAL_ERROR "Unsupported OS")
endif ()

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/.env" ENV_FILE)
string(REGEX MATCH "ENCRYPTION_KEY=([A-Za-z0-9]+)" _match "${ENV_FILE}")
set(ENCRYPTION_KEY ${CMAKE_MATCH_1})

if (NOT ENCRYPTION_KEY)
    message(FATAL_ERROR "ENCRYPTION_KEY is not set. Please set it in the .env file")
else ()
    message("${Blue}ENCRYPTION_KEY is set to defined value: ${Green}ENCRYPTION_KEY${ColorReset}")
    add_compile_definitions(ENCRYPTION_KEY="${ENCRYPTION_KEY}")
endif ()


file(GLOB_RECURSE SRC_FILES
        "src/*.c"
        "src/*.cpp"
        "src/*.h"
        "src/*.hpp"
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
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLFW/include
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/stb_image
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/HDiff/include
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/MicroXML/include
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/sol/include
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lua/include
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/ImGuiColorTextEdit
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/json/include
        ${VULKAN_SDK}/include

)

add_executable(InfinityPackageBuilder ${SRC_FILES} ${MICROXML_SOURCES} ${SOL_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/ImGuiColorTextEdit/TextEditor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/ImGuiColorTextEdit/TextEditor.h
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/json/include/nlohmann/json.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/json/include/nlohmann/json_fwd.hpp
        src/renderer/Layer.hpp
        src/renderer/buffer/Buffer.hpp
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
        src/Util/FunctionTraits.hpp
        src/Components/ToggleButton/ToggleButton.hpp
        src/Components/Background/Background.cpp
        src/Components/Background/Background.hpp
        src/Components/Buttons/MenuButton.cpp
        src/Components/Buttons/MenuButton.hpp
        src/renderer/GUI/Vulkan/Vulkan.hpp
        src/Util/GithubRelease.cpp
        src/Util/GithubRelease.hpp
        src/Util/GithubOAuth.cpp
        src/Util/GithubOAuth.hpp
        src/Util/Encrypt.hpp
        src/Util/StateTimer.hpp
)

target_sources(InfinityPackageBuilder PRIVATE ${IMGUI_SOURCES})
target_include_directories(InfinityPackageBuilder PRIVATE ${VULKAN_SDK}/include)

add_subdirectory(dependencies/glfw)
add_subdirectory(dependencies/Boxer)


add_library(GLFW INTERFACE)
target_include_directories(GLFW INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLFW/include)

target_link_libraries(GLFW INTERFACE glfw)


target_compile_definitions(InfinityPackageBuilder PRIVATE
        $<$<CONFIG:Release>:RELEASE_DIST>
)

if (WIN32)
    target_link_options(InfinityPackageBuilder PRIVATE
            $<$<CONFIG:Release>:/SUBSYSTEM:WINDOWS>
            $<$<NOT:$<CONFIG:Release>>:/SUBSYSTEM:CONSOLE>
    )
endif ()

if (WIN32)
    target_link_libraries(InfinityPackageBuilder PRIVATE ${VULKAN_SDK}/Lib/vulkan-1.lib glfw Boxer ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lua/lua.lib CURL::libcurl OpenSSL::SSL OpenSSL::Crypto)
else ()
    target_link_libraries(InfinityPackageBuilder PRIVATE -L${VULKAN_SDK}/lib -lvulkan glfw Boxer ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lua/liblua.a CURL::libcurl OpenSSL::SSL OpenSSL::Crypto)

    add_custom_command(TARGET InfinityPackageBuilder POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lua/liblua.so
            $<TARGET_FILE_DIR:InfinityPackageBuilder>
    )
endif ()
