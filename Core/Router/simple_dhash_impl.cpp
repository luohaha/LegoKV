#include "simple_dhash_impl.h"

namespace lkv
{
namespace Router
{

SimpleDHashImpl::SimpleDHashImpl(Conf::IConf *conf)
{
    conf_ = conf;
    conf_->LoadConf();
    for (auto &each : conf_->GetConf().gconf.globalConsensusGroup) {
        cgs_.push_back(each.first);
    }
}

std::string &SimpleDHashImpl::RouterHash(const Base::BaseType &key)
{
    uint32_t hash = key.Hash();
    return cgs_[hash % cgs_.size()];
}
}
}