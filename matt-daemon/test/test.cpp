#include <gtest/gtest.h>

#include "foo.hpp"

TEST(FooTest, ReturnsEmptyVector) {
  auto vec = foo();
  EXPECT_TRUE(vec.empty());
}
