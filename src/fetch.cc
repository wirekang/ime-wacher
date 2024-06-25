#include "fetch.h"

#include <windows.h>

#include "config.h"
#include "util.h"

#ifdef LEGACY_FETCH
#pragma comment(lib, "imm32.lib")
#define IMC_GETOPENSTATUS 0x5

#include "imm.h"
#endif

#ifdef LEGACY_FETCH
FetchResult fetch() {
  auto foreground_window = GetForegroundWindow();
  if (foreground_window == NULL) {
    printf("GetForegroundWindow failed\n");
    return FetchResult::failed;
  }
  auto ime_window = ImmGetDefaultIMEWnd(foreground_window);
  if (ime_window == NULL) {
    printf("ImmGetDefaultIMEWnd failed.\n");
    return FetchResult::failed;
  }
  auto status =
      SendMessageW(ime_window, WM_IME_CONTROL, IMC_GETOPENSTATUS, NULL);
  return status == 0 ? FetchResult::english : FetchResult::non_english;
}
#endif

bool fetch_init() {
  return true;
}

void fetch_free() {}
