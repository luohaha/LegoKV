//
// Created by 罗一鑫 on 2019/12/14.
//

#ifndef LUOKV_BASE_H
#define LUOKV_BASE_H

// 基础头文件

#include "stdio.h"
#include "stdlib.h"

// 错误码定义

#define ERR_SUCCESS 0
#define ERR_UNEXPECTED 1

namespace Base
{
// 日志打印宏定义
  enum LogLevel : int
    {
        DEBUG = 0,
        INFO,
        WARN,
        ERROR
    };
    
    // level map
    static const char *log_level_map[4] 
      = {"DEBUG", "INFO", "WARN", "ERROR"};

    // err info map
    static const char *err_info_map[2]
      = {"ERR_SUCCESS", "ERR_UNEXPECTED"};

    class LogPrint
    {
    public:
        LogPrint() {}
        ~LogPrint() {}
        void log_print(LogLevel level, int errno, const char *msg, ...);
	static LogPrint &get_instance() 
	{ 
	  static LogPrint log_p;
	  return log_p;
	}
    private:
	LogLevel level_;
    };
}

#define LOG Base::LogPrint::get_instance()

#define LOG_OUT(type, msg ...)

#endif //LUOKV_BASE_H
