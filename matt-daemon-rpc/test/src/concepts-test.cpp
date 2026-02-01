#include <gtest/gtest.h>

#include <cstdint>
#include <matt-daemon-rpc/annotations.hpp>
#include <matt-daemon-rpc/concepts.hpp>
#include <matt-daemon-rpc/result.hpp>

namespace test::concepts {

enum[[= ::matt_daemon_rpc::service]] ServiceAnnotationEnum {};

class[[= ::matt_daemon_rpc::service]] ServiceAnnotationClass {};

class NoAnnotationClass {};

struct NoAnnotationStruct {};

struct[[= ::matt_daemon_rpc::service]] ServiceAnnotationStruct {
  enum class Foo : std::uint8_t { kBar };

  auto [[= ::matt_daemon_rpc::method]] MethodAnnotationOneArg(Foo)
      -> ::matt_daemon_rpc::Result<void>;
  auto [[= ::matt_daemon_rpc::method]] MethodAnnotationNoArg()
      -> ::matt_daemon_rpc::Result<void>;
  auto [[= ::matt_daemon_rpc::method]]
      [[= ::matt_daemon_rpc::method]] MethodMultiAnnotationNoArg()
          -> ::matt_daemon_rpc::Result<void>;

  auto NoAnnotationNoArg() -> ::matt_daemon_rpc::Result<void>;
};

}  // namespace test::concepts

enum class Foo : std::uint8_t { kBar };

[[= matt_daemon_rpc::method]] auto MethodOneArg(Foo)
    -> ::matt_daemon_rpc::Result<void>;
;

using matt_daemon_rpc::Callable;
using matt_daemon_rpc::Method;
using matt_daemon_rpc::Service;

TEST(ConceptsTest, CallableOnlyAllowsFunctionsInNamespace) {
  static_assert(
      Callable<
          ^^test::concepts::ServiceAnnotationStruct,
          ^^test::concepts::ServiceAnnotationStruct::MethodAnnotationOneArg,
          test::concepts::ServiceAnnotationStruct::Foo>);
}

TEST(ConceptsTest, CallableDoesNotAllowGlobalNamespace) {
  static_assert(
      !Callable<
          ^^::,
          ^^test::concepts::ServiceAnnotationStruct::MethodAnnotationOneArg,
          test::concepts::ServiceAnnotationStruct::Foo>);
}

TEST(ConceptsTest, CallableAllowsCorrectParameterTypes) {
  static_assert(
      Callable<
          ^^test::concepts::ServiceAnnotationStruct,
          ^^test::concepts::ServiceAnnotationStruct::MethodAnnotationOneArg,
          test::concepts::ServiceAnnotationStruct::Foo>);
}

TEST(ConceptsTest, CallableDoesNotAllowWrongParameterTypes) {
  static_assert(
      !Callable<
          ^^test::concepts::ServiceAnnotationStruct,
          ^^test::concepts::ServiceAnnotationStruct::MethodAnnotationOneArg,
          std::uint8_t>);
}

TEST(ConceptsTest, MethodDoesNotAllowFunctionsWithNoAnnotation) {
  static_assert(
      !Method<^^test::concepts::ServiceAnnotationStruct,
              ^^test::concepts::ServiceAnnotationStruct::NoAnnotationNoArg,
              void>);
}

TEST(ConceptsTest, MethodDoesAllowFunctionsWithAnnotation) {
  static_assert(
      Method<^^test::concepts::ServiceAnnotationStruct,
             ^^test::concepts::ServiceAnnotationStruct::MethodAnnotationNoArg>);
}

TEST(ConceptsTest, MethodDoesAllowFunctionsWithMultipleAnnotations) {
  static_assert(!Method<^^test::concepts::ServiceAnnotationStruct,
                        ^^test::concepts::ServiceAnnotationStruct::
                             MethodMultiAnnotationNoArg>);
}

TEST(ConceptsTest, ServiceDoesAllowStructsWithAnnotation) {
  static_assert(Service<^^test::concepts::ServiceAnnotationStruct>);
}

TEST(ConceptsTest, ServiceDoesNotAllowStructsWithNoAnnotation) {
  static_assert(!Service<^^test::concepts::NoAnnotationStruct>);
}

TEST(ConceptsTest, ServiceDoesAllowClassesWithAnnotation) {
  static_assert(Service<^^test::concepts::ServiceAnnotationClass>);
}

TEST(ConceptsTest, ServiceDoesNotAllowClassesWithNoAnnotation) {
  static_assert(!Service<^^test::concepts::NoAnnotationClass>);
}

TEST(ConceptsTest, ServiceDoesNotAllowEnumsWithAnnotation) {
  static_assert(!Service<^^test::concepts::ServiceAnnotationEnum>);
}