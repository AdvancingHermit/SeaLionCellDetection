#include "dc.h"
#include <stdio.h>



unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];

void greyscale(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS],
               unsigned char output_image[BMP_WIDTH][BMP_HEIGTH]){
    unsigned char THRESHOLD = 90;
    unsigned char white[] = {255, 255, 255};
    unsigned char black = 0;
    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            if (input_image[x][y][0] > THRESHOLD) {

                output_image[x][y] = 1;

            } else {
                output_image[x][y] = 0;
            }

            if (!x || !y || x == BMP_WIDTH-1 || y == BMP_HEIGTH-1) {
                output_image[x][y] = 0;
            }
        }
    }
}
void erodeImage(unsigned char (*gs_image)[BMP_HEIGTH]){
    for (int i = 1; i < BMP_WIDTH; i++) {
        for (int j = 1; j < BMP_HEIGTH; j++) {
            if(gs_image[i][j] && gs_image[i-1][j] && gs_image[i+1][j] && gs_image[i][j-1] && gs_image[i][j+1]) {
                continue;
            }
            if (gs_image[i][j] == 3) {
                continue;
            }
            gs_image[i][j] = 2;
        }

    }
    for (int i = 1; i < BMP_WIDTH; i++) {
        for (int j = 1; j < BMP_HEIGTH; j++) {
            if(gs_image[i][j] == 2) {
                gs_image[i][j] = 0;
            }

        }
    }


} //gs = greyscaled.

void outputImage(unsigned char gs_image[BMP_WIDTH][BMP_HEIGTH], char *  output_file_path){

    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            if (gs_image[x][y] == 3) {
                output_image[x][y][0] = 255;
                printf("AYYYY");
                continue;
            }
            unsigned char color = gs_image[x][y] * 255;
            output_image[x][y][0] = color;
            output_image[x][y][1] = color;
            output_image[x][y][2] = color;
        }
    }
    write_bitmap(output_image, output_file_path);

} // Used at the end of detect cells, to output image with crosses

char exclusionFrame(unsigned char (*gs_image)[BMP_HEIGTH], int *x, int *y) {

    int x_ = *x - HALF_AREA + 1;
    int y_ = *y - HALF_AREA + 1;
    for (int i = 0;  i < HALF_AREA*2; i++) {
        if (gs_image[x_ + i][y_]) {

            return 1;
        }
        if (gs_image[x_][y_ + i]) {
            return 1;
        }
        if (gs_image[x_ + i][y_ + HALF_AREA*2 -1]) {
            return 1;
        }
        if (gs_image[x_ + HALF_AREA*2 - 1][y_ + i]) {
            return 1;
        }
    }

    return 0;
}

struct intPair {
    int first;
    int second;
};

void detectCells(unsigned char (*gs_image)[BMP_HEIGTH], int* cellCount){
    char whiteDetected = 0;
    char whiteOnFrame = 0;
    int lowK = 1000;
    int highK = 0;
    int lowJ = 1000;
    int highJ = 0;

    struct intPair centers[350];

    for (int x = HALF_AREA-1; x < BMP_WIDTH - HALF_AREA; x++)
    {
        for (int y = HALF_AREA-1; y < BMP_HEIGTH - HALF_AREA; y++)
        {
            whiteDetected = 0;

            if (exclusionFrame(gs_image, &x, &y)) {
                continue;
            }
            for (int k = (-HALF_AREA) + 2; k < HALF_AREA; k++) {
                for (int j = (-HALF_AREA) + 2; j < HALF_AREA; j++) {
                    if (gs_image[x + k][y + j] == 1 ) {
                        whiteDetected = 1;
                        if (k > highK) {
                            highK = k;
                        }
                        else if (k < lowK) {
                            lowK = k;
                        }
                        if (j > highJ) {
                            highJ = j;
                        }
                        else if (j < lowJ) {
                            lowJ = j;
                        }
                    }
                    gs_image[x + k][y + j] = 0;
                }
            }
            if (whiteDetected == 1) {

                int midX = x + (lowK + highK)/2;
                int midY = y + (lowJ + highJ)/2;


                centers[*cellCount].first = midX;
                centers[*cellCount].second = midY;

                (*cellCount)++;

                lowK = 1000;
                highK = 0;
                lowJ = 1000;
                highJ = 0;
            }
        }
    }
    for (int i = 0; i < *cellCount; i++) {
        printf("(%u, %u), ", centers[i].first, centers[i].second);
        gs_image[centers[i].first][centers[i].second] = 3;
    }
}

