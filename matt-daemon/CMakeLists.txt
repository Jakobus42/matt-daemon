project(matt-daemon VERSION 0.0.1)

add_library(${PROJECT_NAME}-lib OBJECT)

# note: cmake-format currently does not support FILE_SET in target_source. will
# be fixed after https://github.com/cheshirekow/cmake_format/pull/354 is merged
#
# cmake-format: off
target_sources(${PROJECT_NAME}-lib
  PRIVATE
    src/FileLock.cpp
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS src
    FILES
      src/FileLock.hpp
)
# cmake-format: on

set_target_properties(${PROJECT_NAME}-lib
                      PROPERTIES VERIFY_INTERFACE_HEADER_SETS ON)

target_link_libraries(
  ${PROJECT_NAME}-lib PUBLIC matt-daemon::rpc matt-daemon::interface
                             matt-daemon::options)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}-lib)
matt_daemon_apply_static_analyses(${PROJECT_NAME})

if(MATT_DAEMON_BUILD_TESTS)
  add_subdirectory(test)
endif()

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
                                        COMPONENT ${PROJECT_NAME})
