#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <pico/stdlib.h>

int main(void) {
    stdio_init_all();

    FILE *fp = fopen("/HELLO.TXT", "w");
    if (fp == NULL)
        printf("fopen error: %s\n", strerror(errno));
    fprintf(fp, "Hello World!\n");
    int err = fclose(fp);
    if (err == -1)
        printf("close error: %s\n", strerror(errno));

    fp = fopen("/HELLO.TXT", "r");
    if (fp == NULL)
        printf("fopen error: %s\n", strerror(errno));
    char buffer[512] = {0};
    fgets(buffer, sizeof(buffer), fp);
    fclose(fp);

    printf("HELLO.TXT: %s", buffer);
}
