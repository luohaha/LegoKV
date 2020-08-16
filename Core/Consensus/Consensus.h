#ifndef CONSENSUS_H
#define CONSENSUS_H

#include "base.h"
#include <functional>

namespace lkv 
{
namespace Consensus
{

enum OpType
{
    OP_GET,
    OP_PUT,
    OP_DEL,
};

struct ConsensusType
{
    OpType op;
    std::string key;
    std::string value;
    ConsensusType(const OpType o, const std::string &k) : op(o), key(k) {}
    ConsensusType(const OpType o, const std::string &k, const std::string &v) 
    : op(o), key(k), value(v) {}
};

class IConsensus
{
 public:
  virtual int Propose(const std::string &consensus_group,
                      const ConsensusType &value,
                      std::function<int (bool, const std::string &, const ConsensusType &)> cb) = 0;
  virtual std::string &ChooseReadProvider(const std::string &consensus_group) = 0;
  virtual std::string &ChooseModifyProvider(const std::string &consensus_group) = 0;
};

}
}

#endif