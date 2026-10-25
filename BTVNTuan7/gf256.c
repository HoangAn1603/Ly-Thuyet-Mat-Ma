#include "gf256.h"

uint16_t gf256_nhan(uint8_t a, uint8_t b) {
    /* Triển khai phép nhân trong trường GF(2^8) sử dụng đa thức bất khả quy GF256_POLY */
    uint16_t result = 0;
    a &= 0xFF;  /* Chỉ giữ lại 8 bit thấp nhất của a */
    b &= 0xFF;  /* Chỉ giữ lại 8 bit thấp nhất */

    while (b) {
        if (b & 1) {
            result ^= a; /* Cộng (XOR) a và kết quả */
        }
        
        uint16_t carry = a & 0x80; /* bit x^7: sắp tràn lên x^8 */
        a = (a << 1) & 0xFF; /* a = a * x */
        if (carry) { // Nếu a vượt quá 8 bit
            a ^= GF256_POLY; // Giảm modulo đa thức bất khả quy
        }
        b >>= 1; /* Dịch phải b để xử lý bit tiếp theo */
    }
    
    return result & 0xFF; /* Trả về kết quả chỉ giữ lại 8 bit thấp nhất */
}