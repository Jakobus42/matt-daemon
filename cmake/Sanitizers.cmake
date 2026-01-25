function(matt_daemon_enable_sanitizers TARGET ENABLE_SANITIZER_ADDRESS
         ENABLE_SANITIZER_UNDEFINED_BEHAVIOR ENABLE_SANITIZER_THREAD)
  set(SANITIZERS "")

  if(ENABLE_SANITIZER_ADDRESS)
    list(APPEND SANITIZERS "address")
  endif()

  if(ENABLE_SANITIZER_UNDEFINED_BEHAVIOR)
    list(APPEND SANITIZERS "undefined")
  endif()

  if(ENABLE_SANITIZER_THREAD)
    if("address" IN_LIST SANITIZERS)
      message(WARNING "Thread sanitizer does not work with Address sanitizer")
    else()
      list(APPEND SANITIZERS "thread")
    endif()
  endif()

  list(JOIN SANITIZERS "," LIST_OF_SANITIZERS)

  if(LIST_OF_SANITIZERS)
    target_compile_options(${TARGET} INTERFACE -fsanitize=${LIST_OF_SANITIZERS})
    target_link_options(${TARGET} INTERFACE -fsanitize=${LIST_OF_SANITIZERS})
  endif()

endfunction()
