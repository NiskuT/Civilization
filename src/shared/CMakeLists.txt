
set(lib_shared_target shared_static)

# Generate header files from dia UML files
generate_dia_header("${CMAKE_CURRENT_SOURCE_DIR}/../state.dia")
#generate_dia_header("${CMAKE_CURRENT_SOURCE_DIR}/../engine.dia")
#generate_dia_header("${CMAKE_CURRENT_SOURCE_DIR}/../ai.dia")

# Find all source files to compile into a library
file(GLOB_RECURSE shared_sources *.cpp)

# Build the "shared" library and add required includes
add_library(${lib_shared_target} STATIC ${shared_sources})
add_dependencies(${lib_shared_target} generate-headers)
target_include_directories(${lib_shared_target} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

# vim: set sw=2 sts=2 et:

