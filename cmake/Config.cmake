# yeah idk if this is too good but it works xd
function(matt_daemon_setup_config TARGET)
  string(TOUPPER "${PROJECT_NAME}" PROJECT_NAME_UPPER)
  string(REPLACE "-" "_" PROJECT_NAME_UPPER "${PROJECT_NAME_UPPER}")

  string(REPLACE "-" "_" PROJECT_NAMESPACE "${PROJECT_NAME}")

  configure_file(
    "${CMAKE_SOURCE_DIR}/cmake/templates/config.hpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/configured_files/${PROJECT_NAME}/config.hpp.intermediate"
    ESCAPE_QUOTES)

  file(
    GENERATE
    OUTPUT
    "${CMAKE_CURRENT_BINARY_DIR}/configured_files/include/${PROJECT_NAME}/config.hpp"
    INPUT
    "${CMAKE_CURRENT_BINARY_DIR}/configured_files/${PROJECT_NAME}/config.hpp.intermediate"
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
