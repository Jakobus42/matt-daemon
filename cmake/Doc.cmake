function(matt_daemon_enable_doc)
    find_package(Doxygen REQUIRED)
    find_program(SPHINX_BUILD sphinx-build REQUIRED)

    set(DOCS_DIR ${PROJECT_SOURCE_DIR}/doc)

    add_custom_target(doc
        COMMAND ${CMAKE_COMMAND} -E make_directory build/doxygen/xml
        COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
        COMMAND ${SPHINX_BUILD} -M html .. build -c .
        WORKING_DIRECTORY ${DOCS_DIR}
    )
endfunction()
