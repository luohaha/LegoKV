#include "lkv.pb.h"
#include "lkv.grpc.pb.h"
#include "lkv.h"
#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/grpcpp.h>

using namespace grpc;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

namespace lkv
{
namespace Rpc
{

class LKVServiceImpl final : public lkvrpc::LKV::Service
{
public:
  LKVServiceImpl(LuoKV *lkv) : lkv_(lkv) {}
  ~LKVServiceImpl() {}
  Status Get(ServerContext *context, const lkvrpc::Key *request,
             lkvrpc::Value *reply) override;
  Status Put(ServerContext *context, const lkvrpc::KeyValuePair *request,
             lkvrpc::ReturnInt *reply) override;
  Status Delete(ServerContext *context, const lkvrpc::Key *request,
                lkvrpc::ReturnInt *reply) override;
  void RunServer(const std::string &cur_loc);

private:
  LuoKV *lkv_;
};

class LKVClientImpl
{
public:
  LKVClientImpl(string peer) 
  : stub_(lkvrpc::LKV::NewStub(grpc::CreateChannel(peer, 
          grpc::InsecureChannelCredentials()))) {}
  ~LKVClientImpl() {}
  int Get(const Base::BaseType &key, 
		    Base::BaseType *value);
  int Put(const Base::BaseType &key, 
		    const Base::BaseType &value);
  int Delete(const Base::BaseType &key);
  
private:
  std::unique_ptr<lkvrpc::LKV::Stub> stub_;
};
}
}