#include "lkv.h"
#include "base.h"

using namespace Base;
using namespace StorageEngine;

int main()
{
  LOG.log_print(INFO, ERR_SUCCESS, "hello lkv");
  StorageEngine::IStorageEngine *se = new LevelDBImpl("/tmp/asd/");
  se->Put(BaseType("name"), BaseType("luohaha"));
  BaseType ret;
  se->Get(BaseType("name"), &ret);
  printf("ret : %s\n", ret.buf.c_str());
  return 0;
}