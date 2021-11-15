cmake_minimum_required(VERSION 3.12)
cmake_policy(SET CMP0112 NEW) # Required by Cool__copy_folder_to_output()

project(CoolLab)
add_executable(${PROJECT_NAME})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}) # Set the folder where the executable is created
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
include("Cool/cmake/copy_to_output.cmake")
Cool__copy_folder_to_output("shader-examples")
Cool__copy_folder_to_output("is0 nodes")