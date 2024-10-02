#ifndef BITOPT_H
#define BITOPT_H
#define BMP_WIDTH 950
#define BMP_HEIGTH 950
#define BMP_CHANNELS 3

#define BITS_PER_BYTE 8

#define GET_BIT_NUM(arr, x, y) (arr[(( (y * BMP_WIDTH) + x) / BITS_PER_BYTE)] & (0x00 + (1 << (((( (y * BMP_WIDTH) + x))) % BITS_PER_BYTE))) )

#define GET_BIT(arr, x, y) ((GET_BIT_NUM(arr, x, y) > 0) ? 1 : 0)

#define SET_BIT(arr, x, y) (arr[((y * BMP_WIDTH + x) / BITS_PER_BYTE)] |= (0x00 + (1 << ((((y * BMP_WIDTH + x))) % BITS_PER_BYTE))) )

#define CLEAR_BIT(arr, x, y) (arr[(y * BMP_WIDTH + x) / BITS_PER_BYTE] &= (0xff - (1 << ((((y * BMP_WIDTH + x))) % BITS_PER_BYTE))) )

#endif //BITOPT_H
