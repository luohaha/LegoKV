#include "simple_consensus_impl.h"

namespace lkv 
{
namespace Consensus 
{
int SimpleConsensusImpl::Propose(const std::string &consensus_group,
                      const lkvrpc::ConsensusType &value,
                      std::function<int (bool, const std::string &, const lkvrpc::ConsensusType &)> cb)
{
    return cb(true, consensus_group, value);
}

std::string &SimpleConsensusImpl::ChooseReadProvider(const std::string &consensus_group)
{
    return conf_->GetConf().gconf.globalConsensusGroup[consensus_group].at(0);
}

std::string &SimpleConsensusImpl::ChooseModifyProvider(const std::string &consensus_group)
{
    return conf_->GetConf().gconf.globalConsensusGroup[consensus_group].at(0);
}

}
}

