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
      include/matt-daemon-rpc/foo.hpp
)
# cmake-format: on

set_target_properties(${PROJECT_NAME}
                      PROPERTIES VERIFY_INTERFACE_HEADER_SETS ON)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_23)
target_link_libraries(${PROJECT_NAME} INTERFACE matt-daemon_options)
matt_daemon_apply_static_analyses(${PROJECT_NAME})

target_include_directories(
  ${PROJECT_NAME}
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>)

if(MATT_DAEMON_BUILD_TESTS)
  add_subdirectory(test)
endif()

include(GNUInstallDirs)
install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}-targets
  FILE_SET HEADERS
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  COMPONENT ${PROJECT_NAME})
