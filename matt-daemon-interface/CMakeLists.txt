project(matt-daemon-interface VERSION 0.0.1)

add_library(${PROJECT_NAME} INTERFACE)
add_library(matt-daemon::interface ALIAS ${PROJECT_NAME})

# note: cmake-format currently does not support FILE_SET in target_source. will
# be fixed after https://github.com/cheshirekow/cmake_format/pull/354 is merged
#
# cmake-format: off
target_sources(${PROJECT_NAME}
  INTERFACE
    FILE_SET HEADERS
    BASE_DIRS include
    FILES
      include/matt-daemon/interface/example/example.hpp
)
# cmake-format: on

set_target_properties(${PROJECT_NAME} PROPERTIES VERIFY_INTERFACE_HEADER_SETS
  ON)
target_link_libraries(${PROJECT_NAME} INTERFACE matt-daemon::rpc)

matt_daemon_setup_config(${PROJECT_NAME})

install(
  TARGETS ${PROJECT_NAME}
  FILE_SET HEADERS
  COMPONENT ${PROJECT_NAME})
