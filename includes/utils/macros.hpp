#ifndef MACROS_H
#define MACROS_H

#include <cstdio>
#include <cstdlib>
#include <cassert>

#define stringify(x) #x
#define expend_stringify(x) stringify(x)
#define todo(message) panic(__FILE__":%d: TODO: " message "\n", __LINE__)
#define todof(message, ...) panic(__FILE__":%d: TODO: " message "\n", __LINE__, __VA_ARGS__)
#define unreachable() panic(__FILE__":%d: unreachable code\n", __LINE__)
#define panic(...) do { \
    fprintf(stderr, __VA_ARGS__); \
    abort(); \
} while (0);
#endif
