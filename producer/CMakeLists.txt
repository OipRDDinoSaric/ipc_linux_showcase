add_executable(Producer)

target_sources(Producer PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp)

target_include_directories(Producer PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)

target_link_libraries(Producer PRIVATE
        fmt::fmt
)