#include "dc.h"
#include <stdio.h>
#include <string.h>


coordinate marked[950 * 950] = {0, 0};
void erodeImage(unsigned char* gs_arr, char* done){
    *done = 1;
    int16_t counter = 0;
    for (int16_t i = 1; i < BMP_WIDTH; i++) {
        for (int16_t j = 1; j < BMP_HEIGTH; j++) {
            if(GET_BIT(gs_arr, i, j) && GET_BIT(gs_arr, (i-1), j) && GET_BIT(gs_arr, (i+1) , j) && GET_BIT(gs_arr, i, (j-1)) && GET_BIT(gs_arr, i, (j+1) )) {
                continue;
            }
            if (GET_BIT(gs_arr, i, j) == 0) {
                continue;
            }
            marked[counter].x = i;
            marked[counter].y = j;
            (counter)++;
            *done = 0;
        }
    }
    for (int16_t i = 0; i < counter; i++) {
        CLEAR_BIT(gs_arr, marked[i].x, marked[i].y);
    }
} //gs = greyscaled.

char exclusionFrame(unsigned char* gs_arr, int16_t *x, int16_t *y, char area) {

    int16_t x_ = *x - area + 1;
    int16_t y_ = *y - area + 1;
    for (int8_t i = 0;  i < area*2; i++) {
        if (GET_BIT(gs_arr, (x_ + i), y_)) {
            return 1;
        }
        if (GET_BIT(gs_arr, x_, (y_ + i) )) {
            return 1;
        }
        if (GET_BIT(gs_arr, (x_ + i), (y_ + area*2 -1) )) {
            return 1;
        }
        if (GET_BIT(gs_arr, (x_ + area*2 - 1), (y_ + i) )){
            return 1;
        }
    }
    return 0;
}

void detectCells(unsigned char* gs_arr, int16_t* cellCount, coordinate centers[], unsigned char area){
    char whiteDetected = 0;

    for (int16_t x = area-1; x < BMP_WIDTH - area; x++)
    {
        for (int16_t y = area-1; y < BMP_HEIGTH - area; y++)
        {
            whiteDetected = 0;

            if (exclusionFrame(gs_arr, &x, &y, area)) {
                continue;
            }
            for (int8_t k = (-area) + 2; k < area; k++) {
                for (int8_t j = (-area) + 2; j < area; j++) {
                    if (GET_BIT(gs_arr, (x + k), (y + j)) > 0) {
                        whiteDetected = 1;
                    }
                    CLEAR_BIT(gs_arr, (x + k), (y + j));
                }
            }
            if (whiteDetected == 1) {

                centers[*cellCount].x = x+3;
                centers[*cellCount].y = y+3;

                (*cellCount)++;
            }
        }
    }
}

void splitCellsExclusionFrame(unsigned char* gs_arr, int16_t *x, int16_t *y ) {
    int16_t x_ = *x - HALF_AREA + 1;
    int16_t y_ = *y - HALF_AREA + 1;
    for (int8_t i = 0;  i < HALF_AREA*2; i++) {
        if (GET_BIT(gs_arr, (x_ + i), y_)) {
            CLEAR_BIT(gs_arr, x_ + i, y_);
        }
        if (GET_BIT(gs_arr, x_, (y_ + i))) {
            CLEAR_BIT(gs_arr, x_, (y_ + i));
        }
        if (GET_BIT(gs_arr, (x_ + i), (y_ + HALF_AREA*2 -1))) {
            CLEAR_BIT(gs_arr, (x_ + i), (y_ + HALF_AREA*2 -1));
        }
        if (GET_BIT(gs_arr, (x_ + HALF_AREA*2 - 1), (y_ + i))){
            CLEAR_BIT(gs_arr, (x_ + HALF_AREA*2 - 1), (y_ + i));
        }
    }
}
void removeIslands(unsigned char* gs_arr) {
    char temp = 0;
    for (int16_t i = 0; i<4; i++) {
        erodeImage(gs_arr, &temp);
    }
    char area = HALF_AREA-5;
    for (int16_t x = area-1; x < BMP_WIDTH - area; x++)
    {
        for (int16_t y = area-1; y < BMP_HEIGTH - area; y++)
        {
            if (exclusionFrame(gs_arr, &x, &y, area)) {
                continue;
            }
            for (int8_t k = (-area) + 2; k < area; k++) {
                for (int8_t j = (-area) + 2; j < area; j++) {
                    CLEAR_BIT(gs_arr, (x + k), (y + j));
                }
            }
        }
    }
}

void splitCells(unsigned char* gs_arr){
  //  char numberOfWhites = 0;
    char count = 0;
    char onlyWhite = 1;
    char area = HALF_AREA+2;
    for (int16_t x = area-1; x < BMP_WIDTH - area; x++)
    {
        for (int16_t y = area-1; y < BMP_HEIGTH - area; y++)
        {
            onlyWhite = 1;
            for (int8_t k = (-area) + 2; k < area; k++) {
                for (int8_t j = (-area) + 2; j < area; j++) {
                    if (!GET_BIT(gs_arr, (x + k), (y + j))) {
                        onlyWhite = 0;
                        break;
                    }
                }
            }
            if (onlyWhite) {
                splitCellsExclusionFrame(gs_arr, &x, &y);
                count++;
            }
        }
    }
    removeIslands(gs_arr);
}
