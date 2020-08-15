#include "lkv.h"
#include "base.h"

using namespace lkv;
using namespace Base;
using namespace StorageEngine;
using namespace Conf;
using namespace Consensus;

int main()
{
  LOG.log_print(INFO, ERR_SUCCESS, "hello lkv");
  IStorageEngine *se = new LevelDBImpl("/tmp/asd/");
  IConsensus *cons = new SimpleConsensusImpl();
  IConf *conf = new JsonConfImpl("/tmp/lkv.conf");
  LuoKV lkv;
  lkv.SetConf(conf).SetStorageEngine(se).SetConsensus(cons);
  static_cast<JsonConfImpl*>(conf)->PrintConf();
  lkv.Put(BaseType("name"), BaseType("luohaha"));
  BaseType ret;
  lkv.Get(BaseType("name"), &ret);
  printf("name : %s\n", ret.buf.c_str());
  return 0;
}