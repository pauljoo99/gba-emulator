#include <cstdlib>
#include <stdio.h>

#pragma once

#define LOG(fmt, ...)                                                          \
  printf("\033[1m[LOG] %s:%d:\033[0m " fmt "\n", __FILE__, __LINE__,           \
         ##__VA_ARGS__)

#ifdef ENABLE_LOGGING
#define LOG_VERBOSE(fmt, ...)                                                  \
  printf("\033[1m[LOG] %s:%d:\033[0m " fmt "\n", __FILE__, __LINE__,           \
         ##__VA_ARGS__)
#else
#define LOG_VERBOSE(fmt, ...) ((void)0)
#endif

#define ABORT(fmt, ...)                                                        \
  LOG_VERBOSE(fmt, ##__VA_ARGS__);                                             \
  std::abort();