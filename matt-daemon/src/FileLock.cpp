// This could be replaced with
// https://www.boost.org/doc/libs/1_35_0/doc/html/boost/interprocess/file_lock.html

#include "FileLock.hpp"

#include <fcntl.h>
#include <sys/file.h>
#include <unistd.h>

#include <expected>
#include <filesystem>
#include <system_error>
#include <utility>

namespace matt_daemon {

[[nodiscard]] auto FileLock::acquire(const std::filesystem::path& path)
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

  return FileLock{file_desc};
}

FileLock::FileLock(FileDescriptorType file_desc) noexcept
    : file_desc_{file_desc} {
}

FileLock::~FileLock() noexcept {
  release();
}

FileLock::FileLock(FileLock&& other) noexcept
    : file_desc_{std::exchange(other.file_desc_, kInvalidFd)} {
}

auto FileLock::operator=(FileLock&& other) noexcept -> FileLock& {
  if (this != &other) {
    release();
    file_desc_ = std::exchange(other.file_desc_, kInvalidFd);
  }
  return *this;
}

void FileLock::release() noexcept {
  if (file_desc_ != kInvalidFd) {
    // Release lock but dont delete file (avoids race condition on delete)
    ::flock(file_desc_, LOCK_UN);
    ::close(file_desc_);
    file_desc_ = kInvalidFd;
  }
}

}  // namespace matt_daemon
