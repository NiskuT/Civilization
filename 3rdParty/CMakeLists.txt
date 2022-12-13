include(ExternalProject)


ExternalProject_Add(libxml2
    GIT_REPOSITORY https://gitlab.gnome.org/GNOME/libxml2.git
    GIT_TAG "v2.10.2"
    PREFIX "${PROJECT_SOURCE_DIR}/3rdParty/libxml2"
    SOURCE_DIR "${PROJECT_SOURCE_DIR}/3rdParty/libxml2-src"
    BINARY_DIR "${CMAKE_BINARY_DIR}/lib/xml"
    CMAKE_ARGS 
        -D CMAKE_BUILD_TYPE=Release
        -D LIBXML2_WITH_ICONV=OFF
        -D LIBXML2_WITH_LZMA=OFF
        -D LIBXML2_WITH_PYTHON=OFF
        -D LIBXML2_WITH_ZLIB=OFF
    INSTALL_COMMAND ""
)

ExternalProject_Add(jsoncpp
GIT_REPOSITORY https://github.com/open-source-parsers/jsoncpp.git
    GIT_TAG "1.9.5"
    PREFIX "${PROJECT_SOURCE_DIR}/3rdParty/jsoncpp"
    SOURCE_DIR "${PROJECT_SOURCE_DIR}/3rdParty/jsoncpp-src"
    BINARY_DIR "${CMAKE_BINARY_DIR}/lib/json"
    CMAKE_ARGS
        -DCMAKE_BUILD_TYPE=release 
        -DBUILD_STATIC_LIBS=ON 
        -DBUILD_SHARED_LIBS=OFF 
        -G "Unix Makefiles" ${PROJECT_SOURCE_DIR}/3rdParty/jsoncpp-src
    INSTALL_COMMAND ""
)

add_subdirectory(dia2code)