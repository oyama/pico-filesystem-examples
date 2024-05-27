# File transfer speed test

The pico\_filesystem allows multiple block devices and multiple file systems to be recombined at will. In this sample, data is copied between each combination of filesystems and the time is measured.

If no SD card is connected, build with the option `WITHOUT_BLOCKDEVICE_SD` in cmake.

```bash
cmake .. -DWITHOUT_BLOCKDEVICE_SD=YES
make
```

