#ifndef MATT_DAEMON_DAEMONIZER
#define MATT_DAEMON_DAEMONIZER

#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <expected>
#include <file-lock.hpp>
#include <filesystem>
#include <functional>
#include <print>
#include <string>
#include <system_error>
#include <variant>

#include "utils.hpp"

namespace matt_daemon {

enum deamonize_error : std::uint8_t {
  failed_to_create_fd_iter,
  failed_to_create_session,
  failed_to_fork,
  failed_to_open_dev_null,
};

enum daemonize_success : std::uint8_t { should_die, should_live };

namespace detail {

inline auto close_all_fds() -> std::expected<void, deamonize_error> {
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

  return {};
}
}  // namespace detail

// notes: parent exits
struct Daemonizer final {
  template <typename T = std::monostate>
  using PrivilegedAction = std::function<T()>;

  struct Keep {};

  using UserName = std::string;
  using User = std::variant<Keep, uid_t, std::string>;

  using GroupName = std::string;
  using Group = std::variant<Keep, gid_t, GroupName>;

  using FileDescriptorType = int;
  struct DevNull {};
  using Stream = std::variant<DevNull, FileDescriptorType>;

  // NOLINTBEGIN - This struct is inefficient, and there is nothing you can
  // do about it Mr. Linter >:D
  struct Options final {
    // NOLINTEND
    // uses file lock internally to create the pid file
    std::filesystem::path pid_file = "/var/run/matt_daemon/matt_daemon.pid";
    std::filesystem::path dir = "/";
    std::uint32_t umask = 0;
    // those would be cool to be configureable
    User user = Keep{};
    Group group = Keep{};
    Stream in_stream = DevNull{};
    Stream out_stream = DevNull{};
    Stream err_stream = DevNull{};
    // ---
  };

  // NOLINTNEXTLINE - Same as above
  struct Handle final {
    //   FileLock locked_pid_file;
  };

  // we could have more overloads of this but we dont need it
  template <typename T = std::monostate>
  [[nodiscard]] static auto Daemonize(
      const Options &opts, PrivilegedAction<T> privileged_action = {}) noexcept
      -> std::expected<T, deamonize_error> {
    (void)opts;
    (void)privileged_action;

    return {};

    // // Return control to the shell => let shell think command has finished
    // // And essential for the following `setsid` call.
    // pid_t pid = fork();
    // if (pid == -1) {
    //   return std::unexpected(deamonize_error::failed_to_fork);
    // }

    // if (pid > 0) {
    //   return daemonize_success::should_die;
    // }

    // // Create a new session => Detaches from the terminal, so closing the
    // // terminal won't kill the process.
    // auto setsid_res = setsid();
    // if (setsid_res == -1) {
    //   return std::unexpected(deamonize_error::failed_to_create_session);
    // }

    // // Fork again => Only the session leader can obtain a terminal
    // pid = fork();
    // if (pid == -1) {
    //   return std::unexpected(deamonize_error::failed_to_fork);
    // }

    // if (pid > 0) {
    //   return daemonize_success::should_die;
    // }

    // umask(0);
    // chdir(opts.dir.c_str());

    // auto result = detail::close_all_fds();
    // if (!result) {
    //   return std::unexpected(result.error());
    // }

    // // Need to redirect in, out & err to /dev/null. If we just close them,
    // // and
    // // open a file later, it will get lowest available fd, which might be
    // // 0, 1 or 2, and that not good.
    // // NOLINTNEXTLINE - We need the fd
    // const int dev_null_fd = open("/dev/null", O_RDWR);
    // if (dev_null_fd == -1) {
    //   return std::unexpected(deamonize_error::failed_to_open_dev_null);
    // }

    // auto get_fd = [&](const auto &stream) -> auto {
    //   return std::visit(
    //       overloaded{
    //           [dev_null_fd](DevNull) -> auto { return dev_null_fd; },
    //           // NOLINTNEXTLINE - I refuse to rename `fd` to
    //           `file_descriptor`
    //           [](FileDescriptorType fd) -> auto { return fd; }},
    //       stream);
    // };

    // dup2(get_fd(opts.in_stream), STDIN_FILENO);
    // dup2(get_fd(opts.out_stream), STDOUT_FILENO);
    // dup2(get_fd(opts.err_stream), STDERR_FILENO);

    // close(dev_null_fd);

    // auto privileged_return = privileged_action();
  }
};

}  // namespace matt_daemon

#endif  // MATT_DAEMON_DAEMONIZER
