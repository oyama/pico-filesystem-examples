#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <pico/stdlib.h>
#include <pico/filesystem.h>


int main(void) {
    stdio_init_all();
    fs_init();

    // Create files in littlefs on on-board flash
    FILE *fp = fopen("/HELLO.TXT", "w");
    if (fp == NULL)
        printf("fopen error: %s\n", strerror(errno));
    fprintf(fp, "Hello World!\n");
    int err = fclose(fp);
    if (err == -1)
        printf("close error: %s\n", strerror(errno));

    // Read files in littlefs on on-board flash
    fp = fopen("/HELLO.TXT", "r");
    if (fp == NULL)
        printf("fopen error: %s\n", strerror(errno));
    char buffer[512] = {0};
    fgets(buffer, sizeof(buffer), fp);
    fclose(fp);

    printf("HELLO.TXT: %s", buffer);
}
