#include "simple_paxos.h"
#include <algorithm>

namespace lkv
{
    namespace Consensus
    {

        int SimplePaxos::Propose(const std::string &consensus_group,
                                 const lkvrpc::ConsensusType &value,
                                 std::function<int(bool, const std::string &, const lkvrpc::ConsensusType &)> cb)
        {
	  //if (psm_map_.find(consensus_group) == psm_map_.end())
	  //    return ERR_CONSENSUS_NO_EXIST;
            Base::Cond condition;
	    LOG_INFO("Propose cg %s Push To Queue", consensus_group.c_str());
            psm_map_[consensus_group].propose_q_.Push(std::unique_ptr<ProposVal>(new ProposVal(value, cb, condition)));
            LOG_INFO("Propose cg %s Push To Queue Success", consensus_group.c_str());
	    condition.Wait();
	    LOG_INFO("Propose cg %s End", consensus_group.c_str());
            return ERR_SUCCESS;
        }

        std::string &SimplePaxos::ChooseReadProvider(const std::string &consensus_group)
        {
            return conf_->GetConf().gconf.globalConsensusGroup[consensus_group].at(0);
        }

        std::string &SimplePaxos::ChooseModifyProvider(const std::string &consensus_group)
        {
            return conf_->GetConf().gconf.globalConsensusGroup[consensus_group].at(0);
        }

        Status SimplePaxos::HandleAccept(ServerContext *context,
                                         const simplepaxos::Accept *request,
                                         simplepaxos::AcceptRet *response)
        {
            if (psm_map_.find(request->consenus_group()) == psm_map_.end())
            {
                psm_map_[request->consenus_group()].SetPeerSet(GetPeerSet_(request->consenus_group()));
            }
            PaxosStateMachine &psm = psm_map_[request->consenus_group()];
            Acceptor &acceptor = psm.instances_[request->instanceid()].acceptor_;
            ReadRecord_(request->consenus_group(), request->instanceid(), acceptor);
            if (request->acceptern() >= acceptor.proposaln)
            {
                acceptor.proposaln = request->acceptern();
                acceptor.acceptn = request->acceptern();
                acceptor.acceptvalue = request->accepterv();
                WriteRecord_(request->consenus_group(), request->instanceid(), acceptor);
                response->set_proposaln(request->acceptern());
                response->set_consenus_group(request->consenus_group());
                response->set_instanceid(request->instanceid());
                response->set_isok(true);
                return Status::OK;
            }
            else
            {
                response->set_isok(false);
                return Status::OK;
            }
        }
        Status SimplePaxos::HandlePrepare(ServerContext *context,
                                          const simplepaxos::Prepare *request,
                                          simplepaxos::PrepareRet *response)
        {
            if (psm_map_.find(request->consenus_group()) == psm_map_.end())
            {
                psm_map_[request->consenus_group()].SetPeerSet(GetPeerSet_(request->consenus_group()));
            }
            PaxosStateMachine &psm = psm_map_[request->consenus_group()];
            Acceptor &acceptor = psm.instances_[request->instanceid()].acceptor_;
            ReadRecord_(request->consenus_group(), request->instanceid(), acceptor);
            if (request->proposaln() > acceptor.proposaln)
            {
                acceptor.proposaln = request->proposaln();
                WriteRecord_(request->consenus_group(), request->instanceid(), acceptor);
                response->set_proposaln(acceptor.proposaln);
                response->set_acceptern(acceptor.acceptn);
                response->set_allocated_accepterv(&acceptor.acceptvalue);
                response->set_consenus_group(request->consenus_group());
                response->set_instanceid(request->instanceid());
                response->set_isok(true);
                return Status::OK;
            }
            else
            {
                response->set_proposaln(acceptor.proposaln);
                response->set_isok(false);
                return Status::OK;
            }
        }

        Status SimplePaxos::SendPrepare_(const std::string &peer,
                                         const std::string &cg,
                                         uint64_t instanceid,
                                         uint64_t proposaln)
        {
            if (stub_pool_.find(peer) == stub_pool_.end())
            {
                stub_pool_[peer] = simplepaxos::SimplePaxos::NewStub(grpc::CreateChannel(peer,
                                                                                         grpc::InsecureChannelCredentials()));
            }
            if (psm_map_.find(cg) == psm_map_.end())
            {
                psm_map_[cg].SetPeerSet(GetPeerSet_(cg));
            }
            PaxosStateMachine &psm = psm_map_[cg];
            ClientContext context;
            simplepaxos::Prepare p;
            p.set_consenus_group(cg);
            p.set_proposaln(proposaln);
            p.set_instanceid(instanceid);
            simplepaxos::PrepareRet ret;
            Proposer &proposer = psm.instances_[instanceid].proposer_;
            Status s = stub_pool_[peer]->HandlePrepare(&context, p, &ret);
            if (s.ok() && ret.isok())
            {
                if (ret.isok())
                {
                    proposer.pset.insert(peer);
                    if (ret.acceptern() > proposer.maxacceptn)
                    {
                        proposer.maxacceptv = ret.accepterv();
                        proposer.maxacceptn = ret.acceptern();
                    }
                    if (proposer.pset.size() > psm.peerset_.size() / 2)
                    {
                        proposer.state = ProposerState::ACCEPT;
                    }
                }
                else
                {
                    proposer.next_proposn = std::max(proposer.next_proposn + 1, ret.proposaln());
                }
            }
            return s;
        }

        Status SimplePaxos::SendAccept_(const std::string &peer,
                                        const std::string &cg,
                                        uint64_t instanceid,
                                        uint64_t acceptn,
                                        lkvrpc::ConsensusType &acceptv)
        {
            if (stub_pool_.find(peer) == stub_pool_.end())
            {
                stub_pool_[peer] = simplepaxos::SimplePaxos::NewStub(grpc::CreateChannel(peer,
                                                                                         grpc::InsecureChannelCredentials()));
            }
            if (psm_map_.find(cg) == psm_map_.end())
            {
                psm_map_[cg].SetPeerSet(GetPeerSet_(cg));
            }

            ClientContext context;
            simplepaxos::Accept a;
            a.set_consenus_group(cg);
            a.set_instanceid(instanceid);
            a.set_acceptern(acceptn);
            a.set_allocated_accepterv(&acceptv);
            PaxosStateMachine &psm = psm_map_[cg];
            Proposer &proposer = psm.instances_[instanceid].proposer_;
            Acceptor &acceptor = psm.instances_[instanceid].acceptor_;
            simplepaxos::AcceptRet ret;
            Status s = stub_pool_[peer]->HandleAccept(&context, a, &ret);
            if (s.ok() && ret.isok())
            {
                proposer.aset.insert(peer);
                if (proposer.aset.size() > psm.peerset_.size() / 2 
                && proposer.aset.find(conf_->GetConf().location) != proposer.aset.end())
                {
                    proposer.state = ProposerState::CONFIRM;
                    acceptor.confirm = true;
                    WriteRecord_(cg, instanceid, acceptor);
                }
            }
            return s;
        }

        void SimplePaxos::Drive_()
        {
            do
            {
                for (auto &cg : psm_map_)
                {
                    // for each paxos state machine
                    PaxosStateMachine &psm = cg.second;
                    for (auto &instance : psm.instances_)
                    {
                        // for each instance
                        Proposer &proposer = instance.second.proposer_;
                        if (proposer.state == ProposerState::PREPARE)
                        {
                            for (auto &peer : psm.peerset_)
                            {
                                SendPrepare_(peer, cg.first, instance.first, proposer.next_proposn);
                            }
                        }
                        else if (proposer.state == ProposerState::ACCEPT)
                        {
                            for (auto &peer : psm.peerset_)
                            {
                                if (proposer.maxacceptv.key().empty())
                                {
                                    proposer.maxacceptv = proposer.want_to_pval.get()->value;
                                    proposer.propos_want_to = true;
                                } else {
                                    proposer.propos_want_to = false;
                                }
                                SendAccept_(peer, cg.first, instance.first,
                                            proposer.maxacceptn, proposer.maxacceptv);
                            }
                        }
                        else if (proposer.state == ProposerState::CONFIRM)
                        {
                            // cb
                            if (proposer.propos_want_to) {
                                proposer.want_to_pval.get()->cb(true, cg.first, proposer.maxacceptv);
                            } else {
                                storage_->Apply(cg.first, proposer.maxacceptv);
                            }
                            proposer.want_to_pval.get()->condition.Notify();
                            proposer.state = ProposerState::FINISH;
                        }
                        else
                        {
                        }
                    }
                    auto next = psm.propose_q_.TryPop();
                    if (next.get() != NULL) {
                        uint64_t iid = psm.next_instance_id_.fetch_add(1, std::memory_order_release);
                        psm.instances_.emplace(iid, PaxosStateInstance());
                        psm.instances_[iid].proposer_.want_to_pval = std::move(next);
                    }
                }
            } while (true);
        }

    } // namespace Consensus
} // namespace lkv
