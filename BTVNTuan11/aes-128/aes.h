#ifndef AES_H
#define AES_H
#include <stdint.h>

int  hex_sang_khoi(const char *hex, uint8_t khoi[16]);
void in_trang_thai(const char *ten, const uint8_t tt[16]);

void thay_the_byte(uint8_t tt[16]);                         /* SubBytes   */
void dich_hang(uint8_t tt[16]);                             /* ShiftRows  */
void tron_cot(uint8_t tt[16]);                              /* MixColumns */
void cong_khoa_vong(uint8_t tt[16], const uint8_t kv[16]);  /* AddRoundKey */
void mo_rong_khoa(const uint8_t khoa[16], uint8_t kv[176]); /* KeyExpansion */
void ma_hoa_aes(const uint8_t ro[16], const uint8_t khoa[16],
                uint8_t mat[16], int so_vong_in);
#endif
