#include "lkv.h"
#include "base.h"
#include "lkv_rpc.h"

using namespace lkv;
using namespace Base;
using namespace StorageEngine;
using namespace Conf;
using namespace Consensus;
using namespace Router;
using namespace Rpc;

int main()
{
    LKVClientImpl impl("0.0.0.0:8888");
    for (int i = 0; i < 1000; i++) {
        impl.Put(std::to_string(i), std::to_string(i));
    }
    BaseType ret;
    for (int i = 0; i < 1000; i++) {
        impl.Get(std::to_string(i), &ret);
        assert(std::to_string(i) == ret.buf);
    }
    return 0;
}