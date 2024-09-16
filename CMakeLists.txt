cmake_minimum_required(VERSION 3.17.3)

project(IpcLinuxShowcase)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wall -Wextra -Wconversion -Wpedantic -O0 -g3)

set(CPM_DOWNLOAD_VERSION "v0.40.0")
include(${CMAKE_CURRENT_LIST_DIR}/cmake/install_cpm_if_not_available.cmake)
unset(CPM_DOWNLOAD_VERSION)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/install_cpm_packages.cmake)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/lib/producer)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/lib/consumer)

add_executable(IpcLinuxShowcase)

target_sources(IpcLinuxShowcase PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
)

target_include_directories(IpcLinuxShowcase PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(IpcLinuxShowcase PRIVATE
        fmt::fmt
        Producer::Producer
        Consumer::Consumer
)