//
// Created by luohaha on 2019/12/14.
//

#include "leveldb_impl.h"

namespace lkv 
{
using namespace Base;
using namespace leveldb;

namespace StorageEngine
{

  void LevelDBImpl::init(const std::string &path)
  {
    DB* db;
    Options options;
    options.create_if_missing = true;
    Status s = DB::Open(options, path, &db);
    assert(s.ok());
    db_ = db;
  }

  int LevelDBImpl::Get(const std::string &consensus_group,
           const BaseType &key,
		       BaseType *value)
  {
    Slice skey(key.buf);
    Status s = db_->Get(leveldb::ReadOptions(), skey, &value->buf);
    return s.ok() ? ERR_SUCCESS : ERR_NOT_FOUND;
  }

  int LevelDBImpl::Put(const std::string &consensus_group,
           const BaseType &key, 
		       const BaseType &value) 
  {
    Slice skey(key.buf);
    Slice svalue(value.buf);
    Status s = db_->Put(leveldb::WriteOptions(), skey, svalue);
    return s.ok() ? ERR_SUCCESS : ERR_UNEXPECTED;
  }

  int LevelDBImpl::Delete(const std::string &consensus_group,
                          const Base::BaseType &key)
  {
    Slice skey(key.buf);
    Status s = db_->Delete(leveldb::WriteOptions(), skey);
    return s.ok() ? ERR_SUCCESS : ERR_UNEXPECTED;
  }

  int LevelDBImpl::Apply(const std::string &consensus_group,
                         const Consensus::ConsensusType &value)
  {
    if (value.op == Consensus::OP_PUT) {
      return Put(consensus_group, value.key, value.value);
    } else if (value.op == Consensus::OP_DEL) {
      return Delete(consensus_group, value.key);
    } else if (value.op == Consensus::OP_GET) {
      //do nothing
    }
    return ERR_UNEXPECTED;
  }

}
}
