#include <gtest/gtest.h>

#include <matt-daemon-rpc/annotations.hpp>
#include <matt-daemon-rpc/future-result.hpp>
#include <matt-daemon-rpc/client-port.hpp>
#include <matt-daemon-rpc/concepts.hpp>

using matt_daemon_rpc::ClientPort;
using matt_daemon_rpc::FutureResult;
using matt_daemon_rpc::method;
using matt_daemon_rpc::service;

TEST(ClientPortTest, CallAllowsValidServiceMethods) {
  struct[[= service]] TestService {
    auto [[= method]] TestMethod() -> FutureResult<bool>;
  };

  using matt_daemon_rpc::Method;
  static_assert(Method<^^TestService, ^^TestService::TestMethod>);
  static_assert(requires(ClientPort<^^TestService> client) {
    {
      client.Call<^^TestService::TestMethod>()
    } -> std::same_as<FutureResult<bool>>;
  });
}
