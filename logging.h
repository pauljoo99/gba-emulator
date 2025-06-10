#include <cstdlib>
#include <stdio.h>

#pragma once

#define LOG_DEBUG(fmt, ...)                                                    \
  printf("\033[1m[LOG] %s:%d:\033[0m " fmt "\n", __FILE__, __LINE__,           \
         ##__VA_ARGS__)

#ifdef ENABLE_LOGGING
#define LOG(fmt, ...)                                                          \
  printf("\033[1m[LOG] %s:%d:\033[0m " fmt "\n", __FILE__, __LINE__,           \
         ##__VA_ARGS__)
#else
#define LOG(fmt, ...) ((void)0)
#endif

#define LOG_ABORT(fmt, ...)                                                    \
  LOG(fmt, ##__VA_ARGS__);                                                     \
  std::abort();