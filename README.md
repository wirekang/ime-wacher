# IME Watcher

## How it works

Read IME state and send it to keyboard via Raw HID.

## Build

1. Update `src/config.h` to fit with your keyboard(VENDOR_ID, PRODUCT_ID, RAW_EPSIZE).  
2. Run `build.cmd` (CMake and any compiler are required)
3. Move `build/bin/Release/ime-watcher.exe` to anywhere.
4. Register it as a Windows service.

## QMK

```cpp
void raw_hid_receive(uint8_t *data, uint8_t length) {
    switch(data[1]){
        case 2: // english;
        case 3: // korean;
    }
}
```
