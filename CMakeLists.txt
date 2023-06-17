cmake_minimum_required(VERSION 3.20)

project(Coollab)
add_executable(${PROJECT_NAME})
add_library(Coollab-Properties INTERFACE) # This is needed by both Coollab and its tests
target_link_libraries(${PROJECT_NAME} PRIVATE Coollab-Properties)

# Choose C++ version
target_compile_features(Coollab-Properties INTERFACE cxx_std_20)

# Set the folder where the executable is created
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

# Set warning level
if(MSVC)
    target_compile_options(Coollab-Properties INTERFACE /W4)
else()
    target_compile_options(Coollab-Properties INTERFACE -Wall -Wextra -Wpedantic -pedantic-errors -Wconversion -Wsign-conversion)
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

# Choose our GPU API and its version
# set(COOL_USE_VULKAN 110)
set(COOL_USE_OPENGL $<IF:$<PLATFORM_ID:Darwin>,410,430>) # Must be >= 330 for ImGui to work properly # We use 410 when building on Mac because it doesn't support higher versions of OpenGL

# Include Cool
add_subdirectory(Cool)
target_link_libraries(Coollab-Properties INTERFACE Cool::Core)
cool_setup(${PROJECT_NAME})

# Set app icon
Cool__set_app_icon(${PROJECT_NAME} "${CMAKE_SOURCE_DIR}/app-resources/lab.rc" "res/logo.png")

# Include cmd
add_subdirectory(lib/cmd)
target_link_libraries(Coollab-Properties INTERFACE cmd::cmd)

# Include range-v3
target_include_directories(Coollab-Properties INTERFACE lib/range-v3/include)

# Include OpenCV
# include("${CMAKE_SOURCE_DIR}/lib/easy_opencv/target_link_opencv.cmake")
add_subdirectory(lib/easy_opencv)
target_link_opencv(Coollab PRIVATE)
# target_link_libraries(Coollab-Properties INTERFACE easy_opencv::easy_opencv)

set(COOLLAB_REQUIRE_ALL_FEATURES OFF CACHE BOOL "Some features are optional, like making HTTPS requests to post images to the Coollab gallery. If you don't have the necessary dependencies these features will be disabled. If you want to make sure you have all the features of Coollab enabled, set this option to ON.")

# ----------------
# ---OpenSSL---
# ----------------
# Link OpenSSL if we find it. It will be used
# to send images to the Coollab gallery.
# If OpenSSL is not found this feature will be disabled.
# Note: cpp-httplib can only work with OpenSSL versions 3 or 1.1.1
# Set the path to OpenSSL library directory
set(OPENSSL_USE_STATIC_LIBS TRUE)
if(COOLLAB_REQUIRE_ALL_FEATURES)
    find_package(OpenSSL REQUIRED)
else()
    find_package(OpenSSL)
endif()

if(OpenSSL_FOUND)
    message("[Coollab] Found OpenSSL.")
    target_link_libraries(Coollab-Properties INTERFACE OpenSSL::SSL OpenSSL::Crypto)
    target_compile_definitions(Coollab-Properties INTERFACE "COOLLAB_HAS_OPENSSL")
else()
    message("[Coollab] Did not find OpenSSL. Posting images to the Coollab gallery will be disabled.")
endif()

# Grab all the source files
file(GLOB_RECURSE COOL_LAB_SOURCES CONFIGURE_DEPENDS src/*)
target_sources(${PROJECT_NAME} PRIVATE ${COOL_LAB_SOURCES})

# Set include directories
target_include_directories(Coollab-Properties SYSTEM INTERFACE src lib)

# Copy resources to the output folder (where the executable is created) after each build
include("Cool/CMakeUtils/files_and_folders.cmake")
Cool__target_copy_folder(${PROJECT_NAME} "res")
Cool__target_copy_folder(${PROJECT_NAME} "Nodes")

if(MSVC)
    target_compile_options(Coollab-Properties INTERFACE /bigobj) # We use too many templates apparently so we need this flag O:)
endif()

# ---------------------
# ---Setup the tests---
# ---------------------
list(REMOVE_ITEM COOL_LAB_SOURCES ${CMAKE_SOURCE_DIR}/src/main.cpp)
add_executable(Tests-Coollab tests/tests.cpp ${COOL_LAB_SOURCES})
target_compile_definitions(Tests-Coollab PRIVATE LAB_ENABLE_TESTS)
target_link_libraries(Tests-Coollab PRIVATE Coollab-Properties)
target_link_libraries(Tests-Coollab PRIVATE doctest::doctest)
target_link_opencv(Tests-Coollab PRIVATE)