#pragma once

#ifndef MATT_DAEMON_UTILS
#define MATT_DAEMON_UTILS

#include <cstdint>
#include <expected>

enum deamonize_error : std::uint8_t {
  failed_to_create_fd_iter,
  failed_to_create_session,
  failed_to_fork,
  failed_to_open_dev_null,
};

enum daemonize_success : std::uint8_t { should_die, should_live };

auto daemonize() -> std::expected<daemonize_success, deamonize_error>;

#endif
