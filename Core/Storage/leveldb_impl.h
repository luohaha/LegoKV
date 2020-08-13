//
// Created by luohaha on 2019/12/14.
//

#ifndef LEVELDB_IMPL_H
#define LEVELDB_IMPL_H

#include "storage_engine.h"

namespace lkv 
{
namespace StorageEngine
{
  class LevelDBImpl : public IStorageEngine
  {
  public:
    LevelDBImpl(const std::string &path) 
    { init(path); }
    ~LevelDBImpl() { delete db_; }
    void init(const std::string &path);
    virtual int Get(const Base::BaseType &key, 
		    Base::BaseType *value) override;
    virtual int Put(const Base::BaseType &key, 
		    const Base::BaseType &value) override;
    virtual int Delete(const Base::BaseType &key) override;
  private:
    leveldb::DB* db_;
  };
}
}

#endif

