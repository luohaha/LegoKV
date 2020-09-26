#include "lkv_rpc.h"

namespace lkv
{
using namespace Base;
namespace Rpc
{
Status LKVServiceImpl::Get(ServerContext *context, const lkvrpc::Key *request,
             lkvrpc::Value *reply)
{
    BaseType resp;
    LOG_INFO("Server : Get key %s", request->buf().c_str());
    lkv_->Get(BaseType(request->buf()), &resp);
    reply->set_buf(resp.buf);
    return Status::OK;
}
Status LKVServiceImpl::Put(ServerContext *context, const lkvrpc::KeyValuePair *request,
             lkvrpc::ReturnInt *reply)
{
    LOG_INFO("Server : Put key %s", request->buf().c_str());
    reply->set_ret(lkv_->Put(BaseType(request->key().buf()), BaseType(request->value().buf())));
    return Status::OK;
}
Status LKVServiceImpl::Delete(ServerContext *context, const lkvrpc::Key *request,
                lkvrpc::ReturnInt *reply)
{
    LOG_INFO("Server : Del key %s", request->buf().c_str());
    reply->set_ret(lkv_->Delete(BaseType(request->buf())));
    return Status::OK;
}
void LKVServiceImpl::RunServer(const std::string &cur_loc)
{
    std::string server_address(cur_loc);

    //grpc::EnableDefaultHealthCheckService(true);
    //grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(this);
    // Finally assemble the server.
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}

int LKVClientImpl::Get(const Base::BaseType &key, 
		                Base::BaseType *value)
{
    lkvrpc::Key ikey;
    ikey.set_buf(key.buf);
    lkvrpc::Value ivalue;
    ClientContext context;
    Status status = stub_->Get(&context, ikey, &ivalue);
    value->buf = ivalue.buf();
    return status.ok() ? 0 : 1;
}

int LKVClientImpl::Put(const Base::BaseType &key, 
		                const Base::BaseType &value)
{
    lkvrpc::KeyValuePair pair;
    lkvrpc::Key *ikey = new lkvrpc::Key();
    ikey->set_buf(key.buf);
    lkvrpc::Value *ivalue = new lkvrpc::Value();
    ivalue->set_buf(value.buf);
    pair.set_allocated_key(ikey);
    pair.set_allocated_value(ivalue);
    lkvrpc::ReturnInt ret;
    ClientContext context;
    Status status = stub_->Put(&context, pair, &ret);
    if (!status.ok()) return 1;
    return ret.ret();
}
  
int LKVClientImpl::Delete(const Base::BaseType &key)
{
    lkvrpc::Key ikey;
    ikey.set_buf(key.buf);
    lkvrpc::ReturnInt ret;
    ClientContext context;
    Status status = stub_->Delete(&context, ikey, &ret);
    if (!status.ok()) return 1;
    return ret.ret();
}

}
}
