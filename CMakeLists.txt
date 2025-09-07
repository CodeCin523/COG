cmake_minimum_required(VERSION 3.30)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_GENERATOR_PLATFORM x64)
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


# set(_Vulkan_hint_executable_search_paths    # Supposed to help finding x64
#   "$ENV{VULKAN_SDK}/Bin"
# )
# set(_Vulkan_hint_library_search_paths       # Supposed to help finding x64 
#   "$ENV{VULKAN_SDK}/Lib"
#   "$ENV{VULKAN_SDK}/Bin"
# )
find_package(Vulkan QUIET)
if(Vulkan_FOUND)
    message(STATUS "Vulkan found: ${Vulkan_INCLUDE_DIRS}.")
    set(Vulkan_DIRS ${Vulkan_INCLUDE_DIRS})
    set(Vulkan_LIBS ${Vulkan_LIBRARIES})
else()
    message(WARNING, "Vulkan not found. Using manual includes.")
    set(Vulkan_DIRS "C:/VulkanSDK/1.3.268.0/Include")
    set(Vulkan_LIBS "C:/VulkanSDK/1.3.268.0/Lib/vulkan-1.lib")
endif()

find_package(SDL2 QUIET)
if(SDL2_FOUND)
    message(STATUS "SDL2 found: ${SDL2_INCLUDE_DIRS}.")
    set(SDL2_DIRS ${SDL2_INCLUDE_DIRS})
    set(SDL2_LIBS SDL2::SDL2)
else()
    message(WARNING "SDL2 not found. Using manual includes.")
    set(SDL2_DIRS "C:/VulkanSDK/1.3.268.0/Include")
    set(SDL2_LIBS "C:/VulkanSDK/1.3.268.0/Lib/SDL2.lib")
endif()


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
