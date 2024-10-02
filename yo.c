// To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
// To run (linux/mac): ./main.out example.bmp example_inv.bmp

// To compile (win): gcc cbmp.c dc.c main.c -o main.exe -std=c99
// To run (win): main.exe example.bmp example_inv.bmp

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "bitOPT.h"

// Main function
int main(/*int argc, char** argv*/)
{

    int test = 60;

    int n = (950 * 950) / 8 + 1;
    unsigned char *gs_arr = (unsigned char *)calloc(n, sizeof(unsigned char));

    int temp1 = 1;
    int temp2 = 0;

    SET_BIT(gs_arr, temp1, temp2);
    SET_BIT(gs_arr, 0, 0);
    SET_BIT(gs_arr, 0, 1);

    printf("%u \n", (1 << ((temp2 * BMP_WIDTH + temp1) % BITS_PER_BYTE)));

    printf("%u \n", 0xff & (0x00 + (1 << ((temp2 * BMP_WIDTH + temp1) % BITS_PER_BYTE))));

    printf("%u \n", (temp2 * BMP_WIDTH + temp1) / BITS_PER_BYTE);

    for (int y = 0; y < 3; y++){
        for (int x = 0; x < 3; x++){
            if (GET_BIT(gs_arr, x, y) >= 1){
                printf("%u", 1);
            }
            else{
                printf("%u", 0);
            }
        }
        printf("\n");
    }

    printf("fun \n");
    for (int x = 0; x < 4; x++){
        printf("%u \n", gs_arr[x]);
    }
    //printf("%u \n", gs_arr[950 / 8]);

    // printf("%u", test &= 0xff - (int)(pow(2, 6 -  6 + 2)));

    // 00111100

    //(3, 0) /

    // printf("%u", 0xff - (int)log2(6));
    // printf("%u", (int)log2(6));

    // 00001010
    // 00001010

    return 0;
}