cmake_minimum_required(VERSION 3.14)
project(photon VERSION 1.0 LANGUAGES C)
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(MACOS CMAKE_SYSTEM_NAME STREQUAL "Darwin")
include(FetchContent)
include(CMakeParseArguments)

# Helper functions
function(find_or_fetch DEP_NAME)
    # Usage:
    #   find_or_fetch(<name> [CONFIG] [GIT_REPOSITORY <repo_url>] [GIT_TAG <tag>] [URL <url>])
    #
    # If CONFIG is provided, try to use find_package with CONFIG mode.
    # Otherwise, try finds without CONFIG.
    # If find_package did not find it then it uses FetchContent to download it.

    cmake_parse_arguments(FOF
        ""  # no options
        "CONFIG;GIT_REPOSITORY;GIT_TAG;URL" # oneValueArgs
        ""  # no multiValueArgs
        ${ARGN}
    )

    # Try to find the package first
    if (FOF_CONFIG)
        find_package(${DEP_NAME} CONFIG QUIET)
    else()
        find_package(${DEP_NAME} QUIET)
    endif()

    if (${DEP_NAME}_FOUND)
        message(STATUS "Found ${DEP_NAME} on system, using system installation.")
    else()
        message(STATUS "${DEP_NAME} not found on system, downloading via FetchContent.")

        if (FOF_GIT_REPOSITORY)
            FetchContent_Declare(
                ${DEP_NAME}
                GIT_REPOSITORY ${FOF_GIT_REPOSITORY}
                GIT_TAG ${FOF_GIT_TAG}
            )
        elseif (FOF_URL)
            FetchContent_Declare(
                ${DEP_NAME}
                URL ${FOF_URL}
            )
        else()
            message(FATAL_ERROR "No download information provided for ${DEP_NAME}.")
        endif()
        FetchContent_MakeAvailable(${DEP_NAME})
    endif()
endfunction()

function(link_and_include TARGET_NAME DEP_NAME)
    # Usage:
    #   link_and_include(<target> <dependency> [INCLUDE_DIR <include_dir>] [LINK_TARGET <link_target>])
    #
    # Links a dependency to a target and adds its include directories.
    # If INCLUDE_DIR is not provided, it defaults to ${DEP_NAME}_SOURCE_DIR/include.
    # If LINK_TARGET is not provided, it defaults to DEP_NAME.

    cmake_parse_arguments(LAI
        ""  # no options
        "INCLUDE_DIR;LINK_TARGET"  # oneValueArgs
        ""  # no multiValueArgs
        ${ARGN}
    )
    
    # Set default values if not provided
    if (NOT DEFINED LAI_LINK_TARGET)
        set(LAI_LINK_TARGET "${DEP_NAME}")
    endif()
    
    # Link library
    target_link_libraries(${TARGET_NAME} PRIVATE ${LAI_LINK_TARGET})
    
    # Include headers if INCLUDE_DIR is provided
    if (DEFINED LAI_INCLUDE_DIR)
        target_include_directories(${TARGET_NAME} PRIVATE ${LAI_INCLUDE_DIR})
    endif()
endfunction()

# ======================== CONFIGURATION
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
endif()

# ======================== DEPS
# === SDL 3
find_or_fetch(SDL
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-3.2.0
)

# === SPIRV-Headers
find_or_fetch(SPIRV-Headers
  GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Headers.git
  GIT_TAG vulkan-sdk-1.4.304.1
)

# === SPIRV-Tools
find_or_fetch(SPIRV-Tools
  GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Tools.git
  GIT_TAG vulkan-sdk-1.4.304.1
)

# === glslang
find_or_fetch(glslang
  GIT_REPOSITORY https://github.com/KhronosGroup/glslang.git
  GIT_TAG vulkan-sdk-1.4.304.1
)

# === shaderc
find_or_fetch(shaderc
  GIT_REPOSITORY https://github.com/google/shaderc.git
  GIT_TAG v2025.1
)
set(SHADERC_SKIP_TESTS ON CACHE BOOL "" FORCE)
set(SHADERC_SKIP_EXAMPLES ON CACHE BOOL "" FORCE)

if (MACOS)
    # === Metal bindings
    add_subdirectory(/Users/tomtar/dev/projects/cmt ${CMAKE_BINARY_DIR}/cmt)
else()
    # === Vulkan
    find_package(Vulkan REQUIRED)
    if (Vulkan_FOUND)
        message(STATUS "Found Vulkan: ${Vulkan_LIBRARY}")
    else()
        message(FATAL_ERROR "Vulkan SDK not found. Please ensure VULKAN_SDK is set correctly.")
    endif()
endif()

# ========================= SOURCE
file(GLOB PHOTON_PUBLIC_HEADERS include/**.h)
file(GLOB PHOTON_PRIVATE_SOURCES src/**.c)
add_library(${PROJECT_NAME} STATIC
    ${PHOTON_PRIVATE_SOURCES}
    ${PHOTON_PUBLIC_HEADERS}
)
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Link dependencies and include headers
link_and_include(${PROJECT_NAME} SDL LINK_TARGET SDL3::SDL3 INCLUDE_DIR ${SDL_SOURCE_DIR}/include)
link_and_include(${PROJECT_NAME} SPIRV-Headers INCLUDE_DIR ${SPIRV-Headers_SOURCE_DIR}/include)
link_and_include(${PROJECT_NAME} SPIRV-Tools INCLUDE_DIR ${SPIRV-Tools_SOURCE_DIR}/include)
link_and_include(${PROJECT_NAME} glslang INCLUDE_DIR ${glslang_SOURCE_DIR}/include)
link_and_include(${PROJECT_NAME} shaderc INCLUDE_DIR ${shaderc_SOURCE_DIR}/libshaderc/include)

if (MACOS)
    link_and_include(${PROJECT_NAME} cmt_lib INCLUDE_DIR ${cmt_SOURCE_DIR}/include)
else()
    link_and_include(${PROJECT_NAME} Vulkan::Vulkan INCLUDE_DIR ${Vulkan_INCLUDE_DIRS})
endif()

# ======================== EXAMPLES
file(GLOB EXAMPLE_SOURCES examples/*.c)
foreach(example_file ${EXAMPLE_SOURCES})
    get_filename_component(example_name ${example_file} NAME_WE)
    add_executable(${example_name} ${example_file})
    target_link_libraries(${example_name} PRIVATE ${PROJECT_NAME})
    target_include_directories(${example_name} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
endforeach()
