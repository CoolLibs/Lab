cmake_minimum_required(VERSION 3.16)

# Set project name
project(CoolLab C CXX)

# Set C++ version
set(CXX_STANDARD 17)

# Choose our GPU API
#set(COOL_USE_VULKAN true)
set(COOL_USE_OPENGL true)

# Set the folder where the executable is created
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

# Grab all the source files
file(GLOB_RECURSE MY_SOURCES
    src/*
    Cool/src/*
    lib/imgui-node-editor/*
)
file(GLOB_RECURSE SHADER_EXAMPLES shader-examples/*)

# Create target executable
add_executable(${PROJECT_NAME}
    ${MY_SOURCES}
)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD ${CXX_STANDARD})

# #define CMAKE_SOURCE_DIR
# This is useful to set the current working directory in debug mode. YOUR RELEASE CODE SHOULDN'T RELY ON CMAKE_SOURCE_DIR
set_property(TARGET ${PROJECT_NAME} PROPERTY COMPILE_DEFINITIONS "CMAKE_SOURCE_DIR=\"${CMAKE_SOURCE_DIR}\"")

# Include Cool
include("Cool/cmake/IncludeMe.cmake")

# Recreate the folder architecture inside the Visual Studio solution (might work for other IDEs as well)
include("Cool/cmake/VS_helpers.cmake")
VS_RegisterFiles("${MY_SOURCES}")
VS_RegisterFiles("${SHADER_EXAMPLES}")

# Add a post build operation to copy shaders to the output folder (where the executable is created)
include("Cool/cmake/copy_folder_to_the_output_folder.cmake")
copy_folder_to_the_output_folder("shader-examples")

# Set include directories
include_directories(
    src
    lib
    lib/entt/single_include
)