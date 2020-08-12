//
// Created by 罗一鑫 on 2019/12/14.
//

#include "lkv.h"
#include "base.h"
#include "leveldb_impl.h"

using namespace Base;
using namespace StorageEngine;
using namespace Consensus;

int LuoKV::Get(const Base::BaseType &key,
               Base::BaseType *value)
{
  ConsensusType v(OP_GET, key.buf);
  return cons_->Propose(v, 
  [&](bool a, const ConsensusType &b) -> int { return se_->Get(key, value); });
}

int LuoKV::Put(const Base::BaseType &key,
               const Base::BaseType &value)
{
  ConsensusType v(OP_PUT, key.buf);
  return cons_->Propose(v, 
  [&](bool a, const ConsensusType &b) -> int { return se_->Put(key, value); });
}

int LuoKV::Delete(const Base::BaseType &key)
{
  ConsensusType v(OP_DEL, key.buf);
  return cons_->Propose(v, 
  [&](bool a, const ConsensusType &b) -> int { return se_->Delete(key); });
}
