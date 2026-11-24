#include <stdio.h>
#include "aes.h"

int main(int argc, char **argv) {
    const char *p = "0x0000000000000011";   /* theo de bai */
    const char *k = "0x0000000000000000";
    int so_vong_in = 1;
    if (argc >= 3) { p = argv[1]; k = argv[2]; }
    if (argc >= 4) sscanf(argv[3], "%d", &so_vong_in);

    uint8_t ro[16], khoa[16], mat[16];
    if (!hex_sang_khoi(p, ro) || !hex_sang_khoi(k, khoa)) {
        printf("Loi: p/k phai la chuoi hex <= 32 chu so\n");
        return 1;
    }
    ma_hoa_aes(ro, khoa, mat, so_vong_in);
    printf("Mat ma (ciphertext) = ");
    for (int i = 0; i < 16; i++) printf("%02x", mat[i]);
    printf("\n");
    return 0;
}