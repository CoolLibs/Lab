cmake_minimum_required(VERSION 3.20)

project(Coollab)
set(COOLLAB_VERSION "Beta 19 WIP")
add_executable(${PROJECT_NAME})
add_library(Coollab-Properties INTERFACE) # This is needed by both Coollab and its tests
target_link_libraries(${PROJECT_NAME} PRIVATE Coollab-Properties)

# Choose C++ version
target_compile_features(Coollab-Properties INTERFACE cxx_std_20)

# Set the folder where the executable is created
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

# Set warning level
if(MSVC)
    target_compile_options(Coollab-Properties INTERFACE /W4)
else()
    target_compile_options(Coollab-Properties INTERFACE -Wall -Wextra -Wpedantic -pedantic-errors -Wconversion -Wsign-conversion -Wimplicit-fallthrough)
endif()

# Maybe enable warnings as errors
set(WARNINGS_AS_ERRORS_FOR_COOLLAB OFF CACHE BOOL "ON iff you want to treat warnings as errors") # Might be overriden in the CMake cache

if(WARNINGS_AS_ERRORS_FOR_COOLLAB)
    if(MSVC)
        target_compile_options(Coollab-Properties INTERFACE /WX)
    else()
        target_compile_options(Coollab-Properties INTERFACE -Werror)
    endif()
endif()

# Embed the manifest file into the executable. This manifest tells windows that our app supports long paths
if(WIN32)
    target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/app-resources/Coollab.manifest")
endif()

# Include Cool
# Choose our GPU API and its version
# set(COOL_USE_VULKAN 110)
set(COOL_USE_OPENGL $<IF:$<PLATFORM_ID:Darwin>,410,430>) # Must be >= 330 for ImGui to work properly # We use 410 when building on Mac because it doesn't support higher versions of OpenGL
set(COOL_USE_INSTALLER OFF)
set(COOL_APP_NAME "Coollab")
add_subdirectory(Cool)
target_link_libraries(Coollab-Properties INTERFACE Cool::Core)
cool_setup(${PROJECT_NAME})
Cool__set_app_icon(${PROJECT_NAME} "res/logo.png" "${CMAKE_SOURCE_DIR}/app-resources/icon.rc")

# Include cmd
add_subdirectory(lib/cmd)
target_link_libraries(Coollab-Properties INTERFACE cmd::cmd)

# Include range-v3
target_include_directories(Coollab-Properties INTERFACE lib/range-v3/include)
install(FILES "lib/range-v3/LICENSE.txt" DESTINATION "license/range-v3")

set(COOLLAB_REQUIRE_ALL_FEATURES OFF CACHE BOOL "Some features are optional, like making HTTPS requests to post images to the Coollab gallery. If you don't have the necessary dependencies these features will be disabled. If you want to make sure you have all the features of Coollab enabled, set this option to ON.")
set(COOLLAB_BUILD_WITH_OPENSSL ON CACHE BOOL "Needed to make HTTPS requests to post images to the Coollab gallery.")

# ----------------
# ---OpenSSL---
# ----------------
# Link OpenSSL if we find it. It will be used
# to send images to the Coollab gallery.
# If OpenSSL is not found this feature will be disabled.
# Note: cpp-httplib can only work with OpenSSL versions 3 or 1.1.1
# Set the path to OpenSSL library directory
if(COOLLAB_BUILD_WITH_OPENSSL)
    set(OPENSSL_USE_STATIC_LIBS TRUE)

    if(NOT APPLE) # Disabled on Mac for now, because brew installs versions 3.3 now, which isn't compatible with cpp-httplib
        find_package(OpenSSL 3)

        if(NOT OpenSSL_FOUND)
            if(COOLLAB_REQUIRE_ALL_FEATURES)
                find_package(OpenSSL 1.1.1 REQUIRED)
            else()
                find_package(OpenSSL 1.1.1)
            endif()
        endif()
    endif()

    if(OpenSSL_FOUND)
        message("[Coollab] Found OpenSSL.")
        target_link_libraries(Coollab-Properties INTERFACE OpenSSL::SSL OpenSSL::Crypto)
        target_compile_definitions(Coollab-Properties INTERFACE "COOLLAB_HAS_OPENSSL")
    else()
        message("[Coollab] Did not find OpenSSL. Posting images to the Coollab gallery will be disabled.")
    endif()
else()
    message("[Coollab] Disabled building with OpenSSL.")
endif()

install(FILES "lib/cpp-httplib/LICENSE" DESTINATION "license/cpp-httplib")
install(FILES "LICENSE.txt" DESTINATION "license" RENAME "Coollab-LICENSE.txt")

# Grab all the source files
file(GLOB_RECURSE COOLLAB_SOURCES CONFIGURE_DEPENDS src/*)
set(COOLLAB_VERSION_STRING "\"${COOLLAB_VERSION}\"")
configure_file("src/Dump/coollab_version.cpp.in" "src/Dump/coollab_version.cpp")
list(APPEND COOLLAB_SOURCES "src/Dump/coollab_version.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${COOLLAB_SOURCES})

# Set include directories
target_include_directories(Coollab-Properties INTERFACE src)
target_include_directories(Coollab-Properties SYSTEM INTERFACE lib)

# Copy resources to the output folder (where the executable is created) after each build
# and register them for installation when creating the installer.
include("Cool/CMakeUtils/files_and_folders.cmake")

function(Lab_copy_folder FOLDER_RELATIVE_PATH)
    Cool__target_copy_folder(${PROJECT_NAME} ${FOLDER_RELATIVE_PATH})
    install(DIRECTORY ${FOLDER_RELATIVE_PATH} DESTINATION bin)
endfunction()

Lab_copy_folder("res")
Lab_copy_folder("User data Default")
Lab_copy_folder("User data Init")
Lab_copy_folder("Nodes")

if(MSVC)
    target_compile_options(Coollab-Properties INTERFACE /bigobj) # We use too many templates apparently so we need this flag O:)
endif()

# ---------------------
# ---Setup the tests---
# ---------------------
list(REMOVE_ITEM COOLLAB_SOURCES ${CMAKE_SOURCE_DIR}/src/main.cpp)
add_executable(Tests-Coollab tests/tests.cpp ${COOLLAB_SOURCES})
target_compile_definitions(Tests-Coollab PRIVATE LAB_ENABLE_TESTS)
target_link_libraries(Tests-Coollab PRIVATE Coollab-Properties)
target_link_libraries(Tests-Coollab PRIVATE doctest::doctest)
set_target_properties(Tests-Coollab PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/tests/${CMAKE_BUILD_TYPE})
cool_setup(Tests-Coollab)
