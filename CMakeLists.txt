cmake_minimum_required(VERSION 3.20)

project(CoolLab)
add_executable(${PROJECT_NAME})
add_library(CooLab-Properties INTERFACE) # This is needed by both CoolLab and its tests
target_link_libraries(${PROJECT_NAME} PRIVATE CooLab-Properties)

target_compile_features(CooLab-Properties INTERFACE cxx_std_20)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}) # Set the folder where the executable is created

if(MSVC)
    target_compile_options(CooLab-Properties INTERFACE /WX /W3)
else()
    target_compile_options(CooLab-Properties INTERFACE -Werror -Wall -Wextra -Wpedantic -pedantic-errors)
endif()

# Choose our GPU API and its version
# set(COOL_USE_VULKAN 110)
set(COOL_USE_OPENGL $<IF:$<PLATFORM_ID:Darwin>,410,430>) # Must be >= 330 for ImGui to work properly # We use 410 when building on Mac because it doesn't support higher versions of OpenGL

# Include Cool
add_subdirectory(Cool)
target_link_libraries(CooLab-Properties INTERFACE Cool::Core)
cool_setup(${PROJECT_NAME})

# Include cmd
add_subdirectory(lib/cmd)
target_link_libraries(CooLab-Properties INTERFACE cmd::cmd)

# Include range-v3
target_include_directories(CooLab-Properties INTERFACE lib/range-v3/include)

# Include ImNodes
add_library(ImNodes lib/imnodes/imnodes.cpp)
target_include_directories(ImNodes PRIVATE Cool/lib/imgui)
target_link_libraries(CooLab-Properties INTERFACE ImNodes)
target_compile_features(ImNodes PUBLIC cxx_std_11)

# Grab all the source files
file(GLOB_RECURSE COOL_LAB_SOURCES CONFIGURE_DEPENDS src/*)
target_sources(${PROJECT_NAME} PRIVATE ${COOL_LAB_SOURCES})

# Set include directories
target_include_directories(CooLab-Properties INTERFACE src lib)

# Copy resources to the output folder (where the executable is created) after each build
include("Cool/CMakeUtils/files_and_folders.cmake")
Cool__target_copy_folder(${PROJECT_NAME} "res")

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