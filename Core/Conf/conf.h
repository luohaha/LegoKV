#ifndef CONF_H
#define CONF_H
#include "base.h"

namespace lkv
{
namespace Conf
{

struct GlobalConf
{
    // { consensus group name -> consensus group {addr1, addr2, ...} }
    std::map<std::string, std::vector<std::string>> globalConsensusGroup;
};

struct ConfType
{
    std::string location;
    std::string workdir;
    struct GlobalConf gconf;
};

class IConf {
public:
    virtual void LoadConf() = 0;
    virtual struct ConfType &GetConf() = 0;
};

}
}

#endif