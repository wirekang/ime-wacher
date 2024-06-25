#include "svc.h"

#include "config.h"
#include "fetch.h"
#include "notify.h"
#include "util.h"

void svc_start() {
  auto success = fetch_init();
  if (!success) {
    printf("fetch_init failed\n");
    return;
  }

  notify_handle handle = nullptr;
  FetchResult previous_result = FetchResult::failed;
  bool new_handle = false;
  while (true) {
    sleep(TASK_DELAY);
    if (handle == nullptr) {
      handle = notify_init();
      if (handle == nullptr) {
        continue;
      }
      new_handle = true;
    }

    auto result = fetch();
    if (result == FetchResult::failed) {
      continue;
    }
    if (previous_result != result || new_handle) {
      auto eng = result == FetchResult::english;
      for (int i = 0; i < REPEAT_COUNT; i += 1) {
        success = notify(handle, eng);
        if (!success) {
          handle = nullptr;
          break;
        }
        sleep(REPEAT_DELAY);
      }
    }

    previous_result = result;
    new_handle = false;
  }
}

void svc_stop() {
  notify_free();
  fetch_free();
}
