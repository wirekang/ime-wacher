#ifndef UTIL_H_
#define UTIL_H_

#include <string>

#ifdef _DEBUG
#define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(...) __noop(__VA_ARGS__)
#endif

void sleep(unsigned short ms);

#endif  // UTIL_H_
