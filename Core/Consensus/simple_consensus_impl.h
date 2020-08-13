#ifndef SIMPLE_CONSENSUS_IMPL_H
#define SIMPLE_CONSENSUS_IMPL_H
#include "Consensus.h"

namespace lkv 
{
namespace Consensus
{
    class SimpleConsensusImpl : public IConsensus {
        virtual int Propose(const ConsensusType &value,
                      std::function<int (bool, const ConsensusType &)> cb) override;
    };
}
}

#endif
