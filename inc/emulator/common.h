#ifndef COMMON_H
#define COMMON_H

#include <cstdint>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>

using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

using int8 = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;

class Logger
{
public:
  static Logger& getInstance()
  {
    static Logger logger;
    return logger;
  }
  void log(std::string msg);

private:
  Logger() {}
  std::mutex logger_lock;
};

char*
getTimeString();

#define internal_log(message, args...)                                         \
  {                                                                            \
    int size_s = std::snprintf(nullptr, 0, message, args) + 1;                 \
    if (size_s <= 0) {                                                         \
      Logger::getInstance().log("Error during log formatting.");               \
    } else {                                                                   \
      std::unique_ptr<char[]> buf(new char[size_s]);                           \
      std::snprintf(buf.get(), size_s, message, args);                         \
      Logger::getInstance().log(                                               \
        std::string(buf.get(), buf.get() + size_s - 1));                       \
    }                                                                          \
  }

#define log_error(message, args...)                                            \
  internal_log("%s | %-5s | %s:%d | " message,                                 \
               getTimeString(),                                                \
               "ERROR",                                                        \
               __PRETTY_FUNCTION__,                                            \
               __LINE__,                                                       \
               ##args)

#define log_info(message, args...)                                             \
  internal_log("%s | %-5s | %s:%d | " message,                                 \
               getTimeString(),                                                \
               "INFO",                                                         \
               __PRETTY_FUNCTION__,                                            \
               __LINE__,                                                       \
               ##args)

#endif // COMMON_H
