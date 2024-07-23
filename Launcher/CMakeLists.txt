cmake_minimum_required(VERSION 3.10)
project(Coollab-Launcher)
add_executable(${PROJECT_NAME})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
set(OPENSSL_USE_STATIC_LIBS ON CACHE BOOL "" FORCE)

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

include(FetchContent)

# Make sure we link the static version
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static library" FORCE)
set(BULD_STATIC_LIBS ON CACHE BOOL "" FORCE)
set(CURL_STATICLIB ON CACHE BOOL "" FORCE)
set(CURL_ZLIB OFF CACHE BOOL "" FORCE)

# JSON
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)

# CURL
FetchContent_Declare(
    curl
    URL https://curl.se/download/curl-7.80.0.tar.gz
)

# MINIZ
# Configurer FetchContent pour miniz
FetchContent_Declare(
    miniz
    GIT_REPOSITORY https://github.com/richgel999/miniz.git
    GIT_TAG master
)

# Get and build JSON
FetchContent_MakeAvailable(json)

# Get and build CURL
FetchContent_MakeAvailable(curl)

FetchContent_MakeAvailable(miniz)

# # Grab all the source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*)
target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json CURL::libcurl miniz)

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ../Cool/lib/stringify/lib/fmt)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${CMAKE_SOURCE_DIR}/Cool/lib/expected/include)