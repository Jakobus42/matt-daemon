// This could be replaced with
// https://www.boost.org/doc/libs/1_35_0/doc/html/boost/interprocess/file_lock.html

#include <fcntl.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>

#include <expected>
#include <filesystem>
#include <print>
#include <system_error>
#include <utility>

#include "file-lock.hpp"

namespace matt_daemon {

[[nodiscard]] auto FileLock::Acquire(std::filesystem::path path)
    -> std::expected<FileLock, std::error_code> {
  // NOLINTNEXTLINE - std::fstream does not expose native handle in c++23
  auto file_desc = ::open(path.c_str(), O_CREAT | O_RDWR | O_CLOEXEC, 0644);
  if (file_desc == -1) {
    return std::unexpected{std::error_code{errno, std::generic_category()}};
  }

  if (::flock(file_desc, LOCK_EX | LOCK_NB) == -1) {
    ::close(file_desc);
    return std::unexpected{std::error_code{errno, std::generic_category()}};
  }

  struct stat fd_stat{};
  auto fstat_res = fstat(file_desc, &fd_stat);
  if (fstat_res == -1) {
    ::close(file_desc);
    return std::unexpected{std::error_code{errno, std::generic_category()}};
  }

  struct stat file_stat{};
  auto stat_res = stat(path.c_str(), &file_stat);
  if (stat_res == -1) {
    ::close(file_desc);
    return std::unexpected{std::error_code{errno, std::generic_category()}};
  }

  std::println("{} => {}", fd_stat.st_ino, file_stat.st_ino);

  if (fd_stat.st_ino != file_stat.st_ino) {
    ::close(file_desc);
    // TODO: better error
    return std::unexpected{std::error_code{errno, std::generic_category()}};
  }

  return FileLock{file_desc, std::move(path)};
}

FileLock::FileLock(FileDescriptorType file_desc,
                   std::filesystem::path path) noexcept
    : file_desc_{file_desc}, path_{std::move(path)} {
}

FileLock::~FileLock() noexcept {
  Release();
}

FileLock::FileLock(FileLock&& other) noexcept
    : file_desc_{std::exchange(other.file_desc_, kInvalidFd)},
      path_{std::move(other.path_)} {
}

auto FileLock::operator=(FileLock&& other) noexcept -> FileLock& {
  if (this != &other) {
    Release();
    file_desc_ = std::exchange(other.file_desc_, kInvalidFd);
    path_ = std::move(other.path_);
  }
  return *this;
}

auto FileLock::Release() noexcept -> void {
  if (file_desc_ != kInvalidFd) {
    ::flock(file_desc_, LOCK_UN);
    ::close(file_desc_);
    std::filesystem::remove(path_);
    file_desc_ = kInvalidFd;
  }
}

}  // namespace matt_daemon
