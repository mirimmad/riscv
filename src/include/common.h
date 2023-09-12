#ifndef COMMON_H
#define COMMON_H

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define log(args...) fprintf(stderr, ##args)
#define fatal(args...)                                                         \
  printf("[Fatal] ");                                                          \
  printf(args);                                                                \
  exit(-1);

#endif
