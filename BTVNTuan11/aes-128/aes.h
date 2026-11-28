#ifndef AES_H
#define AES_H

#include <stdint.h>
#include <stdbool.h>

// Các phép biến đổi trong một vòng
void SubBytes_function(uint8_t state[16]);
void InvSubBytes_function(uint8_t state[16]);
void ShiftRows_function(uint8_t state[16]);
void InvShiftRows_function(uint8_t state[16]);
void MixColumns_function(uint8_t state[16]);
void InvMixColumns_function(uint8_t state[16]);
void AddRoundKey_function(uint8_t state[16], const uint8_t rk[16]);

// Sinh khoá con: k[0..15] = K0, k[16..31] = K1, ..., k[160..175] = K10
void gen_subkey(const uint8_t key[16], uint8_t k[176]);

// Hàm mã hoá và giải mã (in_log = true: in giá trị từng trạng thái)
void processBlock(const uint8_t input[16], const uint8_t key[16],
                  uint8_t output[16], bool decrypt, bool in_log);
void encrypt(const uint8_t plaintext[16], const uint8_t key[16],
             uint8_t ciphertext[16], bool in_log);
void decrypt(const uint8_t ciphertext[16], const uint8_t key[16],
             uint8_t plaintext[16], bool in_log);

// Nhập/xuất
bool hex_to_block(const char *hex, uint8_t block[16]);
void print_hex(const char *name, const uint8_t block[16]);
void print_state(const char *name, const uint8_t state[16]);

#endif