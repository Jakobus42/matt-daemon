cpmaddpackage(NAME googletest GITHUB_REPOSITORY google/googletest VERSION
              1.17.0)

add_executable(${PROJECT_NAME}_test)
target_sources(${PROJECT_NAME}_test PRIVATE src/file-lock-test.cpp)
target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME}-lib
                                                   GTest::gtest_main)
matt_daemon_apply_static_analyses(${PROJECT_NAME}_test)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_test)
