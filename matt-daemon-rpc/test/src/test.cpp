#include <gtest/gtest.h>

#include <matt-daemon-rpc/foo.hpp>

TEST(FooTest, ReturnsEmptyVector) {
  EXPECT_EQ(matt_daemon_rpc::foo(), 1);
}
