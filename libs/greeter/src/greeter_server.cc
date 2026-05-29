#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "greeter.grpc.pb.h"

#include "greeter_server.hh"

using greeter::Greeter;
using greeter::HelloReply;
using greeter::HelloRequest;
using grpc::ServerContext;
using grpc::ServerWriter;
using grpc::Status;

// Service implementation — kept private to this translation unit.
class GreeterServiceImpl final : public Greeter::Service {
public:
  Status SayHello(ServerContext* /*ctx*/,
                  const HelloRequest* req,
                  HelloReply* reply) override
  {
    reply->set_message("Hello, " + req->name() + "!");
    return Status::OK;
  }

  Status SayHelloStream(ServerContext* /*ctx*/,
                        const HelloRequest* req,
                        ServerWriter<HelloReply>* writer) override
  {
    for (int i = 1; i <= 3; ++i) {
      HelloReply reply;
      reply.set_message("Hello #" + std::to_string(i) + ", " + req->name() + "!");
      writer->Write(reply);
    }
    return Status::OK;
  }
};

// ── pimpl body ───────────────────────────────────────────────────────────────
struct GreeterServer::Impl {
  std::string          address;
  GreeterServiceImpl   service;
  std::unique_ptr<grpc::Server> server;
};

GreeterServer::GreeterServer(std::string address)
    : pimpl_(std::make_unique<Impl>())
{
  pimpl_->address = std::move(address);
}

GreeterServer::~GreeterServer()
{
  Shutdown();
}

void GreeterServer::Run()
{
  grpc::ServerBuilder builder;
  builder.AddListeningPort(pimpl_->address, grpc::InsecureServerCredentials());
  builder.RegisterService(&pimpl_->service);
  pimpl_->server = builder.BuildAndStart();
  std::cout << "GreeterServer: listening on " << pimpl_->address << "\n";
  pimpl_->server->Wait();  // blocks until Shutdown()
}

void GreeterServer::Shutdown()
{
  if (pimpl_ && pimpl_->server) {
    pimpl_->server->Shutdown();
    pimpl_->server.reset();
  }
}
