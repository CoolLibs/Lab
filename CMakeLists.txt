cmake_minimum_required(VERSION 3.20)

project(CoolLab)
add_executable(${PROJECT_NAME})
target_compile_features(${PROJECT_NAME} PRIVATE
    cxx_std_20)
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}) # Set the folder where the executable is created
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /WX /W3)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Werror -Wall -Wextra -Wpedantic -pedantic-errors)
endif()

# Choose our GPU API and its version
# set(COOL_USE_VULKAN 110)
set(COOL_USE_OPENGL $<$<NOT:$<PLATFORM_ID:Darwin>>:430> $<$<PLATFORM_ID:Darwin>:330>) # Must be >= 330 for ImGui to work properly # We use 330 when building on Mac because they don't support higher versions of OpenGL

# Include Cool
add_subdirectory(Cool)
target_link_libraries(${PROJECT_NAME} PRIVATE Cool::Core)

# Include ImNodes
add_library(ImNodes lib/imnodes/imnodes.cpp)
target_include_directories(ImNodes PRIVATE Cool/lib/imgui)
target_link_libraries(${PROJECT_NAME} PRIVATE ImNodes)

# Grab all the source files
file(GLOB_RECURSE COOL_LAB_SOURCES CONFIGURE_DEPENDS src/*)
target_sources(${PROJECT_NAME} PRIVATE ${COOL_LAB_SOURCES})

# Set include directories
target_include_directories(${PROJECT_NAME} PRIVATE src lib)

# Copy resources to the output folder (where the executable is created) after each build
include("Cool/CMakeUtils/files_and_folders.cmake")
Cool__target_copy_folder(${PROJECT_NAME} "shader-examples")
Cool__target_copy_folder(${PROJECT_NAME} "is0 nodes")
Cool__target_copy_folder(${PROJECT_NAME} "is0 shaders")

option(TestIs0Shaders "Test all Is0 shaders" OFF)
if (TestIs0Shaders)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        IS0_TEST_NODES
    )
endif()