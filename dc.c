#include "dc.h"
#include <stdio.h>
#include <string.h>


unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];

void greyscale(unsigned char (*input_image)[BMP_HEIGTH][BMP_CHANNELS],
               unsigned char* gs_arr){
    unsigned char THRESHOLD = 90;

    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            if (input_image[x][y][0] > THRESHOLD) {

                SET_BIT(gs_arr, x, y);

            } else {
                CLEAR_BIT(gs_arr, x, y);
            }

            if (!x || !y || x == BMP_WIDTH-1 || y == BMP_HEIGTH-1) {
                CLEAR_BIT(gs_arr, x, y);
            }
        }
    }
}

coordinate marked[950 * 950] = {0, 0};
void erodeImage(unsigned char* gs_arr, char* done){
    *done = 1;
    int counter = 0;
    int numCont = 0;
    for (int i = 1; i < BMP_WIDTH; i++) {
        for (int j = 1; j < BMP_HEIGTH; j++) {
            if(GET_BIT(gs_arr, i, j) && GET_BIT(gs_arr, (i-1), j) && GET_BIT(gs_arr, (i+1) , j) && GET_BIT(gs_arr, i, (j-1)) && GET_BIT(gs_arr, i, (j+1) )) {
                numCont++;
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
    printf("AYY: %d\n", numCont);
    for (int i = 0; i < counter; i++) {
        CLEAR_BIT(gs_arr, marked[i].x, marked[i].y);
    }
} //gs = greyscaled.

char exclusionFrame(unsigned char* gs_arr, int *x, int *y, char area) {

    int x_ = *x - area + 1;
    int y_ = *y - area + 1;
    for (int i = 0;  i < area*2; i++) {
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



void detectCells(unsigned char* gs_arr, int* cellCount, coordinate centers[], unsigned char area){
    char whiteDetected = 0;

    for (int x = area-1; x < BMP_WIDTH - area; x++)
    {
        for (int y = area-1; y < BMP_HEIGTH - area; y++)
        {
            whiteDetected = 0;

            if (exclusionFrame(gs_arr, &x, &y, area)) {
                continue;
            }
            for (int k = (-area) + 2; k < area; k++) {
                for (int j = (-area) + 2; j < area; j++) {
                    if (GET_BIT(gs_arr, (x + k), (y + j)) > 0) {
                        whiteDetected = 1;
                    }
                    CLEAR_BIT(gs_arr, (x + k), (y + j));
                }
            }
            if (whiteDetected == 1) {

                centers[*cellCount].x = x;
                centers[*cellCount].y = y;

                (*cellCount)++;
            }
        }
    }
}

void splitCellsExclusionFrame(unsigned char* gs_arr, int *x, int *y ) {
    int x_ = *x - HALF_AREA + 1;
    int y_ = *y - HALF_AREA + 1;
    for (int i = 0;  i < HALF_AREA*2; i++) {
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
    for (int i = 0; i<4; i++) {
        erodeImage(gs_arr, &temp);
    }
    char area = HALF_AREA-5;
    for (int x = area-1; x < BMP_WIDTH - area; x++)
    {
        for (int y = area-1; y < BMP_HEIGTH - area; y++)
        {
            if (exclusionFrame(gs_arr, &x, &y, area)) {
                continue;
            }
            for (int k = (-area) + 2; k < area; k++) {
                for (int j = (-area) + 2; j < area; j++) {
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
    for (int x = area-1; x < BMP_WIDTH - area; x++)
    {
        for (int y = area-1; y < BMP_HEIGTH - area; y++)
        {
          //  numberOfWhites = 0;
            onlyWhite = 1;
            for (int k = (-area) + 2; k < area; k++) {
                for (int j = (-area) + 2; j < area; j++) {
                    if (!GET_BIT(gs_arr, (x + k), (y + j))) {
                        //numberOfWhites++;
                        onlyWhite = 0;
                    }
                }
            }
            if (onlyWhite) {
                splitCellsExclusionFrame(gs_arr, &x, &y);
                count++;
            }
        }
    }

   // printf("%u \n",count);
    removeIslands(gs_arr);
}
