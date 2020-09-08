#ifndef CONSENSUS_H
#define CONSENSUS_H

#include "base.h"
#include <functional>
#include "lkv.pb.h"
#include "lkv.grpc.pb.h"

namespace lkv 
{
namespace Consensus
{

class IConsensus
{
 public:
  virtual int Propose(const std::string &consensus_group,
                      const lkvrpc::ConsensusType &value,
                      std::function<int (bool, const std::string &, const lkvrpc::ConsensusType &)> cb) = 0;
  virtual std::string &ChooseReadProvider(const std::string &consensus_group) = 0;
  virtual std::string &ChooseModifyProvider(const std::string &consensus_group) = 0;
};

}
}

#endif
