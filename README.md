# IME Watcher

A simple Windows Service, `ime-watcher`, running in the background, fetching IME status changing and notify it to keyboard via RAW Hid.

## Usage - Windows

1. Update `src/config.h` to fit with your keyboard(VENDOR_ID, PRODUCT_ID,USAGE_PAGE,USAGE_ID, RAW_EPSIZE).  
2. Run `build.cmd` to build executable.   
3. Run `install.cmd` or run `_install.cmd` with admin permission.  

## Usage - QMK

```cpp
void raw_hid_receive(uint8_t *data, uint8_t length) {
    if(data[0] == 0){
            // english
    }
    if(data[0]== 1){
        // non-english
    }
}
```
