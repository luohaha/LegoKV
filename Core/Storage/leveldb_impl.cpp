//
// Created by luohaha on 2019/12/14.
//

#include "leveldb_impl.h"

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

  int LevelDBImpl::Get(const BaseType &key,
		       BaseType *value)
  {
    Slice skey(key.buf);
    Status s = db_->Get(leveldb::ReadOptions(), skey, &value->buf);
    return s.ok() ? ERR_SUCCESS : ERR_NOT_FOUND;
  }

  int LevelDBImpl::Put(const BaseType &key, 
		       const BaseType &value) 
  {
    Slice skey(key.buf);
    Slice svalue(value.buf);
    Status s = db_->Put(leveldb::WriteOptions(), skey, svalue);
    return s.ok() ? ERR_SUCCESS : ERR_UNEXPECTED;
  }

  int LevelDBImpl::Delete(const Base::BaseType &key)
  {
    Slice skey(key.buf);
    Status s = db_->Delete(leveldb::WriteOptions(), skey);
    return s.ok() ? ERR_SUCCESS : ERR_UNEXPECTED;
  }

}
