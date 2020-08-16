#ifndef SIMPLE_CONSENSUS_IMPL_H
#define SIMPLE_CONSENSUS_IMPL_H
#include "consensus.h"
#include "conf.h"

namespace lkv 
{
namespace Consensus
{
    class SimpleConsensusImpl : public IConsensus {
    public:
        SimpleConsensusImpl(Conf::IConf *conf) : conf_(conf) 
        { conf->LoadConf(); }
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
