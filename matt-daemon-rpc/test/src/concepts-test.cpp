#include <gtest/gtest.h>

#include <cstdint>
#include <matt-daemon-rpc/annotations.hpp>
#include <matt-daemon-rpc/concepts.hpp>
#include <matt-daemon-rpc/result.hpp>

namespace {

using ::matt_daemon_rpc::Callable;
using ::matt_daemon_rpc::method;
using ::matt_daemon_rpc::Method;
using ::matt_daemon_rpc::Result;
using ::matt_daemon_rpc::service;
using ::matt_daemon_rpc::Service;

enum[[= service]] ServiceAnnotationEnum {};
class[[= service]] ServiceAnnotationClass {};
class NoAnnotationClass {};
struct NoAnnotationStruct {};

struct[[= service]] ServiceAnnotationStruct {
  enum class Foo : std::uint8_t { kBar };

  auto [[= method]] MethodAnnotationOneArg(Foo) -> Result<void>;
  auto [[= method]] MethodAnnotationNoArg() -> Result<void>;
  auto [[= method]][[= method]] MethodMultiAnnotationNoArg() -> Result<void>;

  auto NoAnnotationNoArg() -> Result<void>;
};

}  // namespace

TEST(ConceptsTest, CallableOnlyAllowsFunctionsInNamespace) {
  static_assert(Callable<^^ServiceAnnotationStruct,
                         ^^ServiceAnnotationStruct::MethodAnnotationOneArg,
                         ServiceAnnotationStruct::Foo>);
}

TEST(ConceptsTest, CallableDoesNotAllowGlobalNamespace) {
  static_assert(!Callable<^^::,
                          ^^ServiceAnnotationStruct::MethodAnnotationOneArg,
                          ServiceAnnotationStruct::Foo>);
}

TEST(ConceptsTest, CallableAllowsCorrectParameterTypes) {
  static_assert(Callable<^^ServiceAnnotationStruct,
                         ^^ServiceAnnotationStruct::MethodAnnotationOneArg,
                         ServiceAnnotationStruct::Foo>);
}

TEST(ConceptsTest, CallableDoesNotAllowWrongParameterTypes) {
  static_assert(!Callable<^^ServiceAnnotationStruct,
                          ^^ServiceAnnotationStruct::MethodAnnotationOneArg,
                          std::uint8_t>);
}

TEST(ConceptsTest, MethodDoesNotAllowFunctionsWithNoAnnotation) {
  static_assert(!Method<^^ServiceAnnotationStruct,
                        ^^ServiceAnnotationStruct::NoAnnotationNoArg,
                        void>);
}

TEST(ConceptsTest, MethodDoesAllowFunctionsWithAnnotation) {
  static_assert(Method<^^ServiceAnnotationStruct,
                       ^^ServiceAnnotationStruct::MethodAnnotationNoArg>);
}

TEST(ConceptsTest, MethodDoesAllowFunctionsWithMultipleAnnotations) {
  static_assert(!Method<^^ServiceAnnotationStruct,
                        ^^ServiceAnnotationStruct::MethodMultiAnnotationNoArg>);
}

TEST(ConceptsTest, ServiceDoesAllowStructsWithAnnotation) {
  static_assert(Service<^^ServiceAnnotationStruct>);
}

TEST(ConceptsTest, ServiceDoesNotAllowStructsWithNoAnnotation) {
  static_assert(!Service<^^NoAnnotationStruct>);
}

TEST(ConceptsTest, ServiceDoesAllowClassesWithAnnotation) {
  static_assert(Service<^^ServiceAnnotationClass>);
}

TEST(ConceptsTest, ServiceDoesNotAllowClassesWithNoAnnotation) {
  static_assert(!Service<^^NoAnnotationClass>);
}

TEST(ConceptsTest, ServiceDoesNotAllowEnumsWithAnnotation) {
  static_assert(!Service<^^ServiceAnnotationEnum>);
}