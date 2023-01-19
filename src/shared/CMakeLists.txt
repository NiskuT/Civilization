
set(lib_shared_target shared_static)

# Generate header files from dia UML files
generate_dia_header("${CMAKE_SOURCE_DIR}/include/class-diagram.dia" shared)

# Find all source files to compile into a library
file(GLOB_RECURSE shared_sources *.cpp)

# Build the "shared" library and add required includes

add_library(${lib_shared_target} STATIC ${shared_sources})


target_link_libraries(${lib_shared_target} PRIVATE boost_serialization)
add_dependencies(${lib_shared_target} generate-headers)

target_include_directories(${lib_shared_target} PUBLIC
  ${CMAKE_SOURCE_DIR}/include
  )

