#ifndef SIMPLE_PAXOS_H
#define SIMPLE_PAXOS_H

#include "simple_paxos.pb.h"
#include "simple_paxos.grpc.pb.h"
#include "consensus.h"
#include "conf.h"
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
using namespace Rpc;
namespace Consensus
{
struct Acceptor
{
int64_t proposaln = 0;
int64_t acceptn = 0;
std::string acceptvalue;
bool is_confirmed = false;
};

enum ProposerState
{
    INIT,
    PREPARE,
    ACCEPT,
    CONFIRM
};

struct Proposer
{
std::set<std::string> pset;
std::set<std::string> aset;
int64_t proposaln = 0;
int64_t maxacceptn = 0;
std::string maxacceptv;
std::string proposev;
ProposerState state = ProposerState::INIT;
};

class SimplePaxos : public IConsensus, public simplepaxos::SimplePaxos::Service
{
public:
    SimplePaxos(Conf::IConf *conf) : conf_(conf) 
    { conf->LoadConf(); }
    ~SimplePaxos() {}
    virtual int Propose(const std::string &consensus_group,
                        const ConsensusType &value,
                        std::function<int (bool, const std::string&, const ConsensusType &)> cb) override;
    virtual std::string &ChooseReadProvider(const std::string &consensus_group) override;
    virtual std::string &ChooseModifyProvider(const std::string &consensus_group) override;

    // rpc impl
    virtual Status HandleAccept(ServerContext* context, 
    const simplepaxos::Accept* request, simplepaxos::AcceptRet* response);
    virtual Status HandlePrepare(ServerContext* context, 
    const simplepaxos::Prepare* request, simplepaxos::PrepareRet* response);
private:
    Status SendPrepare_(std::string peer, int64_t proposaln);
    Status SendAccept_(std::string peer, int64_t acceptn, std::string acceptv);
    void Drive_();
private:
    Conf::IConf *conf_;
    std::map<std::string, std::unique_ptr<simplepaxos::SimplePaxos::Stub>> stub_pool_;
    Acceptor acceptor_;
    Proposer proposer_;
    std::vector<std::string> peerset_;
};

}
}

#endif