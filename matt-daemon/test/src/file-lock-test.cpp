#include <fcntl.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <system_error>

#include "file-lock.hpp"

using matt_daemon::FileLock;

namespace {
constexpr auto kLockPath1 = "/tmp/matt_daemon_test.lock";
constexpr auto kLockPath2 = "/tmp/matt_daemon_test2.lock";
}  // namespace

TEST(FileLockTest, AcquireSucceeds) {
  auto lock = FileLock::Acquire(kLockPath1);
  ASSERT_TRUE(lock.has_value()) << lock.error().message();
}

TEST(FileLockTest, SecondAcquireFails) {
  auto lock1 = FileLock::Acquire(kLockPath1);
  ASSERT_TRUE(lock1.has_value()) << lock1.error().message();

  auto lock2 = FileLock::Acquire(kLockPath1);
  ASSERT_FALSE(lock2.has_value());
  EXPECT_EQ(lock2.error(), std::errc::resource_unavailable_try_again);
}

TEST(FileLockTest, LockReleasedOnDestruction) {
  {
    auto lock = FileLock::Acquire(kLockPath1);
    ASSERT_TRUE(lock.has_value()) << lock.error().message();
  }
  auto lock = FileLock::Acquire(kLockPath1);
  EXPECT_TRUE(lock.has_value()) << lock.error().message();
}

TEST(FileLockTest, MoveConstructorTransfersOwnership) {
  auto lock1 = FileLock::Acquire(kLockPath1);
  ASSERT_TRUE(lock1.has_value()) << lock1.error().message();

  FileLock lock2{std::move(*lock1)};
  auto lock3 = FileLock::Acquire(kLockPath1);
  EXPECT_FALSE(lock3.has_value());
}

TEST(FileLockTest, MoveAssignmentTransfersOwnership) {
  auto lock1 = FileLock::Acquire(kLockPath1);
  auto lock2 = FileLock::Acquire(kLockPath2);

  ASSERT_TRUE(lock1.has_value()) << lock1.error().message();
  ASSERT_TRUE(lock2.has_value()) << lock2.error().message();

  *lock1 = std::move(*lock2);
  auto lock3 = FileLock::Acquire(kLockPath1);
  EXPECT_TRUE(lock3.has_value()) << lock3.error().message();

  auto lock4 = FileLock::Acquire(kLockPath2);
  EXPECT_FALSE(lock4.has_value());
}

TEST(FileLockTest, MovedFromObjectSafeToDestroy) {
  auto lock1 = FileLock::Acquire(kLockPath1);
  ASSERT_TRUE(lock1.has_value()) << lock1.error().message();
  FileLock lock2{std::move(*lock1)};
}

// This is a subject requirement:
// When the daemon shuts down, the matt_daemon.lock file must be deleted
TEST(FileLockTest, FileDeletedOnRelease) {
  {
    auto lock = FileLock::Acquire(kLockPath1);
    ASSERT_TRUE(lock.has_value()) << lock.error().message();
    EXPECT_TRUE(std::filesystem::exists(kLockPath1));
  }
  EXPECT_FALSE(std::filesystem::exists(kLockPath1));
}

// Testing for race condition which happens due to subject requirement:
// When the daemon shuts down, the matt_daemon.lock file must be deleted
// 1. A Locks
// 2. A Unlocks
// 3. B Finds Existing Lockfile And Locks
// 4. A Deletes Lockfile
// 5. C Creates New Lockfile And Locks
// 6. Two instances are running!!
TEST(FileLockTest, FileDeletedAfterLock) {
  // auto lock1 = FileLock::Acquire(kLockPath1);
  // ASSERT_TRUE(lock1.has_value()) << lock1.error().message();
  // EXPECT_TRUE(std::filesystem::exists(kLockPath1));

  // NOLINTNEXTLINE
  int fd = ::open(kLockPath1, O_CREAT | O_WRONLY | O_TRUNC);
  ASSERT_NE(fd, -1);

  auto lock2 = FileLock::Acquire(kLockPath1);
  ASSERT_FALSE(lock2.has_value());
  EXPECT_EQ(lock2.error(), std::errc::resource_unavailable_try_again);

  std::filesystem::remove(kLockPath1);
}
