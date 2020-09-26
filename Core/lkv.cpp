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
  int ret = 0;
  std::string &cg = router_->RouterHash(key);
  std::string &loc = cons_->ChooseReadProvider(cg);
  LOG_INFO("Get cg %s location %s key %s", cg.c_str(), loc.c_str(), key.buf.c_str());
  if (loc == cur_loc_) {
    lkvrpc::ConsensusType v;
    v.set_op(lkvrpc::ConsensusType_OpType::ConsensusType_OpType_OP_GET);
    v.set_key(key.buf);
    ret = cons_->Propose(cg, v,
    [&](bool a, const std::string &cg, const lkvrpc::ConsensusType &b) 
    -> int { return se_->Get(cg, key, value); });
    LOG.log_print(INFO, ret, "Get local");
  } else {
    LKVClientImpl impl(loc);
    ret = impl.Get(key, value);
    LOG.log_print(INFO, ret, "Get remote");
  }
  return ret;
}

int LuoKV::Put(const Base::BaseType &key,
               const Base::BaseType &value)
{
  int ret = 0;
  std::string &cg = router_->RouterHash(key);
  std::string &loc = cons_->ChooseModifyProvider(cg);
  LOG_INFO("Put cg %s location %s key %s", cg.c_str(), loc.c_str(), key.buf.c_str());
  if (loc == cur_loc_) {
    lkvrpc::ConsensusType v;
    v.set_op(lkvrpc::ConsensusType_OpType::ConsensusType_OpType_OP_PUT);
    v.set_key(key.buf);
    v.set_value(value.buf);
    ret = cons_->Propose(router_->RouterHash(key), v,
    [&](bool a, const std::string &cg, const lkvrpc::ConsensusType &b) 
    -> int { return se_->Apply(cg, b); });
    LOG.log_print(INFO, ret, "Put local");
  } else {
    LKVClientImpl impl(loc);
    ret = impl.Put(key, value);
    LOG.log_print(INFO, ret, "Put remote");
  }
  return ret;
}

int LuoKV::Delete(const Base::BaseType &key)
{
  int ret = 0;
  std::string &cg = router_->RouterHash(key);
  std::string &loc = cons_->ChooseModifyProvider(cg);
    LOG_INFO("Del cg %s location %s key %s", cg.c_str(), loc.c_str(), key.buf.c_str());
  if (loc == cur_loc_) {
    lkvrpc::ConsensusType v;
    v.set_op(lkvrpc::ConsensusType_OpType::ConsensusType_OpType_OP_DEL);
    v.set_key(key.buf);
    ret = cons_->Propose(router_->RouterHash(key), v,
    [&](bool a, const std::string &cg, const lkvrpc::ConsensusType &b) 
    -> int { return se_->Apply(cg, b); });
    LOG.log_print(INFO, ret, "Delete local");
  } else {
    LKVClientImpl impl(loc);
    ret = impl.Delete(key);
    LOG.log_print(INFO, ret, "Delete remote");
  }
  return ret;
}

}
