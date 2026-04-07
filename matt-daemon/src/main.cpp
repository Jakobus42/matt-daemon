#include <print>

#include "daemonizer.hpp"

auto main() -> int {
  std::println("Hello World!");

  using matt_daemon::Daemonizer;
  auto socket = Daemonizer::Daemonize(
      Daemonizer::Options{
          // pass stuff from config
          .dir = std::filesystem::path{"/"},
          .in_stream = Daemonizer::Stream{Daemonizer::DevNull{}}},
      Daemonizer::PrivilegedAction<int>{
          []() -> auto { return /*some privilidged socket*/ 1; }});

  (void)socket;
}
