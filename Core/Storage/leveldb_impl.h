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
    virtual int Get(const std::string &consensus_group,
        const Base::BaseType &key, 
		    Base::BaseType *value) override;
    virtual int Put(const std::string &consensus_group,
        const Base::BaseType &key, 
		    const Base::BaseType &value) override;
    virtual int Delete(const std::string &consensus_group,
        const Base::BaseType &key) override;
    virtual int Apply(const std::string &consensus_group,
                      const Consensus::ConsensusType &value) override;
  private:
    leveldb::DB* db_;
  };
}
}

#endif

