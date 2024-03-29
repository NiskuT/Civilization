cmake_minimum_required (VERSION 3.0)
project (civilization)

# CMP0074 policy enable find_* to use <PackageName>_ROOT variables to find packages.
if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()

message(STATUS "To build without debug symbols, use -DCMAKE_BUILD_TYPE=Release")
message(STATUS "To build with code coverage, use -DBUILD_CODE_COVERAGE=ON")
message(STATUS "To build test, use -DBUILD_TESTING=ON")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(GenerateDiaHeader)

# Export executables to "bin" directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Enable C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Debug and Release builds
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING
    "Build type, Debug or Release" FORCE)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
IF(CMAKE_COMPILER_IS_GNUCC)
  SET(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -Wall -std=c17   -pthread -g")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++17 -pthread -g")
ENDIF(CMAKE_COMPILER_IS_GNUCC)



# Add code coverage support for Gcov using GCC
set(BUILD_CODE_COVERAGE OFF CACHE BOOL
  "Set BUILD_CODE_COVERAGE to ON to enable code coverage")
if(BUILD_CODE_COVERAGE AND NOT UNIX)
  message(WARNING "Code coverage is only available for UNIX systems")
  set(BUILD_CODE_COVERAGE OFF)
elseif(BUILD_CODE_COVERAGE AND NOT CMAKE_COMPILER_IS_GNUCC AND NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  message(WARNING "Code coverage is only available for GNU compiler and Clang compiler")
  set(BUILD_CODE_COVERAGE OFF)
endif()

message(STATUS "Code coverage: ${BUILD_CODE_COVERAGE}")
if(BUILD_CODE_COVERAGE)
  include(CodeCoverage)
  APPEND_COVERAGE_COMPILER_FLAGS()
  # Exclude all code not directly generated by user from code coverage
  set(COVERAGE_LCOV_EXCLUDES
    '/usr/*'
    '${PROJECT_SOURCE_DIR}/test/*'
    '${PROJECT_SOURCE_DIR}/3rdParty/*')
  set(COVERAGE_GCOVR_EXCLUDES
    /usr/
    "${PROJECT_SOURCE_DIR}/test/"
    "${PROJECT_SOURCE_DIR}/3rdParty/")
  # Create script to run tests for code coverage
  set(test_runner "${CMAKE_CURRENT_BINARY_DIR}/run_test.sh")
  file(WRITE ${test_runner}
    "cd \"${CMAKE_CURRENT_BINARY_DIR}/test\"\n"
    "ctest --timeout 300 ||/bin/true"
    )
  # Create target for code coverage
  SETUP_TARGET_FOR_COVERAGE_LCOV(
    NAME code-coverage
    EXECUTABLE sh ${test_runner}
    DEPENDS ${test_runner}
    )
  SETUP_TARGET_FOR_COVERAGE_GCOVR_XML(
    NAME code-coverage-gcov
    EXECUTABLE sh ${test_runner}
    DEPENDS ${test_runner}
    )
endif()

# Find library SFML
if(WIN32)
  set(SFML_ROOT "D:/Utils/SFML-2.3" CACHE STRING
    "Root directory for SFML library")
  set(CMAKE_MODULE_PATH "${SFML_ROOT}/cmake/Modules" ${CMAKE_MODULE_PATH})
else(WIN32)
  set(CMAKE_MODULE_PATH "/usr/share/SFML/cmake/Modules/" ${CMAKE_MODULE_PATH})
endif(WIN32)
find_package(SFML 2 COMPONENTS graphics network window system)
set(SFML_LIBRARIES sfml-graphics sfml-network sfml-window sfml-system)


add_subdirectory(3rdParty)

add_compile_definitions(RESOURCES_PATH="${PROJECT_SOURCE_DIR}/resources")

# Add source subdirectories
add_subdirectory(src/shared)
add_subdirectory(src/client)
add_subdirectory(src/server)

# Unit tests
# Set BUILD_TESTING to ON in CMakeCache to enable tests
set(BUILD_TESTING OFF CACHE BOOL "Set BUILD_TESTING to ON to enable tests")
message(STATUS "Test: ${BUILD_TESTING}")
if(BUILD_TESTING)
  # Include tests scripts
  add_subdirectory(test)
endif()

