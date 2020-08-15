//
// Created by 罗一鑫 on 2019/12/14.
//

#ifndef LUOKV_LKV_H
#define LUOKV_LKV_H

#include "Storage/leveldb_impl.h"
#include "Consensus/simple_consensus_impl.h"
#include "Conf/json_conf_impl.h"

namespace lkv 
{

class LuoKV
{
public:
    LuoKV() {}
    ~LuoKV() {}
    LuoKV &SetConf(Conf::IConf *conf)
    { conf_ = conf; conf->LoadConf(); return *this; }
    LuoKV &SetStorageEngine(StorageEngine::IStorageEngine *se)
    { se_ = se; return *this; }
    LuoKV &SetConsensus(Consensus::IConsensus *cons)
    { cons_ = cons; return *this; }
    int Get(const Base::BaseType &key, 
		    Base::BaseType *value);
    int Put(const Base::BaseType &key, 
		    const Base::BaseType &value);
    int Delete(const Base::BaseType &key);
private:
    StorageEngine::IStorageEngine *se_;
    Consensus::IConsensus *cons_;
    Conf::IConf *conf_;
};

}


#endif //LUOKV_LKV_H
