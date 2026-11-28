#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "aes.h"

static void Nhap_khoi(const char *thong_bao, uint8_t block[16]) {
    char buf[64];
    for (;;) {
        printf("%s", thong_bao);
        if (scanf("%63s", buf) != 1) exit(1);
        if (hex_to_block(buf, block)) return;
        printf("Loi: nhap chuoi hex toi da 32 chu so\n");
    }
}
static void Nhap_plaintext(uint8_t plaintext[16]) { Nhap_khoi("Nhap plaintext (hex): ", plaintext); }
static void Nhap_key(uint8_t key[16])             { Nhap_khoi("Nhap key (hex): ", key); }

int main(int argc, char **argv) {
    uint8_t plaintext[16], key[16], ciphertext[16], decrypted[16];

    if (argc >= 3) {                       // chạy không tương tác: main_aes <p> <k>
        if (!hex_to_block(argv[1], plaintext) || !hex_to_block(argv[2], key)) {
            printf("Loi: p/k phai la chuoi hex toi da 32 chu so\n");
            return 1;
        }
    } else {
        Nhap_plaintext(plaintext);
        Nhap_key(key);
    }

    encrypt(plaintext, key, ciphertext, true);          // mã hoá, in log
    print_hex("\nCiphertext (hex)   : ", ciphertext);

    decrypt(ciphertext, key, decrypted, false);         // giải mã để kiểm tra
    print_hex("Decrypted (hex)    : ", decrypted);
    printf("Giai ma khop ban ro: %s\n", memcmp(plaintext, decrypted, 16) == 0 ? "OK" : "FAIL");
    return 0;
}