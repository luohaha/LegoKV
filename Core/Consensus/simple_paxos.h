#ifndef SIMPLE_PAXOS_H
#define SIMPLE_PAXOS_H

#include "simple_paxos.pb.h"
#include "simple_paxos.grpc.pb.h"
#include "consensus.h"
#include "conf.h"
#include "lkv_rpc.h"
#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/grpcpp.h>
#include <dirent.h>

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
            uint64_t proposaln = 0;
            uint64_t acceptn = 0;
            lkvrpc::ConsensusType acceptvalue;
            bool confirm = false;
        };

        enum ProposerState
        {
            PREPARE,
            ACCEPT,
            CONFIRM,
            FINISH,
        };

        struct ProposVal
        {
            ProposVal(const lkvrpc::ConsensusType &v,
                      const std::function<int(bool, const std::string &, const lkvrpc::ConsensusType &)> &c,
                      Base::Cond &co) : value(v), cb(c), condition(co) {}
            ~ProposVal() {}
            const lkvrpc::ConsensusType &value;
            const std::function<int(bool, const std::string &, const lkvrpc::ConsensusType &)> &cb;
            Base::Cond &condition;
        };

        struct Proposer
        {
            std::set<std::string> pset;
            std::set<std::string> aset;
            uint64_t maxacceptn = 0;
            lkvrpc::ConsensusType maxacceptv;
            ProposerState state = ProposerState::PREPARE;
            std::unique_ptr<ProposVal> want_to_pval;
            bool propos_want_to = false;
            uint64_t next_proposn = 1;
        };

        struct PaxosStateInstance
        {
            Acceptor acceptor_;
            Proposer proposer_;
        };

        struct PaxosStateMachine
        {
            PaxosStateMachine() : propose_q_(100) {}
            PaxosStateMachine(const std::vector<std::string> &peerset) : propose_q_(100), next_instance_id_(1)
            {
                peerset_.assign(peerset.begin(), peerset.end());
            }
            void SetPeerSet(const std::vector<std::string> &peerset)
            {
                peerset_.assign(peerset.begin(), peerset.end());
            }
            Base::ConcurrentQueue<ProposVal> propose_q_;
            std::map<uint64_t, PaxosStateInstance> instances_;
            std::vector<std::string> peerset_;
            std::atomic<uint64_t> next_instance_id_;
        };

        class SimplePaxos : public IConsensus, public simplepaxos::SimplePaxos::Service
        {
        public:
            SimplePaxos(Conf::IConf *conf, StorageEngine::IStorageEngine *storage)
                : conf_(conf), storage_(storage)
            {
                conf->LoadConf();
                th_.emplace_back(&SimplePaxos::Drive_, this);
            }
            ~SimplePaxos() 
            {
                for (auto &each : th_) {
                    each.join();
                }
            }
            virtual int Propose(const std::string &consensus_group,
                                const lkvrpc::ConsensusType &value,
                                std::function<int(bool, const std::string &, const lkvrpc::ConsensusType &)> cb) override;
            virtual std::string &ChooseReadProvider(const std::string &consensus_group) override;
            virtual std::string &ChooseModifyProvider(const std::string &consensus_group) override;

            // rpc impl
            virtual Status HandleAccept(ServerContext *context,
                                        const simplepaxos::Accept *request, simplepaxos::AcceptRet *response) override;
            virtual Status HandlePrepare(ServerContext *context,
                                         const simplepaxos::Prepare *request, simplepaxos::PrepareRet *response) override;

        private:
            static const uint64_t MAGIC = 0xDEDEABAB;
            Status SendPrepare_(const std::string &peer,
                                const std::string &cg,
                                uint64_t instanceid,
                                uint64_t proposaln);
            Status SendAccept_(const std::string &peer,
                               const std::string &cg,
                               uint64_t instanceid,
                               uint64_t acceptn,
                               lkvrpc::ConsensusType &acceptv);
            void Drive_();
            const std::vector<std::string> &GetPeerSet_(const std::string &cg)
            {
                return conf_->GetConf().gconf.globalConsensusGroup[cg];
            }
            void WriteRecord_(const std::string &cg,
                              const uint64_t instanceid,
                              Acceptor &acceptor)
            {
                if (pagecontainer_map_.find(cg) == pagecontainer_map_.end())
                {
                    pagecontainer_map_[cg] = Base::make_unique<Base::PageContainer>(conf_->GetConf().workdir + "/" + cg + "/");
                }
                simplepaxos::PaxosRecord record;
                record.set_magic(MAGIC);
                record.set_instanceid(instanceid);
                record.set_proposaln(acceptor.proposaln);
                record.set_acceptn(acceptor.acceptn);
                record.set_allocated_accepterv(&acceptor.acceptvalue);
                record.set_confirm(acceptor.confirm);
                std::string tmp = record.SerializeAsString();
                pagecontainer_map_[cg].get()->writepage(instanceid, tmp.c_str(), tmp.length());
            }
            bool ReadRecord_(const std::string &cg,
                              const uint64_t instanceid,
                              Acceptor &acceptor)
            {
                if (pagecontainer_map_.find(cg) == pagecontainer_map_.end())
                {
                    pagecontainer_map_[cg] = Base::make_unique<Base::PageContainer>(conf_->GetConf().workdir + "/" + cg + "/");
                }
                simplepaxos::PaxosRecord record;
                std::string tmp(4096, '0');
                pagecontainer_map_[cg].get()->readpage(instanceid, &tmp[0], tmp.length());
                if (record.ParsePartialFromString(tmp) && record.magic() == MAGIC) {
                    acceptor.acceptn = record.acceptn();
                    acceptor.proposaln = record.proposaln();
                    acceptor.acceptvalue = record.accepterv();
                    acceptor.confirm = record.confirm();
                    return true;
                } else {
                    return false;
                }
            }

        private:
            Conf::IConf *conf_;
            std::map<std::string, std::unique_ptr<simplepaxos::SimplePaxos::Stub>> stub_pool_;
            std::map<std::string, PaxosStateMachine> psm_map_;
            StorageEngine::IStorageEngine *storage_;
            std::map<std::string, std::unique_ptr<Base::PageContainer>> pagecontainer_map_;
            std::vector<std::thread> th_;
            Base::Cond cond_;
        };

    } // namespace Consensus
} // namespace lkv

#endif