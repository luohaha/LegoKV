//
// Created by 罗一鑫 on 2019/12/14.
//

#include "lkv.h"
#include "base.h"
#include "leveldb_impl.h"

namespace lkv 
{

using namespace Base;
using namespace StorageEngine;
using namespace Consensus;
using namespace Router;

int LuoKV::Get(const Base::BaseType &key,
               Base::BaseType *value)
{
  ConsensusType v(OP_GET, key.buf);
  return cons_->Propose(router_->RouterHash(key), v,
  [&](bool a, const std::string &cg, const ConsensusType &b) 
  -> int { return se_->Get(cg, key, value); });
}

int LuoKV::Put(const Base::BaseType &key,
               const Base::BaseType &value)
{
  ConsensusType v(OP_PUT, key.buf, value.buf);
  return cons_->Propose(router_->RouterHash(key), v,
  [&](bool a, const std::string &cg, const ConsensusType &b) 
  -> int { return se_->Apply(cg, b); });
}

int LuoKV::Delete(const Base::BaseType &key)
{
  ConsensusType v(OP_DEL, key.buf);
  return cons_->Propose(router_->RouterHash(key), v,
  [&](bool a, const std::string &cg, const ConsensusType &b) 
  -> int { return se_->Apply(cg, b); });
}

}
