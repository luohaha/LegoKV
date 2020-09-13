// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: simple_paxos.proto

#include "simple_paxos.pb.h"
#include "simple_paxos.grpc.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace simplepaxos {

static const char* SimplePaxos_method_names[] = {
  "/simplepaxos.SimplePaxos/HandleAccept",
  "/simplepaxos.SimplePaxos/HandlePrepare",
};

std::unique_ptr< SimplePaxos::Stub> SimplePaxos::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< SimplePaxos::Stub> stub(new SimplePaxos::Stub(channel));
  return stub;
}

SimplePaxos::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_HandleAccept_(SimplePaxos_method_names[0], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_HandlePrepare_(SimplePaxos_method_names[1], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status SimplePaxos::Stub::HandleAccept(::grpc::ClientContext* context, const ::simplepaxos::Accept& request, ::simplepaxos::AcceptRet* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_HandleAccept_, context, request, response);
}

void SimplePaxos::Stub::experimental_async::HandleAccept(::grpc::ClientContext* context, const ::simplepaxos::Accept* request, ::simplepaxos::AcceptRet* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_HandleAccept_, context, request, response, std::move(f));
}

void SimplePaxos::Stub::experimental_async::HandleAccept(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::simplepaxos::AcceptRet* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_HandleAccept_, context, request, response, std::move(f));
}

void SimplePaxos::Stub::experimental_async::HandleAccept(::grpc::ClientContext* context, const ::simplepaxos::Accept* request, ::simplepaxos::AcceptRet* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_HandleAccept_, context, request, response, reactor);
}

void SimplePaxos::Stub::experimental_async::HandleAccept(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::simplepaxos::AcceptRet* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_HandleAccept_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::simplepaxos::AcceptRet>* SimplePaxos::Stub::AsyncHandleAcceptRaw(::grpc::ClientContext* context, const ::simplepaxos::Accept& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::simplepaxos::AcceptRet>::Create(channel_.get(), cq, rpcmethod_HandleAccept_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::simplepaxos::AcceptRet>* SimplePaxos::Stub::PrepareAsyncHandleAcceptRaw(::grpc::ClientContext* context, const ::simplepaxos::Accept& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::simplepaxos::AcceptRet>::Create(channel_.get(), cq, rpcmethod_HandleAccept_, context, request, false);
}

::grpc::Status SimplePaxos::Stub::HandlePrepare(::grpc::ClientContext* context, const ::simplepaxos::Prepare& request, ::simplepaxos::PrepareRet* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_HandlePrepare_, context, request, response);
}

void SimplePaxos::Stub::experimental_async::HandlePrepare(::grpc::ClientContext* context, const ::simplepaxos::Prepare* request, ::simplepaxos::PrepareRet* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_HandlePrepare_, context, request, response, std::move(f));
}

void SimplePaxos::Stub::experimental_async::HandlePrepare(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::simplepaxos::PrepareRet* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_HandlePrepare_, context, request, response, std::move(f));
}

void SimplePaxos::Stub::experimental_async::HandlePrepare(::grpc::ClientContext* context, const ::simplepaxos::Prepare* request, ::simplepaxos::PrepareRet* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_HandlePrepare_, context, request, response, reactor);
}

void SimplePaxos::Stub::experimental_async::HandlePrepare(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::simplepaxos::PrepareRet* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_HandlePrepare_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::simplepaxos::PrepareRet>* SimplePaxos::Stub::AsyncHandlePrepareRaw(::grpc::ClientContext* context, const ::simplepaxos::Prepare& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::simplepaxos::PrepareRet>::Create(channel_.get(), cq, rpcmethod_HandlePrepare_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::simplepaxos::PrepareRet>* SimplePaxos::Stub::PrepareAsyncHandlePrepareRaw(::grpc::ClientContext* context, const ::simplepaxos::Prepare& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::simplepaxos::PrepareRet>::Create(channel_.get(), cq, rpcmethod_HandlePrepare_, context, request, false);
}

SimplePaxos::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      SimplePaxos_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< SimplePaxos::Service, ::simplepaxos::Accept, ::simplepaxos::AcceptRet>(
          [](SimplePaxos::Service* service,
             ::grpc_impl::ServerContext* ctx,
             const ::simplepaxos::Accept* req,
             ::simplepaxos::AcceptRet* resp) {
               return service->HandleAccept(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      SimplePaxos_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< SimplePaxos::Service, ::simplepaxos::Prepare, ::simplepaxos::PrepareRet>(
          [](SimplePaxos::Service* service,
             ::grpc_impl::ServerContext* ctx,
             const ::simplepaxos::Prepare* req,
             ::simplepaxos::PrepareRet* resp) {
               return service->HandlePrepare(ctx, req, resp);
             }, this)));
}

SimplePaxos::Service::~Service() {
}

::grpc::Status SimplePaxos::Service::HandleAccept(::grpc::ServerContext* context, const ::simplepaxos::Accept* request, ::simplepaxos::AcceptRet* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status SimplePaxos::Service::HandlePrepare(::grpc::ServerContext* context, const ::simplepaxos::Prepare* request, ::simplepaxos::PrepareRet* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace simplepaxos

