#include <stdio.h>
#include <pico/stdlib.h>

#define COLOR_GREEN(format)  ("\e[32m" format "\e[0m")

extern void test_blockdevice();
extern void test_filesystem();
extern void test_vfs();
extern void test_stdio();
extern void test_copy_between_different_filesystems(void);

int main(void) {
    stdio_init_all();

    printf("Start all tests\n");

    test_blockdevice();
    test_filesystem();
    test_vfs();
    test_stdio();
    test_copy_between_different_filesystems();

    printf(COLOR_GREEN("All tests are ok\n"));
}
