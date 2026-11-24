#include <stdio.h>
#include <string.h>
#include "aes.h"

static int so_test = 0;
static int so_dat  = 0;

static void so_sanh_khoi(const char *ten, const uint8_t *got, const uint8_t *exp) {
    int ok = (memcmp(got, exp, 16) == 0);
    so_test++;
    if (ok) so_dat++;
    printf("%-28s %s\n", ten, ok ? "OK" : "FAIL");
    if (!ok) {
        in_trang_thai("  got:", got);
        in_trang_thai("  exp:", exp);
    }
}

/* Test 1: SubBytes */
static void kiem_tra_thay_the_byte(void) {
    uint8_t tt[16], dung[16];
    memset(tt, 0, 16); tt[15] = 0x11;            /* p = 0x...0011 */
    thay_the_byte(tt);
    memset(dung, 0x63, 16); dung[15] = 0x82;     /* 0x00 -> 0x63, 0x11 -> 0x82 */
    so_sanh_khoi("SubBytes (p=...11)", tt, dung);
}

/* Test 2: ShiftRows */
static void kiem_tra_dich_hang(void) {
    uint8_t tt[16];
    for (int i = 0; i < 16; i++) tt[i] = (uint8_t)i;
    dich_hang(tt);
    const uint8_t dung[16] = {0x00,0x05,0x0A,0x0F, 0x04,0x09,0x0E,0x03,
                              0x08,0x0D,0x02,0x07, 0x0C,0x01,0x06,0x0B};
    so_sanh_khoi("ShiftRows (00..0F)", tt, dung);
}

/* Test 3: MixColumns */
static void kiem_tra_tron_cot(void) {
    uint8_t tt[16];

    /* cot (63,63,63,82) trong bai tinh tay */
    memset(tt, 0, 16); tt[0]=0x63; tt[1]=0x63; tt[2]=0x63; tt[3]=0x82;
    tron_cot(tt);
    const uint8_t dung1[16] = {0x82,0x82,0x5B,0xBA, 0,0,0,0, 0,0,0,0, 0,0,0,0};
    so_sanh_khoi("MixColumns (63,63,63,82)", tt, dung1);

    /* vi du chuan FIPS-197 */
    memset(tt, 0, 16); tt[0]=0xdb; tt[1]=0x13; tt[2]=0x53; tt[3]=0x45;
    tron_cot(tt);
    const uint8_t dung2[16] = {0x8e,0x4d,0xa1,0xbc, 0,0,0,0, 0,0,0,0, 0,0,0,0};
    so_sanh_khoi("MixColumns (db,13,53,45)", tt, dung2);
}

/* Test 4: Khoa con */
static void kiem_tra_mo_rong_khoa(void) {
    uint8_t khoa[16], kv[176], dung[16];

    /* khoa toan 0: K1 = 62 63 63 63 lap lai */
    memset(khoa, 0, 16);
    mo_rong_khoa(khoa, kv);
    for (int i = 0; i < 16; i++) dung[i] = (i % 4 == 0) ? 0x62 : 0x63;
    so_sanh_khoi("K1 voi khoa toan 0", kv + 16, dung);

    /* khoa cua slide 42 */
    hex_sang_khoi("0f1571c947d9e8590cb7add6af7f6798", khoa);
    mo_rong_khoa(khoa, kv);
    hex_sang_khoi("dc9037b09b49dfe997fe723f388115a7", dung);
    so_sanh_khoi("K1 (slide 42)", kv + 16, dung);
    hex_sang_khoi("b48ef352ba98134e7f4d592086261876", dung);
    so_sanh_khoi("K10 (slide 42)", kv + 160, dung);
}

/* Test 5: Ma hoa day du */
static void kiem_tra_ma_hoa(void) {
    uint8_t ro[16], khoa[16], mat[16], dung[16];
    hex_sang_khoi("0123456789abcdeffedcba9876543210", ro);
    hex_sang_khoi("0f1571c947d9e8590cb7add6af7f6798", khoa);
    hex_sang_khoi("ff0b844a0853bf7c6934ab4364148fb9", dung);
    ma_hoa_aes(ro, khoa, mat, 0);
    so_sanh_khoi("Ciphertext (slide 42)", mat, dung);
}

int main(void) {
    printf(" KIEM TRA AES-128 \n");
    kiem_tra_thay_the_byte();
    kiem_tra_dich_hang();
    kiem_tra_tron_cot();
    kiem_tra_mo_rong_khoa();
    kiem_tra_ma_hoa();
    printf(" Tong ket: %d/%d test dat \n", so_dat, so_test);
    return (so_dat == so_test) ? 0 : 1;
}