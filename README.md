# IME Watcher

Running in the background, watching IME status, notifying it to keyboard via RAW Hid.

## Usage - Windows

1. Update `src/config.h` to fit with your keyboard(VENDOR_ID, PRODUCT_ID,USAGE_PAGE,USAGE_ID, RAW_EPSIZE).    
2. Run `build.cmd` to build executable.   
3. Move `build/bin/Release/ime-watcher.exe` to somewhere(e.g. `C:/ime-watcher.exe`), register it as [a startup task.](https://superuser.com/a/929227)(optional, recommended).   

## Usage - QMK

Configure [Raw Hid](https://docs.qmk.fm/features/rawhid).

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

