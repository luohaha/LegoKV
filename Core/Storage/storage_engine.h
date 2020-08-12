//
// Created by 罗一鑫 on 2019/12/14.
//

#ifndef LUOKV_STORAGE_ENGINE_H
#define LUOKV_STORAGE_ENGINE_H

#include "base.h"

namespace StorageEngine
{

class IStorageEngine
{
 public:
  virtual int Get(const Base::BaseType &key, Base::BaseType *value) = 0;
  virtual int Put(const Base::BaseType &key, const Base::BaseType &value) = 0;
  virtual int Delete(const Base::BaseType &key) = 0;
};

}

#endif //LUOKV_STORAGE_ENGINE_H
