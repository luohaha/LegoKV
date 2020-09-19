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
  if (argc < 3) {
    printf("usage : ./test_legokv conf_file leveldb_dir\n");
    return 0;
  }
  std::string conf_file(argv[1]);
  LOG.log_print(INFO, ERR_SUCCESS, "hello lkv");
  IStorageEngine *se = new LevelDBImpl(argv[2]);
  IConf *conf = new JsonConfImpl(conf_file);
  IConsensus *cons = new SimplePaxos(conf, se);
  IRouter *router = new SimpleDHashImpl(conf);
  LuoKV lkv;
  lkv.SetConf(conf).SetStorageEngine(se).SetConsensus(cons).SetRouter(router);
  LKVServiceImpl impl(&lkv);
  impl.RunServer(lkv.get_cur_location()); 

  return 0;
}