//
// Created by 罗一鑫 on 2019/12/14.
//

#include "lkv.h"
#include "base.h"
#include "leveldb_impl.h"

using namespace Base;
using namespace StorageEngine;

int LuoKV::Get(const Base::BaseType &key,
               Base::BaseType *value)
{
  return se_->Get(key, value);
}

int LuoKV::Put(const Base::BaseType &key,
               const Base::BaseType &value)
{
  return se_->Put(key, value);
}

int LuoKV::Delete(const Base::BaseType &key)
{
  return se_->Delete(key);
}
