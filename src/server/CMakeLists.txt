set(lib_server_target server_static)

# Generate header files from dia UML files
generate_dia_header("${CMAKE_SOURCE_DIR}/include/class-diagram.dia" server)

# Find all source files to include in library
file(GLOB_RECURSE server_sources *.cpp)
list(REMOVE_ITEM server_sources "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

# Build the "server" library, add required include and link against dependencies
add_library(${lib_server_target} STATIC ${server_sources} ${jsoncpp_sources})
add_dependencies(${lib_server_target} generate-headers)
target_include_directories(${lib_server_target} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${jsoncpp_include_dir}
  )
target_link_libraries(${lib_server_target} PUBLIC
  ${MHD_LIBRARY}
  $<TARGET_NAME:shared_static>
  )
target_link_libraries(${lib_server_target} PRIVATE boost_serialization)

# Compile "server" executable
set(server_target server)
add_executable(${server_target} main.cpp)
target_link_libraries(${server_target} ${lib_server_target})
if(BUILD_CODE_COVERAGE)
  add_dependencies(code-coverage ${server_target})
endif()