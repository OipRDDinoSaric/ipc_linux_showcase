add_library(Producer)
add_library(Producer::Producer ALIAS Producer)

target_sources(Producer PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/produce/Producer.cpp
)

target_include_directories(Producer PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(Producer PRIVATE
        fmt::fmt
)