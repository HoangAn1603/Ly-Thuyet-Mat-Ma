#ifndef GF256_H
#define GF256_H

#include <stdint.h>

#define GF256_POLY 0x11B /* Đa thức bất khả quy: x^8 + x^4 + x^3 + x + 1 */

/* Hàm nhân hai phần tử trong trường GF(2^8), a, b chỉ dùng 8 bit đầu */
uint8_t gf256_nhan(uint8_t a, uint8_t b);

#endif /* GF256_H */