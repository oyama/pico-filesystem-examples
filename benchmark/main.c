/*
 * Benchmark tests with different block devices and file systems
 *
 * Copyright 2024, Hiroyuki OYAMA. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <errno.h>
#include <fcntl.h>
#include <hardware/clocks.h>
#include <pico/stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pico/filesystem.h>
#include <pico/filesystem/blockdevice/flash.h>
#include <pico/filesystem/blockdevice/sd.h>
#include <pico/filesystem/filesystem/fat.h>
#include <pico/filesystem/filesystem/littlefs.h>

#define COLOR_RED(format)    ("\e[31m" format "\e[0m")
#define COLOR_GREEN(format)  ("\e[32m" format "\e[0m")
#define BENCHMARK_SIZE       (1.0 * 1024 * 1024)


struct combination_map {
    blockdevice_t *device;
    filesystem_t *filesystem;
};

#if !defined(WITHOUT_BLOCKDEVICE_SD)
#define NUM_COMBINATION    4
#else
#define NUM_COMBINATION    2
#endif

static struct combination_map combination[NUM_COMBINATION];


static void init_filesystem_combination(void) {
    blockdevice_t *flash = blockdevice_flash_create(0.5 * 1024 * 1024, 0);
    filesystem_t *fat = filesystem_fat_create();
    filesystem_t *littlefs = filesystem_littlefs_create(500, 16);
    combination[0] = (struct combination_map){.device = flash, .filesystem = fat};
    combination[1] = (struct combination_map){.device = flash, .filesystem = littlefs};

#if !defined(WITHOUT_BLOCKDEVICE_SD)
    blockdevice_t *sd = blockdevice_sd_create(spi0,
                                              PICO_DEFAULT_SPI_TX_PIN,
                                              PICO_DEFAULT_SPI_RX_PIN,
                                              PICO_DEFAULT_SPI_SCK_PIN,
                                              PICO_DEFAULT_SPI_CSN_PIN,
                                              24 * MHZ,
                                              false);
    combination[2] = (struct combination_map){.device = sd, .filesystem = fat};
    combination[3] = (struct combination_map){.device = sd, .filesystem = littlefs};
#endif
}

static uint32_t xor_rand(uint32_t *seed) {
    *seed ^= *seed << 13;
    *seed ^= *seed >> 17;
    *seed ^= *seed << 5;
    return *seed;
}

static uint32_t xor_rand_32bit(uint32_t *seed) {
    return xor_rand(seed);
}

static void benchmark_write(void) {
    printf("Write ");
    uint64_t start_at = get_absolute_time();

    int fd = open("/benchmark", O_WRONLY|O_CREAT);
    if (fd == -1) {
        printf("open error: %s\n", strerror(errno));
        return;
    }

    uint32_t counter = 0;
    xor_rand(&counter);
    uint8_t buffer[1024*64] = {0};
    size_t remind = BENCHMARK_SIZE;
    while (remind > 0) {
        size_t chunk = remind % sizeof(buffer) ? remind % sizeof(buffer) : sizeof(buffer);
        uint32_t *b = (uint32_t *)buffer;
        for (size_t j = 0; j < (chunk / sizeof(uint32_t)); j++) {
            b[j] = xor_rand_32bit(&counter);
        }
        ssize_t write_size = write(fd, buffer, chunk);
        if (write_size == -1) {
            printf("write: error: %s\n", strerror(errno));
            return;
        }
        printf(".");
        remind = remind - write_size;
    }

    int err = close(fd);
    if (err == -1) {
        printf("close error: %s\n", strerror(errno));
        return;
    }

    double duration = (double)absolute_time_diff_us(start_at, get_absolute_time()) / 1000 / 1000;
    printf(" %.1f KB/s\n", (double)(BENCHMARK_SIZE) / duration / 1024);
}

static void benchmark_read(void) {
    printf("Read  ");
    uint64_t start_at = get_absolute_time();

    int fd = open("/benchmark", O_RDONLY);
    if (fd == -1) {
        printf("open error: %s\n", strerror(errno));
        return;
    }

    uint32_t counter = 0;
    xor_rand(&counter);
    uint8_t buffer[1024*64] = {0};
    size_t remind = BENCHMARK_SIZE;
    while (remind > 0) {
        size_t chunk = remind % sizeof(buffer) ? remind % sizeof(buffer) : sizeof(buffer);
        ssize_t read_size = read(fd, buffer, chunk);
        if (read_size == -1) {
            printf("read error: %s\n", strerror(errno));
            return;
        }
        uint32_t *b = (uint32_t *)buffer;
        for (size_t j = 0; j < chunk / sizeof(uint32_t); j++) {
            volatile uint32_t v = xor_rand_32bit(&counter);
            if (b[j] != v) {
                printf("data mismatch\n");
                return;
            }
        }
        printf(".");
        remind = remind - read_size;
    }

    int err = close(fd);
    if (err == -1) {
        printf("close error: %s\n", strerror(errno));
        return;
    }

    double duration = (double)absolute_time_diff_us(start_at, get_absolute_time()) / 1000 / 1000;
    printf(" %.1f KB/s\n", (double)(BENCHMARK_SIZE) / duration / 1024);
}

int main(void) {
    stdio_init_all();
    init_filesystem_combination();

    for (size_t i = 0; i < NUM_COMBINATION; i++) {
        struct combination_map setting = combination[i];
        printf("Test of %s on %s:\n", setting.filesystem->name, setting.device->name);

        int err = fs_format(setting.filesystem, setting.device);
        if (err == -1) {
            printf("fs_format error: %s\n", strerror(errno));
            return -1;
        }
        err = fs_mount("/", setting.filesystem, setting.device);
        if (err == -1) {
            printf("fs_mount / error: %s\n", strerror(errno));
            return -1;
        }

        benchmark_write();
        benchmark_read();

        err = fs_unmount("/");
        if (err == 01) {
            printf("fs_unmount / error: %s\n", strerror(errno));
            return -1;
        }
    }
    printf(COLOR_GREEN("ok\n"));
}
