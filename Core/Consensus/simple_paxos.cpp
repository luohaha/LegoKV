#include "simple_paxos.h"
#include <algorithm>

namespace lkv 
{
namespace Consensus
{

int SimplePaxos::Propose(const std::string &consensus_group,
                      const ConsensusType &value,
                      std::function<int (bool, const std::string &, const ConsensusType &)> cb)
{
    
    return cb(true, consensus_group, value);
}

std::string &SimplePaxos::ChooseReadProvider(const std::string &consensus_group)
{
    return conf_->GetConf().gconf.globalConsensusGroup[consensus_group].at(0);
}

std::string &SimplePaxos::ChooseModifyProvider(const std::string &consensus_group)
{
    return conf_->GetConf().gconf.globalConsensusGroup[consensus_group].at(0);
}

Status SimplePaxos::HandleAccept(ServerContext* context, 
                                const simplepaxos::Accept* request, 
                                simplepaxos::AcceptRet* response)
{
    if (request->acceptern() >= acceptor_.proposaln) {
        acceptor_.proposaln = request->acceptern();
        acceptor_.acceptvalue = request->acceptern();
        acceptor_.acceptvalue = request->accepterv();
        response->set_proposaln(request->acceptern());
        response->set_isok(true);
        return Status::OK;
    } else {
        response->set_isok(false);
        return Status::OK;
    }
}
Status SimplePaxos::HandlePrepare(ServerContext* context, 
                                  const simplepaxos::Prepare* request, 
                                  simplepaxos::PrepareRet* response)
{
    if (request->proposaln() > acceptor_.proposaln) {
        acceptor_.proposaln = request->proposaln();
        response->set_proposaln(acceptor_.proposaln);
        response->set_acceptern(acceptor_.acceptn);
        response->set_accepterv(acceptor_.acceptvalue);
        response->set_isok(true);
        return Status::OK;
    } else {
        response->set_isok(false);
        return Status::OK;
    }
}

Status SimplePaxos::SendPrepare_(std::string peer, int64_t proposaln)
{
    if (stub_pool_.find(peer) == stub_pool_.end()) {
        stub_pool_[peer](simplepaxos::SimplePaxos::NewStub(grpc::CreateChannel(peer, 
          grpc::InsecureChannelCredentials())));
    }
    ClientContext context;
    simplepaxos::Prepare p;
    p.set_proposaln(proposaln);
    simplepaxos::PrepareRet ret;
    Status s = stub_pool_[peer]->HandlePrepare(&context, p, &ret);
    if (s.ok() && ret.isok()) {
        proposer_.pset.insert(peer);
        proposer_.proposaln = std::max(ret.proposaln(), proposer_.proposaln);
        if (ret.acceptern() > proposer_.maxacceptn) {
            proposer_.maxacceptv = ret.accepterv();
        }
        if (proposer_.pset.size() > peerset_.size() / 2) {
            proposer_.state = ProposerState::ACCEPT;
        }
    }
    return s;
}
    
Status SimplePaxos::SendAccept_(std::string peer, int64_t acceptn, std::string acceptv)
{
    if (stub_pool_.find(peer) == stub_pool_.end()) {
        stub_pool_[peer](simplepaxos::SimplePaxos::NewStub(grpc::CreateChannel(peer, 
          grpc::InsecureChannelCredentials())));
    }
    ClientContext context;
    simplepaxos::Accept a;
    a.set_acceptern(acceptn);
    a.set_accepterv(acceptv);
    simplepaxos::AcceptRet ret;
    Status s = stub_pool_[peer]->HandleAccept(&context, a, &ret);
    if (s.ok() && ret.isok()) {
        proposer_.aset.insert(peer);
        if (proposer_.aset.size() > peerset_.size() / 2) {
            proposer_.state = ProposerState::CONFIRM;
        }
    }
    return s;
}

void SimplePaxos::Drive_()
{
    do {
        if (proposer_.state == ProposerState::PREPARE) {
            for (auto &peer : peerset_) {
                SendPrepare_(peer, 0);
            }
        } else if (proposer_.state == ProposerState::ACCEPT) {
            for (auto &peer : peerset_) {
                SendAccept_(peer, proposer_.maxacceptn, proposer_.maxacceptv);
            }
        } else if (proposer_.state == ProposerState::CONFIRM) {

        } else {}
    } while (true);
}

}
}