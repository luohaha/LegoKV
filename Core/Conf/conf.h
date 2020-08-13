#include "base.h"

namespace lkv
{
namespace Conf
{

struct GlobalConf
{
    // {consensus group -> location}
    std::vector<std::vector<string>> globalConsensusGroup;
};

struct ConfType
{
    std::string location;
    int32_t curConsensusGroup;
    struct GlobalConf gconf;
};

class IConf {
public:
    virtual void LoadConf() = 0;
    virtual struct ConfType &GetConf() = 0;
};

}
}