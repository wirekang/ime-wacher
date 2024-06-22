#include "sample.h"

#include <hidapi.h>
#include <windows.h>

void sample() {
  hid_init();
  MessageBoxW(0, L"", L"asdf", MB_OK);
}
