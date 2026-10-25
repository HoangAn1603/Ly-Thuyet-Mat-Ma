# Ly-Thuyet-Mat-Ma
BTVN Tuần 7:
    1. Cài đặt giải thuật thuật toán nhân 2 đa thức trong trường hữu hạn GF(2^4), GF(2^8) tương ứng với đa thức tối giản trong trường 
    2. Tạo repository riêng trên git và nộp lại link git (để tài khoản tên thật)

Nguyên Lý Thuật toán:

Phần tử của GF(2^n) là đa thức nhị phân bậc < n, biểu diễn bằng số nguyên (mỗi bit là một hệ số).
Ví dụ x^3 + x + 1 ứng với 1011b = 0xB.

Nhân theo kiểu “Russian peasant”:
    1. res = 0
    2. Lặp khi b != 0:
        - Nếu bit thấp nhất của b bằng 1 thì res ^= a (cộng trong GF(2) là XOR).
        - b >>= 1
        - a <<= 1 (nhân a với x). Nếu a có bit bậc n thì a ^= poly (rút gọn modulo đa thức tối giản).
    3. Trả về res
