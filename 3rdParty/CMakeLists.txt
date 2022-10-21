include(ExternalProject)


ExternalProject_Add(libxml2
    GIT_REPOSITORY https://gitlab.gnome.org/GNOME/libxml2.git
    GIT_TAG "v2.10.2"
    PREFIX "${PROJECT_SOURCE_DIR}/3rdParty/libxml2"
    SOURCE_DIR "${PROJECT_SOURCE_DIR}/3rdParty/libxml2-src"
    BINARY_DIR "${CMAKE_BINARY_DIR}/lib"
    CMAKE_ARGS 
        -D CMAKE_BUILD_TYPE=Release
        -D LIBXML2_WITH_ICONV=OFF
        -D LIBXML2_WITH_LZMA=OFF
        -D LIBXML2_WITH_PYTHON=OFF
        -D LIBXML2_WITH_ZLIB=OFF
    INSTALL_COMMAND ""
)

add_subdirectory(dia2code)
