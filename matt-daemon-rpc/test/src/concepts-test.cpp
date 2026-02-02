

#include "matt-daemon-rpc/concepts.hpp"

#include <gtest/gtest.h>

#include <optional>
#include <string>
#include <vector>

#include "matt-daemon-rpc/annotations.hpp"
#include "matt-daemon-rpc/future-result.hpp"
#include "matt-daemon-rpc/error.hpp"

using matt_daemon_rpc::Callable;
using matt_daemon_rpc::FutureResult;
using matt_daemon_rpc::method;
using matt_daemon_rpc::Method;
using matt_daemon_rpc::service;
using matt_daemon_rpc::Service;

TEST(ConceptsTest, ServiceDoesAllowAnnotatedStructs) {
  struct[[= service]] AnnotatedStruct {};

  static_assert(Service<^^AnnotatedStruct>);
}

TEST(ConceptsTest, ServiceDoesNotAllowUnannotatedStructs) {
  struct UnannotatedStruct {};

  static_assert(!Service<^^UnannotatedStruct>);
}

TEST(ConceptsTest, ServiceDoesAllowAnnotatedClasses) {
  class[[= service]] AnnotatedClass {};

  static_assert(Service<^^AnnotatedClass>);
}

TEST(ConceptsTest, ServiceDoesNotAllowUnannotatedClasses) {
  class UnannotatedClass {};

  static_assert(!Service<^^UnannotatedClass>);
}

TEST(ConceptsTest, ServiceDoesNotAllowAnnotatedEnums) {
  enum class[[= service]] AnnotatedEnum {};

  static_assert(!Service<^^AnnotatedEnum>);
}

TEST(ConceptsTest, CallableDoesNotAllowUnannotatedService) {
  struct UnannotatedServiceStruct {
    auto UnannotatedNoParameterValidResult() -> FutureResult<void>;
  };

  static_assert(!Service<^^UnannotatedServiceStruct>);
  static_assert(
      !Callable<^^UnannotatedServiceStruct,
                ^^UnannotatedServiceStruct::UnannotatedNoParameterValidResult>);
}

TEST(ConceptsTest, CallableAllowsFunctionsInService) {
  struct[[= service]] AnnotatedServiceStruct {
    auto UnannotatedNoParameterValidResult() -> FutureResult<void>;
  };

  static_assert(Service<^^AnnotatedServiceStruct>);
  static_assert(
      Callable<^^AnnotatedServiceStruct,
               ^^AnnotatedServiceStruct::UnannotatedNoParameterValidResult>);
}

TEST(ConceptsTest, CallableDoesNotAllowFunctionsOutsideService) {
  struct[[= service]] AnnotatedServiceStruct {};
  auto UnannotatedNoParameterValidResult() -> FutureResult<void>;

  static_assert(
      !Callable<^^AnnotatedServiceStruct, ^^UnannotatedNoParameterValidResult>);
}

TEST(ConceptsTest, CallableDoesAllowCorrectParameterTypes) {
  struct[[= service]] AnnotatedServiceStruct {
    auto UnannotatedOneParameterValidResult(int) -> FutureResult<void>;
  };

  static_assert(
      Callable<^^AnnotatedServiceStruct,
               ^^AnnotatedServiceStruct::UnannotatedOneParameterValidResult,
               int>);
}

TEST(ConceptsTest, CallableDoesNotAllowWrongParameterTypes) {
  struct[[= service]] AnnotatedServiceStruct {
    auto UnannotatedOneParameterValidResult(int) -> FutureResult<void>;
  };

  static_assert(
      !Callable<^^AnnotatedServiceStruct,
                ^^AnnotatedServiceStruct::UnannotatedOneParameterValidResult,
                char>);
}

TEST(ConceptsTest, CallableDoesAllowComplexParameterTypes) {
  struct[[= service]] AnnotatedServiceStruct {
    // NOLINTNEXTLINE(altera-struct-pack-align)
    struct ComplexStruct {
      std::vector<std::string> foo;
      std::array<std::string, 1> bar;
    };

    auto UnannotatedOneComplexParameterValidResult(ComplexStruct)
        -> FutureResult<void>;
  };

  static_assert(
      Callable<
          ^^AnnotatedServiceStruct,
          ^^AnnotatedServiceStruct::UnannotatedOneComplexParameterValidResult,
          AnnotatedServiceStruct::ComplexStruct>);
}

TEST(ConceptsTest, CallableDoesNotAllowSimilarComplexParameterTypes) {
  struct[[= service]] AnnotatedServiceStruct {
    // NOLINTNEXTLINE(altera-struct-pack-align)
    struct ComplexStruct {
      std::vector<std::string> foo;
      std::array<std::string, 1> bar;
    };

    // NOLINTNEXTLINE(altera-struct-pack-align)
    struct ComplexSimilarStruct {
      std::vector<std::string> foo;
      std::array<std::string, 1> bar;
    };

    auto UnannotatedOneComplexParameterValidResult(ComplexStruct)
        -> FutureResult<void>;
  };

  static_assert(
      !Callable<
          ^^AnnotatedServiceStruct,
          ^^AnnotatedServiceStruct::UnannotatedOneComplexParameterValidResult,
          AnnotatedServiceStruct::ComplexSimilarStruct>);
}

TEST(ConceptsTest, CallableDoesAllowCorrectReturnType) {
  struct[[= service]] AnnotatedServiceStruct {
    auto UnannotatedNoParameterValidResult() -> FutureResult<void>;
  };

  static_assert(
      Callable<^^AnnotatedServiceStruct,
               ^^AnnotatedServiceStruct::UnannotatedNoParameterValidResult>);
}

TEST(ConceptsTest, CallableDoesNotAllowWrongReturnType) {
  struct[[= service]] AnnotatedServiceStruct {
    auto UnannotatedNoParameterInvalidResult() -> bool;
  };

  static_assert(
      !Callable<^^AnnotatedServiceStruct,
                ^^AnnotatedServiceStruct::UnannotatedNoParameterInvalidResult>);
}

TEST(ConceptsTest, CallableDoesNotAllowWrongTemplatedReturnType) {
  struct[[= service]] AnnotatedServiceStruct {
    auto UnannotatedNoParameterInvalidResult() -> std::optional<bool>;
  };

  static_assert(
      !Callable<^^AnnotatedServiceStruct,
                ^^AnnotatedServiceStruct::UnannotatedNoParameterInvalidResult>);
}

TEST(ConceptsTest, CallableDoesAllowCustomError) {
  struct[[= service]] AnnotatedServiceStruct {
    struct Error {};
    auto UnannotatedNoParameterValidResultWithCustomError()
        -> FutureResult<int, ::matt_daemon_rpc::Error<Error>>;
  };

  static_assert(
      Callable<^^AnnotatedServiceStruct,
               ^^AnnotatedServiceStruct::
                    UnannotatedNoParameterValidResultWithCustomError>);
}

TEST(ConceptsTest, MethodDoesNotAllowNonCallableFunctions) {
  struct[[= service]] AnnotatedServiceStruct {
    auto [[= method]] AnnotatedMethodOneParameterValidResult(int)
        -> FutureResult<void>;
  };

  static_assert(
      !Callable<
          ^^AnnotatedServiceStruct,
          ^^AnnotatedServiceStruct::AnnotatedMethodOneParameterValidResult>);
  static_assert(
      !Method<
          ^^AnnotatedServiceStruct,
          ^^AnnotatedServiceStruct::AnnotatedMethodOneParameterValidResult>);
}

TEST(ConceptsTest, MethodAllowsCallableFunctions) {
  struct[[= service]] AnnotatedServiceStruct {
    auto [[= method]] AnnotatedMethodOneParameterValidResult(int)
        -> FutureResult<void>;
  };

  static_assert(
      Callable<^^AnnotatedServiceStruct,
               ^^AnnotatedServiceStruct::AnnotatedMethodOneParameterValidResult,
               int>);
  static_assert(
      Method<^^AnnotatedServiceStruct,
             ^^AnnotatedServiceStruct::AnnotatedMethodOneParameterValidResult,
             int>);
}

TEST(ConceptsTest, MethodDoesAllowAnnotatedFunctions) {
  struct[[= service]] AnnotatedServiceStruct {
    auto [[= method]] AnnotatedMethodNoParameterValidResult()
        -> FutureResult<void>;
  };

  static_assert(
      Method<^^AnnotatedServiceStruct,
             ^^AnnotatedServiceStruct::AnnotatedMethodNoParameterValidResult>);
}

TEST(ConceptsTest, MethodDoesNotAllowUnannotatedFunction) {
  struct[[= service]] AnnotatedServiceStruct {
    auto UnannotatedNoParameterValidResult() -> FutureResult<void>;
  };

  static_assert(
      !Method<^^AnnotatedServiceStruct,
              ^^AnnotatedServiceStruct::UnannotatedNoParameterValidResult>);
}
