#include "simple_paxos.h"

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

}
}