#include "common.h"

#include <cstring>
#include <ctime>
#include <iostream>

char*
getTimeString()
{
  std::time_t result = std::time(nullptr);
  char* t = std::ctime(&result);
  if (t[std::strlen(t) - 1] == '\n')
    t[std::strlen(t) - 1] = '\0';
  return t;
}

void
Logger::log(std::string msg)
{
  std::lock_guard<std::mutex> guard(logger_lock);
  std::cout << msg << std::endl;
}
