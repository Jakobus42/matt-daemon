add_library(${PROJECT_NAME}-rpc INTERFACE)
add_library(${PROJECT_NAME}::rpc ALIAS ${PROJECT_NAME}-rpc)

# note: cmake-format currently does not support FILE_SET in target_source. will
# be fixed after https://github.com/cheshirekow/cmake_format/pull/354 is merged
#
# cmake-format: off
target_sources(${PROJECT_NAME}-rpc
  INTERFACE
    FILE_SET HEADERS
    BASE_DIRS include
    FILES
      include/matt-daemon-rpc/foo.hpp
)
# cmake-format: on

set_target_properties(${PROJECT_NAME}-rpc
                      PROPERTIES VERIFY_INTERFACE_HEADER_SETS ON)

target_compile_features(${PROJECT_NAME}-rpc INTERFACE cxx_std_23)
target_link_libraries(${PROJECT_NAME}-rpc INTERFACE ${PROJECT_NAME}_options)
matt_daemon_apply_static_analyses(${PROJECT_NAME}-rpc)

target_include_directories(
  ${PROJECT_NAME}-rpc
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>)

if(MATT_DAEMON_BUILD_TESTS)
  add_subdirectory(test)
endif()

include(GNUInstallDirs)
install(
  TARGETS ${PROJECT_NAME}-rpc
  EXPORT ${PROJECT_NAME}-targets
  FILE_SET HEADERS
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
