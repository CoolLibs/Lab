cmake_minimum_required(VERSION 3.16)

# Set project name
project(CoolLab C CXX)

# Set C++ version
set(CXX_STANDARD 20)

# Choose our GPU API and its version
# set(COOL_USE_VULKAN 110)
set(COOL_USE_OPENGL $<$<NOT:$<PLATFORM_ID:Darwin>>:430> $<$<PLATFORM_ID:Darwin>:330>) # Must be >= 330 for ImGui to work properly # We use 330 when building on Mac because they don't support higher versions of OpenGL

# Set the folder where the executable is created
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

# Grab all the source files
file(GLOB_RECURSE MY_SOURCES
    src/*
    Cool/src/*
    lib/imgui-node-editor/*
)

# Create target executable
add_executable(${PROJECT_NAME}
    ${MY_SOURCES}
)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD ${CXX_STANDARD})

# Include Cool
include("Cool/cmake/IncludeMe.cmake")

# Add a post build operation to copy shaders to the output folder (where the executable is created)
include("Cool/cmake/copy_folder_to_the_output_folder.cmake")
copy_folder_to_the_output_folder("shader-examples")

# Set include directories
include_directories(
    src
    lib
)