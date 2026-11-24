# AES-128 - BTVN Tuan 11

## Build
gcc -Wall -Wextra aes.c main_aes.c -o output\main_aes.exe
gcc -Wall -Wextra aes.c kiem_tra_aes.c -o output\kiem_tra_aes.exe

## Chay
output\main_aes.exe                      (de bai: p=0x...11, k=0, in vong 1)
output\main_aes.exe <p> <k> 10           (in du 10 vong, vi du test vector slide 42)
output\kiem_tra_aes.exe                  (kiem tra tung ham)

## Log nop
nhat_ky\aes_de_bai_vong1.txt
nhat_ky\aes_slide42_10vong.txt
nhat_ky\aes_kiem_tra.txt