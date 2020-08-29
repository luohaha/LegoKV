#ifndef SIMPLE_PAXOS_H
#define SIMPLE_PAXOS_H

#include "consensus.h"
#include "conf.h"

namespace lkv 
{
namespace Consensus
{
class SimplePaxos : public IConsensus
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
private:
    Conf::IConf *conf_;
};

}
}

#endif