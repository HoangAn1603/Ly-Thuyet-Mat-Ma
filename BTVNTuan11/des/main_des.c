
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "des.h"

void print_binary64(uint64_t value) {
    for (int i = 63; i >= 0; --i) {
        printf("%" PRIu64, (value >> i) & 1);
    }
    printf("\n");
}

uint64_t Nhap_plaintext() {
    uint64_t plaintext = 0;

    printf("Nhap plaintext (hex): ");
    scanf("%" SCNx64, &plaintext);

    return plaintext;
}

uint64_t Nhap_key() {
    uint64_t key = 0;

    printf("Nhap key (hex): ");
    scanf("%" SCNx64, &key);

    return key;
}

int main() {
    uint64_t plaintext = Nhap_plaintext();
    uint64_t key = Nhap_key();

    // Ma hoa
    uint64_t ciphertext = encrypt(plaintext, key);

    printf("\nCiphertext (hex)   : %" PRIX64 "\n", ciphertext);
    printf("Ciphertext (binary): ");
    print_binary64(ciphertext);

    // Giai ma
    uint64_t decrypted = decrypt(ciphertext, key);

    printf("\nDecrypted (hex)    : %" PRIX64 "\n", decrypted);
    printf("Decrypted (binary) : ");
    print_binary64(decrypted);

    return 0;
}
