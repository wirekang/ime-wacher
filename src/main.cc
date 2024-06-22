#pragma comment(lib, "imm32.lib")

#define UNICODE
#define IMC_GETOPENSTATUS 0x5

#include <hidapi.h>
#include <windows.h>

#include <chrono>

#include "config.h"
#include "imm.h"

enum MODE : unsigned char {
  MODE_UNDEFINED = 1,
  MODE_ENG,
  MODE_KOR,
};

static void start();
static void init();
static MODE get_mode();
static void send_mode(const MODE mode);
static void sleep(const unsigned short ms);
static void exit();

int main() {
  start();
  return 1;
}

void start() {
  init();
  MODE previous_mode = MODE_UNDEFINED;

#ifdef _DEBUG
  using std::chrono::duration;
  using std::chrono::high_resolution_clock;
#endif

  for (unsigned short i = 0;; i += 1) {
#ifdef _DEBUG
    const auto start_time = high_resolution_clock::now();
#endif

    const auto mode = get_mode();

#ifdef _DEBUG
    const auto poll_time = high_resolution_clock::now();
#endif

    if (previous_mode != mode || i == FORCE_COUNT) {
      i = 0;
    }

    if (i < REPEAT_COUNT) {
#ifdef _DEBUG
      printf("send_mode(%d) %d\n", i, mode);
#endif
      send_mode(mode);
    }

#ifdef _DEBUG
    const auto send_time = high_resolution_clock::now();
#endif

    previous_mode = mode;

#ifdef _DEBUG
    const duration poll_duration = poll_time - start_time;
    const duration send_duration = send_time - poll_time;
    printf("%-4lld%-4lld %d\n", poll_duration.count() / 1000000,
           send_duration.count() / 1000000, mode);
#endif

    sleep(DELAY);
  }
}

void init() {
  if (hid_init() == -1) {
    auto error = hid_error(NULL);
    printf("\nhid_error:\n%ls\n", error);
    exit();
  };
}

MODE get_mode() {
  HWND hwnd = ImmGetDefaultIMEWnd(GetForegroundWindow());
  LRESULT status = SendMessageW(hwnd, WM_IME_CONTROL, IMC_GETOPENSTATUS, NULL);
  return status ? MODE_KOR : MODE_ENG;
}

void send_mode(MODE mode) {
  unsigned char buf[RAW_EPSIZE];
  memset(buf, 0, RAW_EPSIZE);

  static hid_device *device = NULL;
  if (device == NULL) {
    printf("hid_open()\n");
    device = hid_open(VENDOR_ID, PRODUCT_ID, NULL);
    if (device == NULL) {
      printf("no device\n");
      return;
    }
  }

  buf[1] = mode;
  if (hid_write(device, buf, RAW_EPSIZE) == -1) {
    device = NULL;
    printf("hid_write() error: %ls\n", hid_error(device));
  }
}

void sleep(unsigned short ms) {
  Sleep(ms);
}

void exit() {
  hid_exit();
  exit(1);
}
