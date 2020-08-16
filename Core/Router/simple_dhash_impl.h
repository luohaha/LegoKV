#ifndef SIMPLE_DHASH_IMPL_H
#define SIMPLE_DHASH_IMPL_H

#include "router.h"
#include "conf.h"

namespace lkv
{
namespace Router
{
class SimpleDHashImpl : public IRouter
{
public:
    SimpleDHashImpl(Conf::IConf *conf);
    ~SimpleDHashImpl() {}
    virtual std::string &RouterHash(const Base::BaseType &key);
private:
    Conf::IConf *conf_;
    std::vector<std::string> cgs_;
};

}
}

#endif