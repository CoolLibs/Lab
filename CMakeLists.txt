cmake_minimum_required(VERSION 3.0)

# Set project name
project(CoolLab)
add_executable(${PROJECT_NAME})

# Choose our GPU API and its version
# set(COOL_USE_VULKAN 110)
set(COOL_USE_OPENGL $<$<NOT:$<PLATFORM_ID:Darwin>>:430> $<$<PLATFORM_ID:Darwin>:330>) # Must be >= 330 for ImGui to work properly # We use 330 when building on Mac because they don't support higher versions of OpenGL

# Include Cool
add_subdirectory(Cool)
target_link_libraries(${PROJECT_NAME} Cool::)

# Set C++ version
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

# Set the folder where the executable is created
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

# Grab all the source files
file(GLOB_RECURSE MY_SOURCES
    src/*
)
list(APPEND IMNODE_SOURCES
    lib/imnodes/imnodes_internal.h
    lib/imnodes/imnodes.h
    lib/imnodes/imnodes.cpp
)

# Create target executable
target_sources(${PROJECT_NAME} PRIVATE
    ${MY_SOURCES}
    ${IMNODE_SOURCES}
)

# Add a post build operation to copy shaders to the output folder (where the executable is created)
include("Cool/cmake/copy_folder_to_the_output_folder.cmake")
copy_folder_to_the_output_folder("shader-examples")
copy_folder_to_the_output_folder("is0 nodes")

# Set include directories
include_directories(
    src
    lib
)