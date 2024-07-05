cmake_minimum_required(VERSION 3.20)

project(Coollab-Launcher)
add_executable(${PROJECT_NAME})

# Choose C++ version
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# Set the folder where the executable is created
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/Launcher/${CMAKE_BUILD_TYPE})

# Set warning level
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -pedantic-errors -Wconversion -Wsign-conversion -Wimplicit-fallthrough)
endif()

# Maybe enable warnings as errors
set(WARNINGS_AS_ERRORS_FOR_COOLLAB_LAUNCHER OFF CACHE BOOL "ON iff you want to treat warnings as errors") # Might be overriden in the CMake cache

if(WARNINGS_AS_ERRORS_FOR_COOLLAB_LAUNCHER)
    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE /WX)
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -Werror)
    endif()
endif()

# Set app icon
Cool__set_app_icon(${PROJECT_NAME} "../res/logo.png" "${CMAKE_SOURCE_DIR}/app-resources/icon.rc")

# Grab all the source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*)
target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})

# Set include directories
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE src)