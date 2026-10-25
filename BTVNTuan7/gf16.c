#include "gf16.h"

uint8_t gf16_nhan(uint8_t a, uint8_t b) {
    /* Triển khai phép nhân trong trường GF(2^4) sử dụng đa thức bất khả quy GF16_POLY */
    uint8_t result = 0;
    a &= 0x0F;  /* Chỉ giữ lại 4 bit thấp nhất của a */
    b &= 0x0F;  /* Chỉ giữ lại 4 bit thấp nhất */

    while (b) {
        if (b & 1) {
            result ^= a; /* Cộng (XOR) a và kết quả */
        }
        
        uint8_t carry = a & 0x08; /* bit x^3: sắp tràn lên x^4 */
        a = (a << 1) & 0x0F; /* a = a * x */
        if (carry) {  /* Nếu a vượt quá 4 bit */
            a ^= (GF16_POLY & 0x0F);  /* Giảm modulo đa thức bất khả quy (x^4 = x+1 -> XOR 0x03) */
        }
        b >>= 1; /* Dịch b sang phải để xử lý bit tiếp theo */
    }
    return result;
}
