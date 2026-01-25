cpmaddpackage(NAME googletest GITHUB_REPOSITORY google/googletest VERSION
              1.17.0)

add_executable(${PROJECT_NAME}-test)
target_sources(${PROJECT_NAME}-test PRIVATE src/test.cpp)
target_link_libraries(${PROJECT_NAME}-test PRIVATE ${PROJECT_NAME}
                                                   GTest::gtest_main)
matt_daemon_apply_static_analyses(${PROJECT_NAME}-test)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}-test)
