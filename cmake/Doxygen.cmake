function(matt_daemon_enable_doxygen)
    CPMAddPackage(
        NAME doxygen-awesome-css
        URL https://github.com/jothepro/doxygen-awesome-css/archive/refs/tags/v1.6.1.zip
        DOWNLOAD_ONLY YES
    )

    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${PROJECT_SOURCE_DIR}/README.md")
    set(DOXYGEN_QUIET YES)
    set(DOXYGEN_CALLER_GRAPH YES)
    set(DOXYGEN_CALL_GRAPH YES)
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_GENERATE_TREEVIEW YES)
    set(DOXYGEN_DOT_IMAGE_FORMAT svg)
    set(DOXYGEN_EXCLUDE_PATTERNS "*/test/*" "*/build/*" "*/detail/*")
    set(DOXYGEN_DOT_TRANSPARENT YES)
    set(DOXYGEN_HTML_EXTRA_STYLESHEET
        "${doxygen-awesome-css_SOURCE_DIR}/doxygen-awesome.css"
        "${doxygen-awesome-css_SOURCE_DIR}/doxygen-awesome-sidebar-only.css"
    )

    find_package(Doxygen REQUIRED COMPONENTS dot)
    doxygen_add_docs(doxygen-docs ALL ${PROJECT_SOURCE_DIR})
endfunction()
