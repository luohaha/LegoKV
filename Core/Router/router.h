#ifndef ROUTER_H
#define ROUTER_H

#include "base.h"

namespace lkv
{
namespace Router
{
class IRouter
{
public:
    virtual std::string &RouterHash(const Base::BaseType &key) = 0;
};
}
}

#endif