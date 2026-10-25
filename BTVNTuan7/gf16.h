#ifndef GF16_H
#define GF16_H

#include <stdint.h>

#define GF16_POLY 0x13 /* Đa thức bất khả quy: x^4 + x + 1 */

/* Hàm nhân hai phần tử trong trường GF(2^4), a, b chỉ dùng 4 bit đầu */
uint8_t gf16_nhan(uint8_t a, uint8_t b);

#endif /* GF16_H */