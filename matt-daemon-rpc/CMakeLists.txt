project(matt-daemon-rpc VERSION 0.0.1)

add_library(${PROJECT_NAME} INTERFACE)
add_library(matt-daemon::rpc ALIAS ${PROJECT_NAME})

# note: cmake-format currently does not support FILE_SET in target_source. will
# be fixed after https://github.com/cheshirekow/cmake_format/pull/354 is merged
#
# cmake-format: off
target_sources(${PROJECT_NAME}
  INTERFACE
    FILE_SET HEADERS
    BASE_DIRS include
    FILES
      include/matt-daemon-rpc/annotations.hpp
      include/matt-daemon-rpc/client-port.hpp
      include/matt-daemon-rpc/concepts.hpp
      include/matt-daemon-rpc/error.hpp
      include/matt-daemon-rpc/result.hpp
      include/matt-daemon-rpc/strong-type.hpp
      
      include/matt-daemon-rpc/detail/client-port-impl.hpp
      include/matt-daemon-rpc/detail/concepts.hpp
)
# cmake-format: on

set_target_properties(${PROJECT_NAME} PROPERTIES VERIFY_INTERFACE_HEADER_SETS
                                                 ON)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_26)
target_compile_options(
  ${PROJECT_NAME}
  INTERFACE
    -freflection-latest # https://github.com/bloomberg/clang-p2996/blob/p2996/P2996.md
)

target_include_directories(
  ${PROJECT_NAME}
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>)

include(GNUInstallDirs)

if(MATT_DAEMON_BUILD_TESTS)
  add_subdirectory(test)
endif()

install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}-targets
  FILE_SET HEADERS
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  COMPONENT ${PROJECT_NAME})
