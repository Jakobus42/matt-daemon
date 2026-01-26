#ifndef MATT_DAEMON_FILE_LOCK
#define MATT_DAEMON_FILE_LOCK

#include <expected>
#include <filesystem>

namespace matt_daemon {

class FileLock final {
 public:
  [[nodiscard]] static auto Acquire(std::filesystem::path path)
      -> std::expected<FileLock, std::error_code>;

  ~FileLock() noexcept;
  FileLock(FileLock&& other) noexcept;
  auto operator=(FileLock&& other) noexcept -> FileLock&;

  FileLock(const FileLock&) = delete;
  auto operator=(const FileLock&) -> FileLock& = delete;

 private:
  using FileDescriptorType = int;

  static constexpr FileDescriptorType kInvalidFd{-1};

  explicit FileLock(FileDescriptorType file_desc,
                    std::filesystem::path path) noexcept;

  auto Release() noexcept -> void;

  FileDescriptorType file_desc_{kInvalidFd};
  std::filesystem::path path_;
};

}  // namespace matt_daemon

#endif  // MATT_DAEMON_FILE_LOCK
