# Example of flexible file system support for pico-sdk.

This project is a demonstration of the **pico_filesystem** library, which was added independently to pico-sdk 1.5 _develop_ branch. The library adds a POSIX file API compliant file manipulation API that allows access to Pico's on-board flash memory and SD card.
The project requires a **forked pico-sdk**. Check it out and specify it in `PICO_SDK_PATH`:

## How to build

```bash
git clone --branch filesystem_support https://github.com/oyama/pico-sdk.git
cd pico-sdk
git submodule update --init
cd ..

git clone https://github.com/oyama/pico-filesystem-examples.git
cd pico-filesystem-examples
mkdir build; cd build
PICO_SDK_PATH=../../pico-sdk cmake ..
make
```

## More details

The pico\_filesystem library is based on the _pico-vfs_ library. See the [pico-vfs repository](https://github.com/oyama/pico-vfs) for more information on the available APIs, file systems, etc.
