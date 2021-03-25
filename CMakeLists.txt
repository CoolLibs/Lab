cmake_minimum_required(VERSION 3.8)

# Set project name
project(is0 C CXX)

# Set C++ version
set(CXX_STANDARD 17)

# Declare the Cool modules that we use
set(COOL_MODULES
    Log
    App
    OpenGL
    Renderer_Fullscreen
    Camera
    File
    FileWatcher
    ExportImage
    ImGui
    Time
    LoadImage
    Icons
    Exporter
    String
    Serialization
    Random
    History
    Params
    Alert
)

# Set the path where all the Cool modules are stored. It is required by all the Cool IncludeMe.cmake files
set(PATH_TO_COOL ${CMAKE_SOURCE_DIR}/Cool)

# Set the folder where the executable is created
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

# Grab all the source files
foreach(cool_module IN ITEMS ${COOL_MODULES})
    list(APPEND COOL_SOURCES "${PATH_TO_COOL}/${cool_module}/src/*")
endforeach()
file(GLOB_RECURSE MY_SOURCES
    src/*
    ${COOL_SOURCES}
)
file(GLOB_RECURSE MY_SHADERS myShaders/*)

# Create target executable
add_executable(${PROJECT_NAME}
    ${MY_SOURCES}
    ${MY_SHADERS} # we include them here only so that they will appear in the solution explorer of Visual Studio
)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD ${CXX_STANDARD})

# Include Cool CMake things
foreach(cool_module IN ITEMS ${COOL_MODULES})
    include("${PATH_TO_COOL}/${cool_module}/IncludeMe.cmake")
endforeach()

# Recreate the folder architecture inside the Visual Studio solution (might work for other IDEs as well)
include("${PATH_TO_COOL}/CMake-Helpers/VS_helpers.cmake")
VS_RegisterFiles("${MY_SOURCES}")
VS_RegisterFiles("${MY_SHADERS}")

# Add a post build operation to copy shaders to the output folder (where the executable is created)
include("${PATH_TO_COOL}/CMake-Helpers/copy_folder_to_the_output_folder.cmake")
copy_folder_to_the_output_folder("myShaders")

# Set include directories
include_directories(
    src
)

# /////////////////////////////////////////////////////////////////////////////
# ////////////////////////// PRECOMPILED HEADER ! /////////////////////////////
# /////////////////////////////////////////////////////////////////////////////

# More infos on precompiled headers : https://www.youtube.com/watch?v=eSI4wctZUto&ab_channel=TheCherno
target_precompile_headers(${PROJECT_NAME} PRIVATE
    <string>
    <memory>
)