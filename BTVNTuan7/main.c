#include <stdio.h>
#include "gf16.h"
#include "gf256.h"

int main(void) {
    int choice;
    unsigned int a, b;

    printf("Chọn trường hữu hạn để thực hiện phép nhân:\n");
    printf("1. GF(2^4)\n");
    printf("2. GF(2^8)\n");
    printf("Nhập lựa chọn của bạn (1 hoặc 2): ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("Nhập hai phần tử trong GF(2^4): ");
            scanf("%u %u", &a, &b);
            printf("Kết quả: %u\n", gf16_nhan(a, b));
            break;
        case 2:
            printf("Nhập hai phần tử trong GF(2^8): ");
            scanf("%u %u", &a, &b);
            printf("Kết quả: %u\n", gf256_nhan(a, b));
            break;
        default:
            printf("Lựa chọn không hợp lệ.\n");
    }

    return 0;
}