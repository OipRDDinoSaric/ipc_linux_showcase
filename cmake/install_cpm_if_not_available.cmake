# Variables:
#     - CPM_DOWNLOAD_VERSION - version of CPM to download. Format of the version is the name of the release.

if (NOT DEFINED CPM_DOWNLOAD_VERSION)
    message(FATAL_ERROR "CPM_DOWNLOAD_VERSION must be set.")
endif ()

set(CPM_DOWNLOAD_FILE_PATH "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")

if (NOT EXISTS ${CPM_DOWNLOAD_FILE_PATH})
    message(STATUS "Installing CPM.")
    file(DOWNLOAD https://github.com/TheLartians/CPM.cmake/releases/download/${CPM_DOWNLOAD_VERSION}/CPM.cmake
            ${CPM_DOWNLOAD_FILE_PATH})
    message(STATUS "CPM ${CPM_DOWNLOAD_VERSION} installed.")
endif ()

include(${CPM_DOWNLOAD_FILE_PATH})
