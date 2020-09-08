#include "json_conf_impl.h"
#include <fstream>
#include <streambuf>


namespace lkv
{
using namespace Base;
namespace Conf
{
void JsonConfImpl::GetJsonValue(std::string &jsonstr, Json::Value &root)
{
    Json::CharReaderBuilder builder;
    JSONCPP_STRING err;
    const auto rawJsonLength = static_cast<int>(jsonstr.length());
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    if (!reader->parse(jsonstr.c_str(), jsonstr.c_str() + rawJsonLength, &root,
                       &err)) {
      LOG.log_print(ERROR, ERR_JSON_PARSER_ERR, "parse json error");
    }
}
void JsonConfImpl::LoadConf()
{
    std::ifstream is(conf_file_, std::ifstream::in);
    std::stringstream buffer;
    buffer << is.rdbuf();
    std::string contents(buffer.str());
    Json::Value root;
    GetJsonValue(contents, root);
    cur_conf_.location = root.get("location", "NULL").asString();
    cur_conf_.workdir = root.get("workdir", "NULL").asString();
    Json::Value &gconf = root["gconf"];
    const std::vector<std::string> &keys = gconf.getMemberNames();
    for (auto &each : keys) {
        Json::Value &locations = gconf[each];
        std::vector<std::string> values;
        for (auto &loc : locations) {
            values.push_back(loc.asString());
        }
        cur_conf_.gconf.globalConsensusGroup
        .insert(std::pair<std::string, std::vector<std::string>>(each, values));
    }
}

struct ConfType &JsonConfImpl::GetConf()
{
    return cur_conf_;
}

void JsonConfImpl::PrintConf()
{
    printf("location : %s\n workdir : %s\n", 
    cur_conf_.location.c_str(), cur_conf_.workdir.c_str());
    for (auto &each : cur_conf_.gconf.globalConsensusGroup) {
        printf("consensus group : %s : ", each.first.c_str());
        printf("[");
        for (auto &v : each.second) {
            printf("%s,", v.c_str());
        }
        printf("]\n");
    }
}

}
}
