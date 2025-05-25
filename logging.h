#pragma once

#define LOG(fmt, ...)                                                          \
  printf("\033[1m[LOG] %s:%d:\033[0m " fmt "\n", __FILE__, __LINE__,           \
         ##__VA_ARGS__)
