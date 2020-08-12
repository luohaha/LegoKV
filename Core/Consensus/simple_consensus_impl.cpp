#include "simple_consensus_impl.h"

namespace Consensus 
{
int SimpleConsensusImpl::Propose(const ConsensusType &value,
                      std::function<int (bool, const ConsensusType &)> cb)
{
    return cb(true, value);
}
}

