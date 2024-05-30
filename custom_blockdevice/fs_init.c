#include <stdio.h>
#include <string.h>
#include <pico/filesystem.h>
#include <pico/filesystem/filesystem/littlefs.h>

#define RAM_DISK_SIZE   (64*1024)

extern blockdevice_t *blockdevice_noisy_create(size_t size);

bool fs_init(void) {
    printf("Initialize custom file system\n");

    blockdevice_t *device = blockdevice_noisy_create(RAM_DISK_SIZE);
    filesystem_t *lfs = filesystem_littlefs_create(500, 16);

    int err = fs_format(lfs, device);
    if (err == -1) {
        printf("format Noisy disk failed: %s\n", strerror(errno));
        return false;
    }
    err = fs_mount("/", lfs, device);
    if (err == -1) {
        printf("Noisy disk mount failed: %s\n", strerror(errno));
        return false;
    }

    return err == 0;
}
