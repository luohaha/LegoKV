#include "lkv.h"
#include "base.h"
#include "lkv_rpc.h"
#include "Consensus/simple_paxos.h"

using namespace lkv;
using namespace Base;
using namespace StorageEngine;
using namespace Conf;
using namespace Consensus;
using namespace Router;
using namespace Rpc;

int main(int argc, char** argv)
{
  if (argc < 2) {
    printf("usage : ./test_legokv conf_file\n");
    return 0;
  }
  std::string conf_file(argv[1]);
  IConf *conf = new JsonConfImpl(conf_file);
  conf->LoadConf();
  static_cast<JsonConfImpl*>(conf)->PrintConf();
  IStorageEngine *se = new LevelDBImpl(conf->GetConf().workdir);
  IConsensus *cons = new SimplePaxos(conf, se);
  IRouter *router = new SimpleDHashImpl(conf);
  LuoKV lkv;
  lkv.SetConf(conf).SetStorageEngine(se).SetConsensus(cons).SetRouter(router);
  LKVServiceImpl impl(&lkv);
  impl.RunServer(lkv.get_cur_location()); 

  return 0;
}
