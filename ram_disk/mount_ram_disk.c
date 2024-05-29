#include <stdio.h>
#include <string.h>
#include <pico/filesystem.h>
#include <pico/filesystem/blockdevice/heap.h>
#include <pico/filesystem/filesystem/littlefs.h>


#define RAM_DISK_SIZE  (64 * 1024)  // 64KB


bool fs_init(void) {
    printf("Initialize custom file system\n");

    blockdevice_t *heap = blockdevice_heap_create(RAM_DISK_SIZE);
    filesystem_t *lfs = filesystem_littlefs_create(500, 16);

    int err = fs_format(lfs, heap);
    if (err == -1) {
        printf("format RAMDISK failed: %s\n", strerror(errno));
        return false;
    }
    err = fs_mount("/", lfs, heap);
    if (err == -1) {
        printf("RAM disk mount failed: %s\n", strerror(errno));
        return false;
    }

    return err == 0;
}
