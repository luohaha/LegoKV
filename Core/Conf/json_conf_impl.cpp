#include "json_conf_impl.h"
#include <fstream>
#include <json/json.h>

namespace lkv
{
namespace Conf
{
void JsonConfImpl::LoadConf()
{
    std::ifstream is(conf_file_, std::ifstream::binary);
    Json::Value root;
    is >> root;
    cur_conf_.location = root.get("location", "NULL").asString();
    cur_conf_.curConsensusGroup = root.get("curConsensusGroup", "NULL").asString();
    Json::Value gconf = root["gconf"];
    for (int i = 0; i < gconf.size(); ++i) {
        Json::Value each = gconf[i];
        Json::Value::Members members = each.getMemberNames();
        for (auto &m : members) {
            Json::Value locations = each[m];
            cur_conf_.gconf.globalConsensusGroup
            .insert(std::pair<std::string, std::vector<std::string>>(m, locations.getMemberNames()));
        }
    }
}

struct ConfType &JsonConfImpl::GetConf()
{
    return cur_conf_;
}

}
}