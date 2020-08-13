//
// Created by 罗一鑫 on 2019/12/14.
//

#include "base.h"

namespace lkv 
{
namespace Base
{
  // TODO support variant arg
  void LogPrint::log_print(LogLevel level,
                           int errno,
                           const char *msg, ...)
  {
    if (level >= level_)
    {
      printf("[%s][%s] %s \n",
             log_level_map[level], err_info_map[errno], msg);
    }
  }
} // namespace Base
} // lkv
