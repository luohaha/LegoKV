#ifndef CONSENSUS_H
#define CONSENSUS_H

#include "base.h"
#include <functional>

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
    std::string buf;
    ConsensusType(const OpType o, const std::string &b) : op(o), buf(b) {}
};

class IConsensus
{
 public:
  virtual int Propose(const ConsensusType &value,
                      std::function<int (bool, const ConsensusType &)> cb) = 0;
};

}

#endif