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
#include <memory>
#include <vector>
#include <map>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace lkv
{
  namespace Base
  {

    template <typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args &&... args)
    {
      return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }

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

    class File
    {
    public:
      File(const std::string &file_name, const uint64_t size)
      {
        ok_ = true;
        size_ = size;
        fd_ = open(file_name.c_str(), O_CREAT | O_RDWR);
        if (fd_ < 0)
        {
          ok_ = false;
        }
        if (ftruncate(fd_, size) < 0)
        {
          ok_ = false;
        }
      }
      ~File()
      {
        if (fd_ >= 0)
        {
          close(fd_);
        }
      }
      bool Ok() { return ok_; }
      void PWrite(const void *buf, size_t count, int offset)
      {
        if (offset + count > size_)
        {
          ok_ = false;
        }
        else
        {
          if (pwrite(fd_, buf, count, offset) < 0)
          {
            ok_ = false;
          }
        }
      }
      void PRead(void *buf, size_t count, int offset)
      {
        if (offset + count > size_)
        {
          ok_ = false;
        }
        else
        {
          if (pread(fd_, buf, count, offset) < 0)
          {
            ok_ = false;
          }
        }
      }

    private:
      int fd_;
      uint64_t size_;
      bool ok_;
    };

    template <typename T>
    class ConcurrentQueue
    {
    public:
      ConcurrentQueue(const int64_t count) : count_(count) {}
      ~ConcurrentQueue() {}
      void Push(std::unique_ptr<T> item)
      {
        std::unique_lock<std::mutex> guard(mutex_);
        if (q_.size() >= count_)
        {
          push_cond_.wait(mutex_);
        }
        q_.push(item);
        pop_cond_.notify_all();
        return;
      }
      std::unique_ptr<T> Pop()
      {
        std::unique_lock<std::mutex> guard(mutex_);
        if (q_.empty())
        {
          pop_cond_.wait(mutex_);
        }
        std::unique_ptr<T> ret = std::move(q_.front());
        q_.pop();
        push_cond_.notify_all();
        return ret;
      }
      std::unique_ptr<T> TryPop()
      {
        std::unique_lock<std::mutex> guard(mutex_);
        if (q_.empty())
        {
          return make_unique<T>(NULL);
        }
        std::unique_ptr<T> ret = std::move(q_.front());
        q_.pop();
        push_cond_.notify_all();
        return ret;
      }

    private:
      int64_t count_;
      std::queue<std::unique_ptr<T>> q_;
      std::mutex mutex_;
      std::condition_variable push_cond_;
      std::condition_variable pop_cond_;
    };

    class PageContainer
    {
    public:
      PageContainer(const std::string &dir)
      {
        dir_ = dir;
        int ret = mkdir(dir_.c_str(), 0777);
        if (ret == 0 || errno == EEXIST)
        {
          ok_ = true;
        }
        else
        {
          ok_ = false;
        }
      }
      ~PageContainer()
      {
        for (auto &each : file_index_)
        {
          delete each.second;
        }
      }
      static const uint64_t PAGE_SIZE = 4096;
      static const uint64_t FILE_SIZE = PAGE_SIZE * 1024 * 1024;
      static const uint64_t PAGE_PER_FILE = 1024 * 1024;
      inline uint64_t GET_FILE(uint64_t index) const { return index / PAGE_PER_FILE; }
      inline uint64_t GET_PAGE(uint64_t index) const { return index % PAGE_PER_FILE; }
      inline File *AllocFile(uint64_t index)
      {
        return new File(dir_ + "/" + std::to_string(GET_FILE(index)), FILE_SIZE);
      }
      bool Ok() const { return ok_; }
      void writepage(uint64_t index, const char *buf, int size)
      {
        if (file_index_.find(GET_FILE(index)) == file_index_.end())
        {
          file_index_.emplace(GET_FILE(index), AllocFile(index));
        }
        file_index_[index]->PWrite(buf, size, GET_PAGE(index) * PAGE_SIZE);
        ok_ = file_index_[index]->Ok();
      }
      void readpage(uint64_t index, char *buf, int size)
      {
        if (file_index_.find(GET_FILE(index)) != file_index_.end())
        {
          file_index_[index]->PRead(buf, size, GET_PAGE(index) * PAGE_SIZE);
          ok_ = file_index_[index]->Ok();
        }
      }

    private:
      std::string dir_;
      std::map<uint64_t, File *> file_index_;
      bool ok_;
    };
  } // namespace Base
} // namespace lkv

#define LOG lkv::Base::LogPrint::get_instance()

#define LOG_OUT(type, msg...)

#endif //LUOKV_BASE_H
