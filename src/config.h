#ifndef CONFIG_H_
#define CONFIG_H_

constexpr unsigned short VENDOR_ID = 0x3434;
constexpr unsigned short PRODUCT_ID = 0x051C;
constexpr unsigned short USAGE_PAGE = 0xFF60;
constexpr unsigned short USAGE_ID = 0x61;
constexpr size_t RAW_EPSIZE = 32;

// In milliseconds
constexpr unsigned short TASK_DELAY = 10;

constexpr unsigned short REPEAT_DELAY = 5;
constexpr unsigned short REPEAT_COUNT = 2;

#define LEGACY_FETCH 1

#endif  // CONFIG_H_
