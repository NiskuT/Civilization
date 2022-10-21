cmake_minimum_required (VERSION 3.0)
project (dia2code)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/3rdParty/dia2code-arch)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Enable C11
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

IF(CMAKE_COMPILER_IS_GNUCC)
	SET(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}  -std=c99   -pthread -g")
	# Hide warnings
	SET(dia2code_warnings
		-Wno-unused-variable
		-Wno-unused-function
		-Wno-pointer-sign
		-Wno-unused-but-set-variable
		-Wno-stringop-truncation
		-Wno-maybe-uninitialized
		)
	STRING(REPLACE ";" " " dia2code_warnings "${dia2code_warnings}")
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${dia2code_warnings}")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

include_directories(${CMAKE_SOURCE_DIR}/3rdParty/libxml2-src/include)
include_directories(${CMAKE_BINARY_DIR}/lib)
IF (WIN32)
	link_libraries(${CMAKE_BINARY_DIR}/lib/libxml2.dll)
ELSE ()
	link_libraries(${CMAKE_BINARY_DIR}/lib/libxml2.so)
ENDIF()

# Sources
file(GLOB_RECURSE main_sources src/*.c)

# Executables
add_executable(dia2code ${main_sources})

add_dependencies(dia2code libxml2)

