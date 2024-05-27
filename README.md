# Example of pico\_filesystem library for pico-sdk.

This project is a demonstration codes of the **pico_filesystem** library, which was added independently to pico-sdk 1.5 _develop_ branch. The library adds a POSIX file API compliant file manipulation API that allows access to Pico's on-board flash memory and SD card.
The project requires a [**forked pico-sdk**](https://github.com/oyama/pico-sdk/tree/filesystem_support). Check it out and specify it in `PICO_SDK_PATH`:

Pull requests to pico-sdk are as follows:
https://github.com/raspberrypi/pico-sdk/pull/1715

## How to build forked pico-sdk

```bash
git clone --branch filesystem_support https://github.com/oyama/pico-sdk.git
cd pico-sdk
git submodule update --init
```

## Build samples

Go to the directory of each sample code:
```bash
mkdir build; cd build
PICO_SDK_PATH=/path/to/forked/pico-sdk cmake ..
make
```

If no SD card device is connected, the `WITHOUT_BLOCKDEVICE_SD` option can be specified to skip the SD card manipulation procedure from the examples:

```bash
PICO_SDK_PATH=/path/to/forked/pico-sdk cmake .. -DWITHOUT_BLOCKDEVICE_SD=YES
```

## Running examples of using SD cards

If an SD card is to be used, a separate circuit must be connected via SPI. As an example, the schematic using the [Adafruit MicroSD card breakout board+](https://www.adafruit.com/product/254) is as follows

![adafruit-microsd](https://github.com/oyama/pico-vfs/assets/27072/b96e8493-4f3f-4d44-964d-8ada61745dff)

The spi and pin used in the block device argument can be customised. The following pins are used in the demonstration.

| Pin  | PCB pin | Usage    | description             |
|------|---------|----------|-------------------------|
| GP18 | 24      | SPI0 SCK | SPI clock               |
| GP19 | 25      | SPI0 TX  | SPI Master Out Slave In |
| GP16 | 21      | SPI0 RX  | SPI Master In Slave Out |
| GP17 | 22      | SPI0 CSn | SPI Chip select         |


## More details

The pico\_filesystem library is based on the _pico-vfs_ library. See the [pico-vfs repository](https://github.com/oyama/pico-vfs) for more information on the available APIs, file systems, etc.
