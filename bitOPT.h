#ifndef BITOPT_H
#define BITOPT_H
#define BMP_WIDTH 950
#define BMP_HEIGHT 950
#define BMP_CHANNELS 3

#define BITS_PER_BYTE 8

#define GET_BIT(arr, x, y) ((arr[(y*BMP_WIDTH + x) / BITS_PER_BYTE] >> (y*BMP_WIDTH + x) % BITS_PER_BYTE) & 1)
#define SET_BIT(arr, x, y) (arr[((y * BMP_WIDTH + x) / BITS_PER_BYTE)] |= (1 << ((y*BMP_WIDTH + x) % BITS_PER_BYTE)))
#define CLEAR_BIT(arr, x, y) (arr[(y * BMP_WIDTH + x) / BITS_PER_BYTE] &= ~(1 << ((y*BMP_WIDTH + x) % BITS_PER_BYTE)))

#endif //BITOPT_H
