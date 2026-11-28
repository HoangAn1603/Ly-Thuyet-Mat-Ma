#include <stdio.h>
#include <string.h>
#include "aes.h"

// SubBytes (bảng S-box)
static const uint8_t SBOX[256] = {
    0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
    0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
    0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
    0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
    0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
    0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
    0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
    0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
    0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
    0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
    0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
    0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
    0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
    0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
    0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};

static uint8_t INV_SBOX[256];
static bool inv_ready = false;

static void init_inv_sbox(void) {
    if (inv_ready) return;
    for (int i = 0; i < 256; ++i) INV_SBOX[SBOX[i]] = (uint8_t)i;
    inv_ready = true;
}

// Phép nhân trong GF(2^8), đa thức x^8 + x^4 + x^3 + x + 1 (0x11B)
static uint8_t xtime(uint8_t a) {
    return (uint8_t)((a << 1) ^ ((a & 0x80) ? 0x1B : 0x00));
}
static uint8_t gmul(uint8_t a, uint8_t b) {
    uint8_t result = 0;
    while (b) {
        if (b & 1) result ^= a;
        a = xtime(a);
        b >>= 1;
    }
    return result;
}

// SubBytes và nghịch đảo
void SubBytes_function(uint8_t state[16]) {
    for (int i = 0; i < 16; ++i) state[i] = SBOX[state[i]];
}
void InvSubBytes_function(uint8_t state[16]) {
    init_inv_sbox();
    for (int i = 0; i < 16; ++i) state[i] = INV_SBOX[state[i]];
}

// ShiftRows và nghịch đảo (hàng r dịch trái / phải r byte)
void ShiftRows_function(uint8_t state[16]) {
    uint8_t tmp[16];
    for (int c = 0; c < 4; ++c)
        for (int r = 0; r < 4; ++r)
            tmp[4 * c + r] = state[4 * ((c + r) % 4) + r];
    memcpy(state, tmp, 16);
}
void InvShiftRows_function(uint8_t state[16]) {
    uint8_t tmp[16];
    for (int c = 0; c < 4; ++c)
        for (int r = 0; r < 4; ++r)
            tmp[4 * ((c + r) % 4) + r] = state[4 * c + r];
    memcpy(state, tmp, 16);
}

// MixColumns và nghịch đảo (nhân từng cột với ma trận trong GF(2^8))
static const uint8_t MIX[4][4] = {
    {0x02, 0x03, 0x01, 0x01},
    {0x01, 0x02, 0x03, 0x01},
    {0x01, 0x01, 0x02, 0x03},
    {0x03, 0x01, 0x01, 0x02}
};
static const uint8_t INV_MIX[4][4] = {
    {0x0E, 0x0B, 0x0D, 0x09},
    {0x09, 0x0E, 0x0B, 0x0D},
    {0x0D, 0x09, 0x0E, 0x0B},
    {0x0B, 0x0D, 0x09, 0x0E}
};
static void mix_with_matrix(uint8_t state[16], const uint8_t M[4][4]) {
    uint8_t tmp[16];
    for (int c = 0; c < 4; ++c)
        for (int r = 0; r < 4; ++r) {
            uint8_t v = 0;
            for (int k = 0; k < 4; ++k) v ^= gmul(M[r][k], state[4 * c + k]);
            tmp[4 * c + r] = v;
        }
    memcpy(state, tmp, 16);
}
void MixColumns_function(uint8_t state[16])    { mix_with_matrix(state, MIX); }
void InvMixColumns_function(uint8_t state[16]) { mix_with_matrix(state, INV_MIX); }

// AddRoundKey
void AddRoundKey_function(uint8_t state[16], const uint8_t rk[16]) {
    for (int i = 0; i < 16; ++i) state[i] ^= rk[i];
}

// Tạo khóa con (11 khóa con, mỗi khóa 16 byte)
void gen_subkey(const uint8_t key[16], uint8_t k[176]) {
    static const uint8_t RCON[10] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1B,0x36};
    memcpy(k, key, 16);                          // w0..w3 = khóa
    for (int i = 4; i < 44; ++i) {               // i = chỉ số word
        uint8_t t[4];
        memcpy(t, k + 4 * (i - 1), 4);           // t = w[i-1]
        if (i % 4 == 0) {
            uint8_t tmp = t[0];                  // RotWord
            t[0] = t[1]; t[1] = t[2]; t[2] = t[3]; t[3] = tmp;
            for (int j = 0; j < 4; ++j) t[j] = SBOX[t[j]];   // SubWord
            t[0] ^= RCON[i / 4 - 1];             // Rcon
        }
        for (int j = 0; j < 4; ++j) k[4 * i + j] = k[4 * (i - 4) + j] ^ t[j];
    }
}

// Nhập/xuất
bool hex_to_block(const char *hex, uint8_t block[16]) {
    char buf[33];
    if (hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X')) hex += 2;
    size_t n = strlen(hex);
    if (n == 0 || n > 32) return false;
    memset(buf, '0', 32);
    buf[32] = '\0';
    memcpy(buf + (32 - n), hex, n);              // đệm 0 bên trái
    for (int i = 0; i < 16; ++i) {
        unsigned v;
        if (sscanf(buf + 2 * i, "%2x", &v) != 1) return false;
        block[i] = (uint8_t)v;
    }
    return true;
}
void print_hex(const char *name, const uint8_t block[16]) {
    printf("%s", name);
    for (int i = 0; i < 16; ++i) printf("%02X", block[i]);
    printf("\n");
}
void print_state(const char *name, const uint8_t state[16]) {
    printf("%s\n", name);
    for (int r = 0; r < 4; ++r) {
        printf("   ");
        for (int c = 0; c < 4; ++c) printf(" %02X", state[4 * c + r]);
        printf("\n");
    }
}

// Xử lý lõi một khối AES 128 bit
void processBlock(const uint8_t input[16], const uint8_t key[16],
                  uint8_t output[16], bool decrypt, bool in_log) {
    uint8_t k[176], state[16];
    char name[64];
    gen_subkey(key, k);
    memcpy(state, input, 16);

    if (in_log) {
        printf("AES-128: %s\n", decrypt ? "GIAI MA" : "MA HOA");
        print_hex("Input : ", input);
        print_hex("Key   : ", key);
        print_state("Khoa K0", k);
    }

    if (!decrypt) {
        AddRoundKey_function(state, k);
        if (in_log) print_state("Round 0 - sau AddRoundKey(K0)", state);
        for (int r = 1; r <= 10; ++r) {
            if (in_log) {
                printf("Round %d \n", r);
                sprintf(name, "Khoa con K%d", r);
                print_state(name, k + 16 * r);
            }
            SubBytes_function(state);
            if (in_log) print_state("Sau SubBytes", state);
            ShiftRows_function(state);
            if (in_log) print_state("Sau ShiftRows", state);
            if (r != 10) {
                MixColumns_function(state);
                if (in_log) print_state("Sau MixColumns", state);
            }
            AddRoundKey_function(state, k + 16 * r);
            if (in_log) {
                sprintf(name, "Round %d - dau ra (sau AddRoundKey)", r);
                print_state(name, state);
            }
        }
    } else {
        AddRoundKey_function(state, k + 160);
        if (in_log) print_state("Round 10 - sau AddRoundKey(K10)", state);
        for (int r = 9; r >= 0; --r) {
            if (in_log) printf("--- Round %d (nguoc) ---\n", r);
            InvShiftRows_function(state);
            if (in_log) print_state("Sau InvShiftRows", state);
            InvSubBytes_function(state);
            if (in_log) print_state("Sau InvSubBytes", state);
            AddRoundKey_function(state, k + 16 * r);
            if (in_log) {
                sprintf(name, "Sau AddRoundKey(K%d)", r);
                print_state(name, state);
            }
            if (r != 0) {
                InvMixColumns_function(state);
                if (in_log) print_state("Sau InvMixColumns", state);
            }
        }
    }

    memcpy(output, state, 16);
    if (in_log) print_hex("Output: ", output);
}

void encrypt(const uint8_t plaintext[16], const uint8_t key[16],
             uint8_t ciphertext[16], bool in_log) {
    processBlock(plaintext, key, ciphertext, false, in_log);
}
void decrypt(const uint8_t ciphertext[16], const uint8_t key[16],
             uint8_t plaintext[16], bool in_log) {
    processBlock(ciphertext, key, plaintext, true, in_log);
}