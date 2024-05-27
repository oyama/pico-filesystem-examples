# pico\_filesystem unit test

pico\_filesystem is based on the [pico-vfs](https://github.com/oyama/pico-vfs) library and this example is a port of the pico-vfs unit tests for pico\_filesystem.

If no SD card is connected, build with the option `WITHOUT_BLOCKDEVICE_SD` in cmake.

```bash
cmake .. -DWITHOUT_BLOCKDEVICE_SD=YES
make
```

