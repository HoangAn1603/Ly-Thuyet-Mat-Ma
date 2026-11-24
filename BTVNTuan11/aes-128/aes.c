#include <stdio.h>
#include <string.h>
#include "aes.h"

int hex_sang_khoi(const char *hex, uint8_t khoi[16]) {
    char buf[33];
    if (hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X')) hex += 2;
    size_t n = strlen(hex);
    if (n == 0 || n > 32) return 0;
    memset(buf, '0', 32); buf[32] = '\0';
    memcpy(buf + (32 - n), hex, n);       /* dem 0 ben trai */
    for (int i = 0; i < 16; i++) {
        unsigned v;
        if (sscanf(buf + 2*i, "%2x", &v) != 1) return 0;
        khoi[i] = (uint8_t)v;
    }
    return 1;
}
void in_trang_thai(const char *ten, const uint8_t tt[16]) {
    printf("%s\n", ten);
    for (int hang = 0; hang < 4; hang++) {
        printf("   ");
        for (int cot = 0; cot < 4; cot++) printf(" %02X", tt[4*cot + hang]);
        printf("\n");
    }
}