cmake_minimum_required(VERSION 3.29)

include(cmake/util.cmake)
include(cmake/colors.cmake)
include(cmake/key_loading.cmake)
include(FetchContent)

if (NOT VCPKG_ROOT)
    set(VCPKG_ROOT $ENV{VCPKG_ROOT})
endif ()

if (VCPKG_ROOT)
    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
    message("${Blue}Using Toolchain file: ${Green}${CMAKE_TOOLCHAIN_FILE}${ColorReset}")
else ()
    message(FATAL_ERROR "VCPKG_ROOT is not set in the environment!")
endif ()

if (WIN32)
    set(VCPKG_TRIPLET "x64-windows-static")
else ()
    set(VCPKG_TRIPLET "x64-linux-static")
endif ()


project(InfinityPackageBuilder)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Vulkan REQUIRED)
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(unofficial-minizip CONFIG REQUIRED)
find_package(zoe CONFIG REQUIRED PATHS "${VCPKG_SOURCE_PATH}/packages/zoe_${VCPKG_TRIPLET}/")
find_package(msgpack-cxx CONFIG REQUIRED)
find_package(ZLIB REQUIRED)
if (WIN32)
    find_package(unofficial-wintoast REQUIRED)
endif ()

print_system_dependencies_info()

message(STATUS "${BoldMagenta}Checking build Environment: ${ColorReset} ")

message("${Blue}Project: ${Green}${CMAKE_PROJECT_NAME}${ColorReset}")
message("${Blue}Release Profile: ${Green}${CMAKE_BUILD_TYPE}${ColorReset}")
message("${Blue}Compiler: ${Green}${CMAKE_CXX_COMPILER_ID}${ColorReset}")
message("${Blue}Compiler Version: ${Green}${CMAKE_CXX_COMPILER_VERSION}${ColorReset}")
option(INFINITY_USE_X11 "Build for X11" OFF)
option(INFINITY_USE_WAYLAND "Build for Wayland" OFF)


FetchContent_Declare(GLFW GIT_REPOSITORY https://github.com/TheCherno/glfw.git)
FetchContent_Declare(Boxer GIT_REPOSITORY https://github.com/aaronmjacobs/Boxer.git)
FetchContent_Declare(Infinity GIT_REPOSITORY https://github.com/Tacotakedown/InfinityPackageBuilderDeps.git)
if (UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(DBUS REQUIRED dbus-1)
    FetchContent_Declare(MiniKDENotify GIT_REPOSITORY https://github.com/rikka-gh/MiniKDENotify.git)

    FetchContent_MakeAvailable(GLFW Boxer Infinity MiniKDENotify)
    option(MINIKDENOTIFY_BUILD_EXAMPLES OFF)
else ()
    FetchContent_MakeAvailable(GLFW Boxer Infinity)
endif ()

message("${Blue}Fetched:${Green}GLFW ${Blue}set to:${Green}${glfw_SOURCE_DIR}${ColorReset}")
message("${Blue}Fetched:${Green}Boxer ${Blue}set to:${Green}${boxer_SOURCE_DIR}${ColorReset}")
message("${Blue}Fetched:${Green}InfinityDeps ${Blue}set to:${Green}${infinity_SOURCE_DIR}${ColorReset}")
if (UNIX)
    message("${Blue}Fetched:${Green}MiniKdeNotify ${Blue}set to:${Green}${minikdenotify_SOURCE_DIR}${ColorReset}")
endif ()

load_keys()

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
        src/Util/StateTimer.hpp
        src/Util/ZipUtil.cpp
        src/Util/ZipUtil.hpp
        src/Util/OpenInBrowser.hpp
        src/Pages/LauncherJsonManager/Serialization/BinaryStructure.hpp
        src/Pages/LauncherJsonManager/Serialization/Serialization.cpp
        src/Pages/LauncherJsonManager/Serialization/Serialization.hpp
        src/Util/Notifications.cpp
        src/Util/Notifications.hpp
        src/Util/Encryption/Encryption.cpp
        src/Util/Encryption/Encryption.hpp
)

file(GLOB IMGUI_SOURCES
        ${infinity_SOURCE_DIR}/src/imgui/
        ${infinity_SOURCE_DIR}/src/imgui/imgui.cpp
        ${infinity_SOURCE_DIR}/src/imgui/imgui_draw.cpp
        ${infinity_SOURCE_DIR}/src/imgui/imgui_widgets.cpp
        ${infinity_SOURCE_DIR}/src/imgui/imgui_tables.cpp
        ${infinity_SOURCE_DIR}/src/imgui/misc/cpp/imgui_stdlib.cpp
        ${infinity_SOURCE_DIR}/src/imgui/backends/imgui_impl_glfw.cpp
        ${infinity_SOURCE_DIR}/src/imgui/backends/imgui_impl_vulkan.cpp
        ${infinity_SOURCE_DIR}/src/imguiTextEditor/TextEditor.cpp
        ${infinity_SOURCE_DIR}/src/imguiTextEditor/TextEditor.h
)
file(GLOB_RECURSE MICROXML_SOURCES ${infinity_SOURCE_DIR}/src/MicroXML/*.cpp ${infinity_SOURCE_DIR}/include/MicroXML/*.hpp)
file(GLOB STB_IMAGE ${infinity_SOURCE_DIR}/include/stb_image/stb_image.h)
file(GLOB_RECURSE SOL_SOURCES ${infinity_SOURCE_DIR}/include/Sol2/*.hpp)
file(GLOB JSON_SOURCES ${infinity_SOURCE_DIR}/include/Json/*.hpp)
target_sources(InfinityPackageBuilder PRIVATE ${MICROXML_SOURCES} ${SOL_SOURCES} ${STB_IMAGE} ${IMGUI_SOURCES} ${JSON_SOURCES})


if (UNIX)
    target_include_directories(InfinityPackageBuilder PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
            ${infinity_SOURCE_DIR}/include
            ${infinity_SOURCE_DIR}/include/Lua # for sol header file #include<lua.h>
            ${infinity_SOURCE_DIR}/src/imgui
            ${infinity_SOURCE_DIR}/src/imguiTextEditor
            ${ZOE_INCLUDE_DIR}
            ${MINIZIP_INCLUDE_DIR}
            ${minikdenotify_SOURCE_DIR}/include
            keys/include
            ${DBUS_INCLUDE_DIRS}
    )
else ()
    target_include_directories(InfinityPackageBuilder PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
            ${infinity_SOURCE_DIR}/include
            ${infinity_SOURCE_DIR}/include/Lua # for sol header file #include<lua.h>
            ${infinity_SOURCE_DIR}/src/imgui
            ${infinity_SOURCE_DIR}/src/imguiTextEditor
            ${ZOE_INCLUDE_DIR}
            keys/include
            ${MINIZIP_INCLUDE_DIR}
    )
endif ()

target_compile_definitions(InfinityPackageBuilder PRIVATE
        $<$<CONFIG:Release>:RELEASE_DIST>
)

if (WIN32)
    target_link_options(InfinityPackageBuilder PRIVATE
            $<$<CONFIG:Release>:/SUBSYSTEM:WINDOWS>
            $<$<NOT:$<CONFIG:Release>>:/SUBSYSTEM:CONSOLE>
    )
    set(LUA_LIB ${infinity_SOURCE_DIR}/lib/lua.lib)

    set(TOAST_LIB unofficial::wintoast::wintoast)

    add_custom_command(TARGET InfinityPackageBuilder POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${infinity_SOURCE_DIR}/bin/libcrypto-3-x64.dll
            $<TARGET_FILE_DIR:InfinityPackageBuilder>
    )
else ()
    add_custom_command(TARGET InfinityPackageBuilder POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${infinity_SOURCE_DIR}/lib/liblua.so
            $<TARGET_FILE_DIR:InfinityPackageBuilder>
    )
    set(LUA_LIB ${infinity_SOURCE_DIR}/lib/liblua.a ${DBUS_LIBRARIES})

endif ()


target_link_libraries(InfinityPackageBuilder PRIVATE Vulkan::Vulkan glfw Boxer ${LUA_LIB} CURL::libcurl OpenSSL::SSL OpenSSL::Crypto unofficial::minizip::minizip ${ZOE_LIB} ${TOAST_LIB})