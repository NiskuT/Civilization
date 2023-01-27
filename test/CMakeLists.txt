
# Add "test" target to make
enable_testing()

# Custom "check" target that enable Boost unit test output on CTest failure.
# Create an alias target "unittest" for "check".
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure)
add_custom_target(unittest DEPENDS check)

# Find library Boost for unit test framework
find_package(Boost 1.50 REQUIRED COMPONENTS unit_test_framework)
message(STATUS "Boost libraries: ${Boost_LIBRARIES}")
message(STATUS "Boost include directory: ${Boost_INCLUDE_DIRS}")

set(jsoncpp_lib_static ${CMAKE_BINARY_DIR}/lib/json/lib/libjsoncpp.a)

function(add_custom_test name)
  # Parse input parameters
  set(options "")
  set(one_value_args "")
  set(multi_values_args
    SOURCES
    TEST_LIBRARIES
    TEST_INCLUDES
    )
  cmake_parse_arguments(ARG
    "${options}"
    "${one_value_args}"
    "${multi_values_args}"
    ${ARGN})

  set(target_name "test_${name}")
  set(sources "test_${name}.cpp")
  list(APPEND sources ${ARG_SOURCES})

  # Compile and link test program
  add_executable(${target_name} ${sources})
  add_dependencies(${target_name} generate-headers jsoncpp)
  target_compile_definitions(${target_name} PRIVATE
    BOOST_TEST_MAIN
    BOOST_TEST_DYN_LINK
    BOOST_TEST_MODULE=${name}
    )
  target_link_libraries(${target_name} PRIVATE
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${ARG_TEST_LIBRARIES}
    $<TARGET_NAME:shared_static>
    $<TARGET_NAME:client_static>
    $<TARGET_NAME:server_static>
    ${jsoncpp_lib_static}
    )
  target_include_directories(${target_name} PRIVATE
    ${Boost_INCLUDE_DIRS}
    ${ARG_TEST_INCLUDES}
    ${CMAKE_SOURCE_DIR}/3rdParty/jsoncpp-src/include
    )
  set_target_properties(${target_name}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )

  # Add test to CTest
  add_test(NAME ${target_name}
    COMMAND $<TARGET_FILE:${target_name}>
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

  # Add dependency to build test target before code coverage
  if(BUILD_CODE_COVERAGE)
    add_dependencies(code-coverage ${target_name})
    add_dependencies(code-coverage-gcov ${target_name})
  endif()
endfunction()

add_subdirectory(shared)
add_subdirectory(client)
add_subdirectory(server)



