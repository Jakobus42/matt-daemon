function(matt_daemon_enable_static_analyses ENABLE_CLANG_TIDY)
  if(${ENABLE_CLANG_TIDY})
    find_program(CLANGTIDY clang-tidy)
    if(CLANGTIDY)
      set(MATT_DAEMON_CLANG_TIDY_OPTIONS
          ${CLANGTIDY} -extra-arg=-Wno-unknown-warning-option
          CACHE INTERNAL "clang-tidy exec path")
      message(STATUS "clang-tidy enabled: ${CLANGTIDY}")
    else()
      message(WARNING "clang-tidy requested but executable not found")
    endif()
  endif()
endfunction()

# TODO: I dont want to apply to each target individually this sucks the issue
# was that clang-tidy tried to analyse external deps aswell maybe fetch
# dependencies first and then set global clang tidy op?
function(matt_daemon_apply_static_analyses TARGET)
  if(MATT_DAEMON_CLANG_TIDY)
    set_target_properties(
      ${TARGET} PROPERTIES CXX_CLANG_TIDY "${MATT_DAEMON_CLANG_TIDY_OPTIONS}")
  endif()
endfunction()
