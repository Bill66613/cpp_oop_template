#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include <grpcpp/grpcpp.h>
#include "greeter.grpc.pb.h"

#include "greeter_client.hh"
#include "greeter_server.hh"

using greeter::Greeter;
using greeter::HelloReply;
using greeter::HelloRequest;
using grpc::ClientContext;
using grpc::Status;

// ── pimpl body ───────────────────────────────────────────────────────────────
struct GreeterClient::Impl {
  std::unique_ptr<Greeter::Stub> stub;
};

GreeterClient::GreeterClient(std::string address)
    : pimpl_(std::make_unique<Impl>())
{
  auto channel = grpc::CreateChannel(std::move(address),
                                     grpc::InsecureChannelCredentials());
  pimpl_->stub = Greeter::NewStub(channel);
}

GreeterClient::~GreeterClient() = default;

std::string GreeterClient::SayHello(const std::string& name)
{
  HelloRequest req;
  req.set_name(name);

  HelloReply reply;
  ClientContext ctx;
  Status status = pimpl_->stub->SayHello(&ctx, req, &reply);
  if (!status.ok()) {
    std::cerr << "GreeterClient::SayHello failed: "
              << status.error_message() << "\n";
    return {};
  }
  return reply.message();
}

std::vector<std::string> GreeterClient::SayHelloStream(const std::string& name)
{
  HelloRequest req;
  req.set_name(name);

  ClientContext ctx;
  auto reader = pimpl_->stub->SayHelloStream(&ctx, req);

  std::vector<std::string> messages;
  HelloReply reply;
  while (reader->Read(&reply)) {
    messages.push_back(reply.message());
  }

  Status status = reader->Finish();
  if (!status.ok()) {
    std::cerr << "GreeterClient::SayHelloStream failed: "
              << status.error_message() << "\n";
  }
  return messages;
}

// ── smoke test ───────────────────────────────────────────────────────────────
void GreeterTest()
{
  const std::string address = "localhost:50051";

  // Run the server on a background thread so main can continue.
  GreeterServer server(address);
  std::thread server_thread([&server] { server.Run(); });

  // Give the server a moment to start accepting connections.
  std::this_thread::sleep_for(std::chrono::milliseconds(200));

  {
    GreeterClient client(address);

    // Unary RPC
    std::string reply = client.SayHello("World");
    std::cout << "Unary:  " << reply << "\n";

    // Server-streaming RPC
    for (const auto& msg : client.SayHelloStream("World")) {
      std::cout << "Stream: " << msg << "\n";
    }
  }

  server.Shutdown();
  server_thread.join();
}
