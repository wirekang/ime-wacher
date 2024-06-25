#include "notify.h"

#include <hidapi.h>

#include "config.h"
#include "util.h"

constexpr static size_t BUF_SIZE = RAW_EPSIZE + 1;
static unsigned char buffer[BUF_SIZE] = {0};

notify_handle notify_init() {
  hid_device *device;
  auto device_infos = hid_enumerate(VENDOR_ID, PRODUCT_ID);
  if (device_infos == NULL) {
    printf("hid_enumerate error: %ls\n", hid_error(NULL));
    return nullptr;
  }
  auto device_info = device_infos;
  while (device_info) {
    if (device_info->usage_page == USAGE_PAGE &&
        device_info->usage == USAGE_ID) {
      printf("found device: %ls: %s\n", device_info->manufacturer_string,
             device_info->path);
      device = hid_open_path(device_info->path);
      if (device == NULL) {
        printf("hid_open_path error: %ls\n", hid_error(NULL));
        break;
      }
    }
    device_info = device_info->next;
  }
  hid_free_enumeration(device_infos);
  if (device == NULL) {
    DEBUG_PRINT("no_device\n");
    return nullptr;
  }
  return device;
}

bool notify(notify_handle h, bool is_english) {
  auto device = static_cast<hid_device *>(h);
  buffer[1] = is_english ? 0 : 1;
  DEBUG_PRINT("send %d\n", buffer[1]);
  if (hid_write(device, buffer, BUF_SIZE) == -1) {
    printf("hid_write error: %ls\n", hid_error(device));
    return false;
  }
  return true;
}

void notify_free() {
  hid_exit();
}
