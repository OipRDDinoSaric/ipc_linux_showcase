add_library(Consumer)
add_library(Consumer::Consumer ALIAS Consumer)

target_sources(Consumer PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/consume/Consumer.cpp
)

target_include_directories(Consumer PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(Consumer PRIVATE
        fmt::fmt
)