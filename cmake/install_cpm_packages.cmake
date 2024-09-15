include(${CMAKE_CURRENT_LIST_DIR}/utility.cmake)

CPMAddPackage(
        NAME fmt
        GITHUB_REPOSITORY fmtlib/fmt
        GIT_TAG 11.0.1
)

if (false)
    CPMAddPackage(
            NAME Boost
            VERSION 1.86.0
            GITHUB_REPOSITORY boostorg/boost
            # Included libraries separators must be escaped, e.g., iostreams\\\;asio
            OPTIONS "BOOST_ENABLE_CMAKE ON" "BOOST_INCLUDE_LIBRARIES "
            GIT_TAG "boost-1.86.0"
    )
endif ()

declare_system_library(fmt::fmt)
