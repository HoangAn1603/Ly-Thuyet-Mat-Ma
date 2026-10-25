#include "gf256.h"

uint8_t gf256_nhan(uint8_t a, uint8_t b) {
    /* Triển khai phép nhân trong trường GF(2^8) sử dụng đa thức bất khả quy GF256_POLY */
    uint8_t result = 0;
    a &= 0xFF;  /* Chỉ giữ lại 8 bit thấp nhất của a */
    b &= 0xFF;  /* Chỉ giữ lại 8 bit thấp nhất */

    while (b) {
        if (b & 1) {
            result ^= a; /* Cộng (XOR) a và kết quả */
        }
        
        uint8_t carry = a & 0x80; /* bit x^7: sắp tràn lên x^8 */
        a <<= 1; /* a = a * x */
        if (carry) {  /* Nếu a vượt quá 8 bit */
            a ^= (GF256_POLY & 0xFF);  /* Giảm modulo đa thức bất khả quy (x^8 = x^4 + x^3 + x + 1 -> XOR 0x1B) */
        }
        b >>= 1; /* Dịch phải b để xử lý bit tiếp theo */
    }
    
    return result & 0xFF; /* Trả về kết quả chỉ giữ lại 8 bit thấp nhất */
}