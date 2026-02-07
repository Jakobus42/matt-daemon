#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <csignal>
#include <cstdlib>
#include <expected>
#include <print>
#include <string>

#include "utils.hpp"

namespace {
auto close_all_fds() -> std::expected<void, deamonize_error> {
  // Using opendir instead of C++ builtin `std::filesystem::directory_iterator`,
  // because we need to get the fd the iterator uses, so we don't close it too
  // early.
  DIR *dir = opendir("/proc/self/fd");
  if (dir == nullptr) {
    return std::unexpected(deamonize_error::failed_to_create_fd_iter);
  }

  int dir_fd = dirfd(dir);
  if (dir_fd == -1) {
    closedir(dir);
    return std::unexpected(deamonize_error::failed_to_create_fd_iter);
  }

  struct dirent *file = nullptr;
  // NOLINTNEXTLINE - readdir is not threadsafe, but we are not multithreading
  while ((file = readdir(dir)) != nullptr) {
    // Linter complains if I use file->d_name....
    const std::string raw_filename = &file->d_name[0];
    if (raw_filename == "." || raw_filename == "..") {
      continue;
    }

    // NOLINTBEGIN - Everyone knows what's meant with fd, and if not they
    // should not read this code
    // We should be able to trust the file name here to be a valid number.
    const int fd = stoi(raw_filename);
    // NOLINTEND
    if (fd == dir_fd) {
      continue;
    }

    if (fd == STDIN_FILENO || fd == STDOUT_FILENO || fd == STDERR_FILENO) {
      continue;
    }

    // valgrind opens some fds, we don't need to close those during testing
    // if (fd >= 1024)
    //   continue;

    close(fd);
  }

  closedir(dir);

  // Need to redirect in, out & err to /dev/null. If we just close them, and
  // open a file later, it will get lowest available fd, which might be
  // 0, 1 or 2, and that not good.
  // NOLINTNEXTLINE - We need the fd
  const int dev_null_fd = open("/dev/null", O_RDWR);
  if (dev_null_fd == -1) {
    return std::unexpected(deamonize_error::failed_to_open_dev_null);
  }

  dup2(dev_null_fd, STDIN_FILENO);
  dup2(dev_null_fd, STDOUT_FILENO);
  dup2(dev_null_fd, STDERR_FILENO);

  close(dev_null_fd);

  return {};
}
}  // namespace

// libc provides a daemon() function (man 3 daemon). It would be smarter to use
// that one, but for educational purposes we do it manually.
auto daemonize() -> std::expected<daemonize_success, deamonize_error> {
  // Return control to the shell => let shell think command has finished
  // And essential for the following `setsid` call.
  pid_t pid = fork();
  if (pid == -1) {
    return std::unexpected(deamonize_error::failed_to_fork);
  }

  if (pid > 0) {
    return daemonize_success::should_die;
  }

  // Create a new session => Detaches from the terminal, so closing the terminal
  // won't kill the process.
  auto setsid_res = setsid();
  if (setsid_res == -1) {
    return std::unexpected(deamonize_error::failed_to_create_session);
  }

  // Fork again => Only the session leader can obtain a terminal
  pid = fork();
  if (pid == -1) {
    return std::unexpected(deamonize_error::failed_to_fork);
  }

  if (pid > 0) {
    return daemonize_success::should_die;
  }

  umask(0);
  chdir("/");

  std::println("WILL CLOSE ALL FDS. THIS IS MY FINAL MESSAGE");

  auto result = close_all_fds();
  if (!result) {
    return std::unexpected(result.error());
  }

  return daemonize_success::should_live;
}
