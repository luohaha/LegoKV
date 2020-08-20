#include "lkv.h"
#include "base.h"

using namespace lkv;
using namespace Base;
using namespace StorageEngine;
using namespace Conf;
using namespace Consensus;
using namespace Router;

int main()
{
  LOG.log_print(INFO, ERR_SUCCESS, "hello lkv");
  IStorageEngine *se = new LevelDBImpl("/tmp/asd/");
  IConf *conf = new JsonConfImpl("./Test/lkv.conf");
  IConsensus *cons = new SimpleConsensusImpl(conf);
  IRouter *router = new SimpleDHashImpl(conf);
  LuoKV lkv;
  lkv.SetConf(conf).SetStorageEngine(se).SetConsensus(cons).SetRouter(router);
  static_cast<JsonConfImpl*>(conf)->PrintConf();
  lkv.Put(BaseType("name"), BaseType("luohaha"));
  BaseType ret;
  lkv.Get(BaseType("name"), &ret);  
  printf("name : %s\n", ret.buf.c_str());
  return 0;
}