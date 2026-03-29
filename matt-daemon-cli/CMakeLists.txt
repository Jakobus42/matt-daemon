project(matt-daemon-cli VERSION 0.0.1)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE src/main.cpp)

target_link_libraries(
  ${PROJECT_NAME} PRIVATE matt-daemon::rpc matt-daemon::interface matt-daemon::options)

matt_daemon_setup_config(${PROJECT_NAME})

matt_daemon_apply_static_analyses(${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME} COMPONENT ${PROJECT_NAME})
