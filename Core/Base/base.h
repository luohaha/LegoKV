//
// Created by 罗一鑫 on 2019/12/14.
//

#ifndef LUOKV_BASE_H
#define LUOKV_BASE_H

// base include

#include "stdio.h"
#include "stdlib.h"
#include <cassert>
#include "leveldb/db.h"
#include "errcode.h"
#include <vector>
#include <map>

namespace lkv 
{
namespace Base
{

  struct BaseType
  {
    BaseType() {}
    BaseType(const std::string &str) : buf(str) {}
    BaseType &operator=(const BaseType &b)
    {
      buf = b.buf;
      return *this;
    }
    BaseType &operator=(const std::string &s)
    {
      buf = s;
      return *this;
    }
    uint32_t Hash() const { return std::hash<std::string>{}(buf); }
    std::string buf;
  };

  // log print
  enum LogLevel : int
  {
    DEBUG = 0,
    INFO,
    WARN,
    ERROR
  };

  // level map
  static const char *log_level_map[4] = {"DEBUG", "INFO", "WARN", "ERROR"};

  class LogPrint
  {
  public:
    LogPrint() {}
    ~LogPrint() {}
    void log_print(LogLevel level, int errcode, const char *msg, ...);
    static LogPrint &get_instance()
    {
      static LogPrint log_p;
      return log_p;
    }

  private:
    LogLevel level_;
  };
} // namespace Base
}

#define LOG lkv::Base::LogPrint::get_instance()

#define LOG_OUT(type, msg...)

#endif //LUOKV_BASE_H
