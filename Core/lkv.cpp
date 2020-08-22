//
// Created by 罗一鑫 on 2019/12/14.
//

#include "lkv.h"
#include "base.h"
#include "leveldb_impl.h"
#include "lkv_rpc.h"

namespace lkv 
{

using namespace Base;
using namespace StorageEngine;
using namespace Consensus;
using namespace Router;
using namespace Rpc;

int LuoKV::Get(const Base::BaseType &key,
               Base::BaseType *value)
{
  std::string &cg = router_->RouterHash(key);
  std::string &loc = cons_->ChooseReadProvider(cg);
  if (loc == cur_loc_) {
    ConsensusType v(OP_GET, key.buf);
    return cons_->Propose(cg, v,
    [&](bool a, const std::string &cg, const ConsensusType &b) 
    -> int { return se_->Get(cg, key, value); });
  } else {
    LKVClientImpl impl(loc);
    return impl.Get(key, value);
  }
}

int LuoKV::Put(const Base::BaseType &key,
               const Base::BaseType &value)
{
  std::string &cg = router_->RouterHash(key);
  std::string &loc = cons_->ChooseModifyProvider(cg);
  if (loc == cur_loc_) {
    ConsensusType v(OP_PUT, key.buf, value.buf);
    return cons_->Propose(router_->RouterHash(key), v,
    [&](bool a, const std::string &cg, const ConsensusType &b) 
    -> int { return se_->Apply(cg, b); });
  } else {
    LKVClientImpl impl(loc);
    return impl.Put(key, value);
  }
}

int LuoKV::Delete(const Base::BaseType &key)
{
  std::string &cg = router_->RouterHash(key);
  std::string &loc = cons_->ChooseModifyProvider(cg);
  if (loc == cur_loc_) {
    ConsensusType v(OP_DEL, key.buf);
    return cons_->Propose(router_->RouterHash(key), v,
    [&](bool a, const std::string &cg, const ConsensusType &b) 
    -> int { return se_->Apply(cg, b); });
  } else {
    LKVClientImpl impl(loc);
    return impl.Delete(key);
  }
}

}
