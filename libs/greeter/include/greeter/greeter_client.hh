#pragma once

#include <memory>
#include <string>
#include <vector>

// gRPC client wrapping the generated Greeter stub.
// Uses the pimpl idiom so consumers do not need to include generated headers.
class GreeterClient {
public:
  explicit GreeterClient(std::string address);
  ~GreeterClient();

  GreeterClient(const GreeterClient&) = delete;
  GreeterClient& operator=(const GreeterClient&) = delete;

  // Unary RPC: returns the server's greeting or an empty string on failure.
  std::string SayHello(const std::string& name);

  // Server-streaming RPC: collects every reply into a vector.
  std::vector<std::string> SayHelloStream(const std::string& name);

private:
  struct Impl;
  std::unique_ptr<Impl> pimpl_;
};

// Smoke-test: starts a local server, makes both RPC calls, shuts down.
void GreeterTest();
