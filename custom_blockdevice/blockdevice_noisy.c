#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pico/filesystem/blockdevice.h>


#if !defined(PICO_VFS_BLOCKDEVICE_HEAP_BLOCK_SIZE)
#define PICO_VFS_BLOCKDEVICE_HEAP_BLOCK_SIZE        512
#endif

#define ANSI_BLUE     "\e[34m"
#define ANSI_CYAN     "\e[36m"
#define ANSI_MAGENTA  "\e[35m"
#define ANSI_CLEAR    "\e[0m"


static void print_hex(const void *buffer, size_t length) {
    const uint8_t *buf = buffer;
    size_t offset = 0;
    for (size_t i = 0; i < length; ++i) {
        if (i % 16 == 0)
            printf("0x%04u%s", offset, (i % 512) == 0 ? ">" : " ");
        if (isalnum(buf[i])) {
            printf("'%c' ", buf[i]);
        } else {
            printf("0x%02x", buf[i]);
        }
        if (i % 16 == 15) {
            printf("\n");
            offset += 16;
        } else {
            printf(", ");
        }
    }
}

typedef struct {
    size_t size;
    uint8_t *heap;
} blockdevice_noisy_config_t;

static const char DEVICE_NAME[] = "noisy";

static int init(blockdevice_t *device) {
    (void)device;
    blockdevice_noisy_config_t *config = device->config;

    if (device->is_initialized) {
        return BD_ERROR_OK;
    }
    config->heap = malloc(config->size);
    if (config->heap == NULL) {
        return -errno;  // Low layer errors in pico-vfs use negative error codes
    }

    device->is_initialized = true;
    return BD_ERROR_OK;
}

static int deinit(blockdevice_t *device) {
    blockdevice_noisy_config_t *config = device->config;

    if (!device->is_initialized) {
        return BD_ERROR_OK;
    }
    if (config->heap)
        free(config->heap);
    config->heap = NULL;
    device->is_initialized = false;

    return BD_ERROR_OK;
}

static int sync(blockdevice_t *device) {
    (void)device;
    return BD_ERROR_OK;
}

static int read(blockdevice_t *device, const void *buffer, bd_size_t addr, bd_size_t length) {
    blockdevice_noisy_config_t *config = device->config;

    memcpy((uint8_t *)buffer, config->heap + (size_t)addr, (size_t)length);
    printf(ANSI_CYAN);
    printf("Read: addr=%u, length=%u\n", (size_t)addr, (size_t)length);
    print_hex(buffer, length);
    printf(ANSI_CLEAR);

    return BD_ERROR_OK;
}

static int erase(blockdevice_t *device, bd_size_t addr, bd_size_t length) {
    (void)device;
    (void)addr;
    (void)length;

    printf(ANSI_BLUE);
    printf("Erase: addr=%u, length=%u\n", (size_t)addr, (size_t)length);
    printf(ANSI_CLEAR);

    return BD_ERROR_OK;
}

static int program(blockdevice_t *device, const void *buffer, bd_size_t addr, bd_size_t length) {
    blockdevice_noisy_config_t *config = device->config;

    memcpy(config->heap + (size_t)addr, buffer, (size_t)length);
    printf(ANSI_MAGENTA);
    printf("Program: addr=%u, length=%u\n", (size_t)addr, (size_t)length);
    print_hex(buffer, length);
    printf(ANSI_CLEAR);

    return BD_ERROR_OK;
}

static int trim(blockdevice_t *device, bd_size_t addr, bd_size_t length) {
    (void)device;
    (void)addr;
    (void)length;
    return BD_ERROR_OK;
}

static bd_size_t size(blockdevice_t *device) {
    blockdevice_noisy_config_t *config = device->config;
    return config->size;
}

blockdevice_t *blockdevice_noisy_create(size_t length) {
    blockdevice_t *device = calloc(1, sizeof(blockdevice_t));
    if (device == NULL) {
        return NULL;
    }
    blockdevice_noisy_config_t *config = calloc(1, sizeof(blockdevice_noisy_config_t));
    if (config == NULL) {
        free(device);
        return NULL;
    }

    device->init = init;
    device->deinit = deinit;
    device->read = read;
    device->erase = erase;
    device->program = program;
    device->trim = trim;
    device->sync = sync;
    device->size = size;
    device->read_size = PICO_VFS_BLOCKDEVICE_HEAP_BLOCK_SIZE;
    device->erase_size = PICO_VFS_BLOCKDEVICE_HEAP_BLOCK_SIZE;
    device->program_size = PICO_VFS_BLOCKDEVICE_HEAP_BLOCK_SIZE;
    device->name = DEVICE_NAME;
    device->is_initialized = false;

    config->size = length;
    config->heap = NULL;

    device->config = config;
    device->init(device);
    return device;
}

void blockdevice_noisy_free(blockdevice_t *device) {
    device->deinit(device);
    free(device->config);
    free(device);
}
