cmake_minimum_required(VERSION 3.27)

project(
  matt-daemon
  LANGUAGES CXX
  VERSION 0.0.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/CPM.cmake)
include(cmake/CompilerWarnings.cmake)
include(cmake/StaticAnalyzers.cmake)
include(cmake/Sanitizers.cmake)

option(MATT_DAEMON_BUILD_TESTS "Build matt-daemon tests" ${BUILD_TESTING})

option(MATT_DAEMON_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)

option(MATT_DAEMON_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
option(MATT_DAEMON_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
option(MATT_DAEMON_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)

add_library(matt-daemon_options INTERFACE)
add_library(matt-daemon::options ALIAS matt-daemon_options)

matt_daemon_project_warnings(${PROJECT_NAME}_options)
matt_daemon_enable_sanitizers(
  ${PROJECT_NAME}_options ${MATT_DAEMON_ENABLE_SANITIZER_ADDRESS}
  ${MATT_DAEMON_ENABLE_SANITIZER_UNDEFINED}
  ${MATT_DAEMON_ENABLE_SANITIZER_THREAD})
matt_daemon_enable_static_analyses(${MATT_DAEMON_ENABLE_CLANG_TIDY})

if(MATT_DAEMON_BUILD_TESTS)
  enable_testing()
endif()

add_subdirectory(matt-daemon-rpc)
add_subdirectory(matt-daemon-interface)
add_subdirectory(matt-daemon)
add_subdirectory(matt-daemon-cli)

set(CPACK_GENERATOR "TGZ" "DEB")
set(CPACK_DEB_COMPONENT_INSTALL ON)
set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
set(CPACK_PACKAGE_CONTACT "Jakob Sadjina <jakob.sadjina@icloud.com>")
set(CPACK_DEBIAN_MATT-DAEMON-RPC_PACKAGE_VERSION ${matt-daemon-rpc_VERSION})
set(CPACK_DEBIAN_MATT-DAEMON-INTERFACE_PACKAGE_VERSION
    ${matt-daemon-interface_VERSION})
set(CPACK_DEBIAN_MATT-DAEMON_PACKAGE_VERSION ${matt-daemon_VERSION})
set(CPACK_DEBIAN_MATT-DAEMON-CLI_PACKAGE_VERSION ${matt-daemon-cli_VERSION})
include(CPack)
