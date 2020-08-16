//
// Created by 罗一鑫 on 2019/12/14.
//

#ifndef LUOKV_STORAGE_ENGINE_H
#define LUOKV_STORAGE_ENGINE_H

#include "base.h"
#include "consensus.h"

namespace lkv 
{
namespace StorageEngine
{

class IStorageEngine
{
 public:
  virtual int Get(const std::string &consensus_group,
                  const Base::BaseType &key, 
                  Base::BaseType *value) = 0;
  virtual int Put(const std::string &consensus_group,
                  const Base::BaseType &key, 
                  const Base::BaseType &value) = 0;
  virtual int Delete(const std::string &consensus_group,
                     const Base::BaseType &key) = 0;
  virtual int Apply(const std::string &consensus_group,
                    const Consensus::ConsensusType &value) = 0;
};

}
}

#endif //LUOKV_STORAGE_ENGINE_H
