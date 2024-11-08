cmake_minimum_required(VERSION 3.28)
project(InfinityPackageBuilder)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/util.cmake)
include(cmake/colors.cmake)
include(FetchContent)

message(STATUS "${BoldMagenta}Checking build Environment: ${ColorReset} ")

message("${Blue}Project: ${Green}${CMAKE_PROJECT_NAME}${ColorReset}")
message("${Blue}Release Profile: ${Green}${CMAKE_BUILD_TYPE}${ColorReset}")
message("${Blue}Compiler: ${Green}${CMAKE_CXX_COMPILER_ID}${ColorReset}")
message("${Blue}Compiler Version: ${Green}${CMAKE_CXX_COMPILER_VERSION}${ColorReset}")
option(INFINITY_USE_X11 "Build for X11" OFF)
option(INFINITY_USE_WAYLAND "Build for Wayland" OFF)

# --- util.cmake ---
check_and_init_submodule("dependencies/ImGuiColorTextEdit" "ImGuiColorTextEdit")

FetchContent_Declare(GLFW GIT_REPOSITORY https://github.com/TheCherno/glfw.git)
FetchContent_Declare(Boxer GIT_REPOSITORY https://github.com/aaronmjacobs/Boxer.git)

FetchContent_MakeAvailable(GLFW)
FetchContent_MakeAvailable(Boxer)

find_package(Vulkan REQUIRED)
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)

print_system_dependencies_info()

# --- util.cmake ---
load_encryption_key_from_env_file()

if (WIN32)
    message("${Green}Detected Windows${ColorReset}")
    add_compile_definitions(INFINITY_WINDOWS)
elseif (UNIX)
    message(STATUS "${BoldMagenta}Checking Linux specific build Environment: ${ColorReset} ")
    message("${Yellow}The Package Builder functionality is only available on Windows. This will create a build that omits its functionality.${ColorReset}")

    if (INFINITY_USE_X11 AND INFINITY_USE_WAYLAND)
        message(FATAL_ERROR "Cannot build for both X11 and Wayland. Only one can be selected.")
    elseif (INFINITY_USE_X11)
        message("${Blue}Display Protocol: ${Green}X11${ColorReset}")
        add_compile_definitions(INFINITY_X11)
    elseif (INFINITY_USE_WAYLAND)
        message("${Blue}Display Protocol: ${Green}Wayland${ColorReset}")
        add_compile_definitions(INFINITY_WAYLAND)
    else ()
        message(FATAL_ERROR "Must select either X11 or Wayland. Use -DINFINITY_USE_X11=ON -DINFINITY_USE_WAYLAND=OFF or -DINFINITY_USE_X11=OFF -DINFINITY_USE_WAYLAND=ON for Linux build.")
    endif ()
else ()
    message(FATAL_ERROR "Unsupported OS")
endif ()


message(STATUS "${BoldMagenta}Infinity configuration complete, Configuring dependencies")
message("${Cyan}")

add_executable(InfinityPackageBuilder
        src/main.cpp
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

# --- util.cmake ---
file(GLOB_RECURSE IMGUI_SOURCES
        dependencies/imgui/imgui.cpp
        dependencies/imgui/imgui_draw.cpp
        dependencies/imgui/imgui_widgets.cpp
        dependencies/imgui/imgui_tables.cpp
        dependencies/imgui/misc/cpp/imgui_stdlib.cpp
        dependencies/imgui/backends/imgui_impl_glfw.cpp
        dependencies/imgui/backends/imgui_impl_vulkan.cpp
        dependencies/ImGuiColorTextEdit/TextEditor.cpp
        dependencies/ImGuiColorTextEdit/TextEditor.h
)
file(GLOB_RECURSE MICROXML_SOURCES dependencies/MicroXML/include/MicroXML/*.hpp dependencies/MicroXML/src/*.cpp)
file(GLOB STB_IMAGE dependencies/stb_image/stb_image.h)
file(GLOB SOL_SOURCES dependencies/sol/include/sol/config.hpp dependencies/sol/include/sol/forward.hpp dependencies/sol/include/sol/sol.hpp)
file(GLOB JSON_SOURCES dependencies/json/include/nlohmann/json.hpp dependencies/json/include/nlohmann/json_fwd.hpp)
target_sources(InfinityPackageBuilder PRIVATE ${MICROXML_SOURCES} ${SOL_SOURCES} ${STB_IMAGE} ${IMGUI_SOURCES} ${JSON_SOURCES})

target_include_directories(InfinityPackageBuilder PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/ini
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/stb_image
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/HDiff/include
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/MicroXML/include
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/sol/include
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lua/include
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/ImGuiColorTextEdit
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/json/include
)


target_compile_definitions(InfinityPackageBuilder PRIVATE
        $<$<CONFIG:Release>:RELEASE_DIST>
)

if (WIN32)
    target_link_options(InfinityPackageBuilder PRIVATE
            $<$<CONFIG:Release>:/SUBSYSTEM:WINDOWS>
            $<$<NOT:$<CONFIG:Release>>:/SUBSYSTEM:CONSOLE>
    )
    set(LUA_LIB ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lua/lua.lib)
else ()
    add_custom_command(TARGET InfinityPackageBuilder POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lua/liblua.so
            $<TARGET_FILE_DIR:InfinityPackageBuilder>
    )
    set(LUA_LIB ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lua/liblua.a)
endif ()

target_link_libraries(InfinityPackageBuilder PRIVATE Vulkan::Vulkan glfw Boxer ${LUA_LIB} CURL::libcurl OpenSSL::SSL OpenSSL::Crypto)