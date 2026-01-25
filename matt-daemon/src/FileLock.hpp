#ifndef MATT_DAEMON_FILE_LOCK
#define MATT_DAEMON_FILE_LOCK

#include <expected>
#include <filesystem>

namespace matt_daemon {

class FileLock final {
 public:
  [[nodiscard]] static auto acquire(std::filesystem::path const& path)
      -> std::expected<FileLock, std::error_code>;

  ~FileLock() noexcept;
  FileLock(FileLock&& other) noexcept;
  auto operator=(FileLock&& other) noexcept -> FileLock&;

  FileLock(const FileLock&) = delete;
  auto operator=(const FileLock&) -> FileLock& = delete;

 private:
  using FileDescriptorType = int;

  static constexpr FileDescriptorType kInvalidFd{-1};

  explicit FileLock(FileDescriptorType file_desc) noexcept;

  auto release() noexcept -> void;

  FileDescriptorType file_desc_{kInvalidFd};
};

}  // namespace matt_daemon

#endif  // MATT_DAEMON_FILE_LOCK
