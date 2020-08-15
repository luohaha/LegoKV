#ifndef JSON_CONF_IMPL_H
#define JSON_CONF_IMPL_H

#include "conf.h"
#include <json/json.h>

namespace lkv
{
namespace Conf
{
class JsonConfImpl : public IConf
{
public:
    JsonConfImpl(const std::string &conf_file) : conf_file_(conf_file) {}
    ~JsonConfImpl() {}
    virtual void LoadConf() override;
    virtual struct ConfType &GetConf() override;
    void PrintConf();
private:
    void GetJsonValue(std::string &jsonstr, Json::Value &root);
private:
    std::string conf_file_;
    struct ConfType cur_conf_;
};
}
}

#endif