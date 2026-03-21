project(matt-daemon-rpc VERSION 0.0.1)

add_library(${PROJECT_NAME})
add_library(matt-daemon::rpc ALIAS ${PROJECT_NAME})

# note: cmake-format currently does not support FILE_SET in target_source. will
# be fixed after https://github.com/cheshirekow/cmake_format/pull/354 is merged
#
# cmake-format: off
target_sources(${PROJECT_NAME}
PUBLIC
  FILE_SET HEADERS
  BASE_DIRS include
  FILES
    include/matt-daemon/rpc/server-port.hpp
PRIVATE
  src/server-port.cpp
)
# cmake-format: on

set_target_properties(${PROJECT_NAME} PROPERTIES VERIFY_INTERFACE_HEADER_SETS
  ON)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_26)
target_compile_options(
  ${PROJECT_NAME}
  PUBLIC
  -freflection-latest # https://github.com/bloomberg/clang-p2996/blob/p2996/P2996.md
)

target_link_libraries(${PROJECT_NAME} PRIVATE matt-daemon::options)

matt_daemon_apply_static_analyses(${PROJECT_NAME})

matt_daemon_setup_config(${PROJECT_NAME})


option(MATT_DAEMON_RPC_BUILD_EXAMPLES "Build examples" OFF)
option(MATT_DAEMON_RPC_BUILD_DEMO "Build demo" OFF)

if(MATT_DAEMON_BUILD_TESTS)
  add_subdirectory(test)
endif()

if(MATT_DAEMON_RPC_BUILD_EXAMPLES)
  add_subdirectory(example)
endif()

if(MATT_DAEMON_RPC_BUILD_DEMO)
  add_subdirectory(demo)
endif()

install(
  TARGETS ${PROJECT_NAME}
  FILE_SET HEADERS
  COMPONENT ${PROJECT_NAME})
