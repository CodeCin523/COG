cmake_minimum_required(VERSION 3.28)

# SET VARIABLES
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_STATIC_LIBRARY_PREFIX "")

set(CORE_NAME "core")
set(CORE_VERSION_MAJOR 1)
set(CORE_VERSION_MINOR 0)
set(CORE_VERSION_PATCH 0)

set(PROJECT_NAME "cog")
set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)

configure_file(
    ${CMAKE_SOURCE_DIR}/src/gbl.cmake.hh
    ${CMAKE_SOURCE_DIR}/include/gbl.hh
)


# LOAD LIBRARIES
find_package(Vulkan QUIET)
if(NOT Vulkan)
    message(WARNING "Vulkan not found. Using \"paths.cmake\".")
else()
    message(STATUS "Vulkan found: ${Vulkan_INCLUDE_DIRS}.")
    set(Vulkan_DIRS ${Vulkan_INCLUDE_DIRS})
    set(Vulkan_LIBS ${Vulkan_LIBRARIES})
endif()

find_package(SDL2 QUIET)
if(NOT SDL2)
    message(WARNING "SDL2 not found. Using \"paths.cmake\".")
else()
    message(STATUS "SDL2 found: ${SDL2_INCLUDE_DIRS}.")
    set(SDL2_DIRS ${SDL2_INCLUDE_DIRS})
    set(SDL2_LIBS SDL2::SDL2)
endif()

if(NOT EXISTS ${CMAKE_SOURCE_DIR}/paths.cmake)
    message(WARNING "\"paths.cmake\" not detected, a new one will be created. Insure to set the correct values.")
    file(
        WRITE "./paths.cmake"
        "if(NOT Vulkan)\nset(Vulkan_DIRS \"C:/VulkanSDK/1.3.268.0/Include\")\nset(Vulkan_LIBS \"C:/VulkanSDK/1.3.268.0/Lib/vulkan-1.lib\")\nendif()\nif(NOT SDL2)\nset(SDL2_DIRS \"C:/VulkanSDK/1.3.268.0/Include\")\nset(SDL2_LIBS \"C:/VulkanSDK/1.3.268.0/Lib/SDL2.lib\")\nendif()"
    )
endif()

include("./paths.cmake")

# CREATE PROJECT
project(
    ${PROJECT_NAME}
    VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}
    LANGUAGES CXX
)

add_library(
    ${CORE_NAME}
    src/core/core.cc
    src/core/coreWindow.cc
    src/core/coreVulkan.cc
)
target_include_directories(
    ${CORE_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/include/
    ${Vulkan_DIRS}
    ${SDL2_DIRS}
)
target_link_libraries(
    ${CORE_NAME} PUBLIC
    ${Vulkan_LIBS}
    ${SDL2_LIBS}
)

add_executable(
    ${PROJECT_NAME}
    src/main.cc
)
target_include_directories(
    ${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include/
    ${Vulkan_DIRS}
    ${SDL2_DIRS}
)
target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    ${CORE_NAME}
)
