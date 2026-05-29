#pragma once

#include <memory>
#include <string>

// gRPC server wrapping the generated Greeter service.
// Uses the pimpl idiom so consumers do not need to include generated headers.
class GreeterServer {
public:
  explicit GreeterServer(std::string address);
  ~GreeterServer();

  GreeterServer(const GreeterServer&) = delete;
  GreeterServer& operator=(const GreeterServer&) = delete;

  // Starts the server and blocks until Shutdown() is called.
  void Run();

  // Requests a graceful shutdown; unblocks a concurrent Run() call.
  void Shutdown();

private:
  struct Impl;
  std::unique_ptr<Impl> pimpl_;
};
