#include <gtest/gtest.h>

#include <filesystem>
#include <system_error>

#include "FileLock.hpp"

using matt_daemon::FileLock;

class FileLockTest : public ::testing::Test {
 protected:
  void TearDown() override {
    std::filesystem::remove(kLockPath1);
    std::filesystem::remove(kLockPath2);
  }

  static constexpr auto kLockPath1 = "/tmp/matt_daemon_test.lock";
  static constexpr auto kLockPath2 = "/tmp/matt_daemon_test2.lock";
};

TEST_F(FileLockTest, AcquireSucceeds) {
  auto lock = FileLock::acquire(kLockPath1);
  ASSERT_TRUE(lock.has_value()) << lock.error().message();
}

TEST_F(FileLockTest, SecondAcquireFails) {
  auto lock1 = FileLock::acquire(kLockPath1);
  ASSERT_TRUE(lock1.has_value()) << lock1.error().message();

  auto lock2 = FileLock::acquire(kLockPath1);
  ASSERT_FALSE(lock2.has_value());
  EXPECT_EQ(lock2.error(), std::errc::resource_unavailable_try_again);
}

TEST_F(FileLockTest, LockReleasedOnDestruction) {
  {
    auto lock = FileLock::acquire(kLockPath1);
    ASSERT_TRUE(lock.has_value()) << lock.error().message();
  }
  auto lock = FileLock::acquire(kLockPath1);
  EXPECT_TRUE(lock.has_value()) << lock.error().message();
}

TEST_F(FileLockTest, MoveConstructorTransfersOwnership) {
  auto lock1 = FileLock::acquire(kLockPath1);
  ASSERT_TRUE(lock1.has_value()) << lock1.error().message();

  FileLock lock2{std::move(*lock1)};
  auto lock3 = FileLock::acquire(kLockPath1);
  EXPECT_FALSE(lock3.has_value());
}

TEST_F(FileLockTest, MoveAssignmentTransfersOwnership) {
  auto lock1 = FileLock::acquire(kLockPath1);
  auto lock2 = FileLock::acquire(kLockPath2);

  ASSERT_TRUE(lock1.has_value()) << lock1.error().message();
  ASSERT_TRUE(lock2.has_value()) << lock2.error().message();

  *lock1 = std::move(*lock2);
  auto lock3 = FileLock::acquire(kLockPath1);
  EXPECT_TRUE(lock3.has_value()) << lock3.error().message();

  auto lock4 = FileLock::acquire(kLockPath2);
  EXPECT_FALSE(lock4.has_value());
}

TEST_F(FileLockTest, MovedFromObjectSafeToDestroy) {
  auto lock1 = FileLock::acquire(kLockPath1);
  ASSERT_TRUE(lock1.has_value()) << lock1.error().message();
  FileLock lock2{std::move(*lock1)};
}
