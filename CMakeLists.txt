cmake_minimum_required(VERSION 3.20)

project(CoolLab)

add_executable(${PROJECT_NAME})

add_library(CooLab-Properties INTERFACE) # This is needed by both CoolLab and its tests
target_link_libraries(${PROJECT_NAME} PRIVATE CooLab-Properties)

# Choose C++ version
target_compile_features(CooLab-Properties INTERFACE cxx_std_20)

# Set the folder where the executable is created
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

# Set warning level
if(MSVC)
    target_compile_options(CooLab-Properties INTERFACE /W3)
else()
    target_compile_options(CooLab-Properties INTERFACE -Wall -Wextra -Wpedantic -pedantic-errors)
endif()

# Maybe enable warnings as errors
set(WARNINGS_AS_ERRORS_FOR_COOLLAB OFF CACHE BOOL "ON iff you want to treat warnings as errors") # Might be overriden in the CMake cache

if(WARNINGS_AS_ERRORS_FOR_COOLLAB)
    if(MSVC)
        target_compile_options(CooLab-Properties INTERFACE /WX)
    else()
        target_compile_options(CooLab-Properties INTERFACE -Werror)
    endif()
endif()

# Choose our GPU API and its version
# set(COOL_USE_VULKAN 110)
set(COOL_USE_OPENGL $<IF:$<PLATFORM_ID:Darwin>,410,430>) # Must be >= 330 for ImGui to work properly # We use 410 when building on Mac because it doesn't support higher versions of OpenGL

# Include Cool
add_subdirectory(Cool)
target_link_libraries(CooLab-Properties INTERFACE Cool::Core)
cool_setup(${PROJECT_NAME})

# Set app icon (only if exists)
Cool__set_app_icon(${PROJECT_NAME} "${CMAKE_SOURCE_DIR}/app-resources/lab.rc" "res/logo.png")

# Include cmd
add_subdirectory(lib/cmd)
target_link_libraries(CooLab-Properties INTERFACE cmd::cmd)

# Include range-v3
target_include_directories(CooLab-Properties INTERFACE lib/range-v3/include)

# Grab all the source files
file(GLOB_RECURSE COOL_LAB_SOURCES CONFIGURE_DEPENDS src/*)
target_sources(${PROJECT_NAME} PRIVATE ${COOL_LAB_SOURCES})

# Set include directories
target_include_directories(CooLab-Properties INTERFACE src lib)

# Copy resources to the output folder (where the executable is created) after each build
include("Cool/CMakeUtils/files_and_folders.cmake")
Cool__target_copy_folder(${PROJECT_NAME} "res")
Cool__target_copy_folder(${PROJECT_NAME} "Nodes")

if(MSVC)
    target_compile_options(CooLab-Properties INTERFACE /bigobj) # We use too many templates apparently so we need this flag O:)
endif()

# ---------------------
# ---Setup the tests---
# ---------------------
list(REMOVE_ITEM COOL_LAB_SOURCES ${CMAKE_SOURCE_DIR}/src/main.cpp)
add_executable(Tests-CoolLab tests/tests.cpp ${COOL_LAB_SOURCES})
target_compile_definitions(Tests-CoolLab PRIVATE LAB_ENABLE_TESTS)
target_link_libraries(Tests-CoolLab PRIVATE CooLab-Properties)
target_link_libraries(Tests-CoolLab PRIVATE doctest::doctest)