# yeah this is cursed 
function(matt_daemon_setup_config TARGET)
  string(REPLACE "-" "_" ROOT_NAMESPACE "${CMAKE_PROJECT_NAME}")
  if(PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME)
    set(PROJECT_NAMESPACE "${ROOT_NAMESPACE}")
    set(INCLUDE_PREFIX "${CMAKE_PROJECT_NAME}")
  else()
    string(REPLACE "${CMAKE_PROJECT_NAME}-" "" SUBPROJECT_SUFFIX "${PROJECT_NAME}")
    string(REPLACE "-" "_" SUBPROJECT_SUFFIX "${SUBPROJECT_SUFFIX}")
    set(PROJECT_NAMESPACE "${ROOT_NAMESPACE}::${SUBPROJECT_SUFFIX}")
    set(INCLUDE_PREFIX "${CMAKE_PROJECT_NAME}/${SUBPROJECT_SUFFIX}")
  endif()

  string(TOUPPER "${INCLUDE_PREFIX}" PROJECT_NAME_UPPER)
  string(REPLACE "/" "_" PROJECT_NAME_UPPER "${PROJECT_NAME_UPPER}")
  string(REPLACE "-" "_" PROJECT_NAME_UPPER "${PROJECT_NAME_UPPER}")

  configure_file(
    "${CMAKE_SOURCE_DIR}/cmake/templates/config.hpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/configured_files/${INCLUDE_PREFIX}/config.hpp.intermediate"
    ESCAPE_QUOTES)

  file(
    GENERATE
    OUTPUT
    "${CMAKE_CURRENT_BINARY_DIR}/configured_files/include/${INCLUDE_PREFIX}/config.hpp"
    INPUT
    "${CMAKE_CURRENT_BINARY_DIR}/configured_files/${INCLUDE_PREFIX}/config.hpp.intermediate"
  )

  get_target_property(TARGET_TYPE ${TARGET} TYPE)
  if(TARGET_TYPE STREQUAL "INTERFACE_LIBRARY")
    set(SCOPE INTERFACE)
  elseif(TARGET_TYPE STREQUAL "EXECUTABLE")
    set(SCOPE PRIVATE)
  else()
    set(SCOPE PUBLIC)
  endif()

  target_include_directories(
    ${TARGET}
    ${SCOPE}
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/configured_files/include>)
endfunction()
