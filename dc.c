#include "dc.h"
#include <stdio.h>
#include <string.h>

unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];

unsigned char imageCopy[BMP_WIDTH][BMP_HEIGTH];

void splitCells(unsigned char (*gs_image)[BMP_HEIGTH], int cellSize, int holeSize) {
    int consecutiveWhitesX[BMP_HEIGTH] = {0};
    int consecutiveWhitesY = 0;

    for (int x = 0; x < BMP_WIDTH; x++)
    {
        consecutiveWhitesY = 0;
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            if (gs_image[x][y] == 1) {
                consecutiveWhitesY++;
                consecutiveWhitesX[y] = consecutiveWhitesX[y] + 1;
                if (consecutiveWhitesY >= cellSize * 1.5) {
                    consecutiveWhitesY = cellSize * 0.75;
                    for (int i = -holeSize; i < holeSize; i++) {
                        gs_image[x + i][y - (int)(cellSize * 0.5)] = 0;
                    }
                }
                if (consecutiveWhitesX[y] >= cellSize * 1.5) {
                    for (int i = -holeSize; i < holeSize; i++) {
                        gs_image[x - (int)(cellSize * 0.5)][y + i] = 0;
                    }
                    for (int i = 0; i < cellSize; i++) {
                        if (consecutiveWhitesX[y + i] > cellSize){
                            consecutiveWhitesX[y + i] = cellSize * 0.75;
                        }
                    }
                }
            } else {
                consecutiveWhitesX[y] = 0;
                consecutiveWhitesY = 0;
            }
        }
    }
}

void greyscale(unsigned char (*input_image)[BMP_HEIGTH][BMP_CHANNELS],
               unsigned char (*gs_image)[BMP_HEIGTH]){
    int nonWhiteCellCounter = 0;

    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            nonWhiteCellCounter = 0;
            for (int k = -2; k < 3; k++) {
                for (int j = -2; j < 3; j++) {
                    if(x + k < 0 || x + k > BMP_WIDTH - 1) {
                        break;
                    }
                    if (y + j < 0 || y + j > BMP_HEIGTH - 1) {
                        continue;
                    }
                    if(x + k == 0 || x + k == BMP_WIDTH - 1 || y + j == 0 || y + j == BMP_HEIGTH - 1) {
                        nonWhiteCellCounter++;
                        break;
                    }
                    if (input_image[x + k][y + j][0] < THRESHOLD) {
                        nonWhiteCellCounter++;
                        break;
                    }
                }
                if (nonWhiteCellCounter) {
                    break;
                }
            }
            if (nonWhiteCellCounter == 0) {
                gs_image[x][y] = 1;
            } else gs_image[x][y] = 0;
        }
    }
    splitCells(gs_image, 23, 15);
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
            if (!gs_image[i][j]) {
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

int min(int a, int b) {
    return a > b ? b : a;
}
int max(int a, int b) {
    return a > b ? a : b;
}

void set_color(unsigned char (*input_image)[BMP_HEIGTH][BMP_CHANNELS], const coordinate* coords, int count, int r, int g, int b, int x_offset, int y_offset) {
    for (int i = 0; i < count; i++) {
        input_image[min(x_offset + coords[i].x, BMP_HEIGTH-1)][min(y_offset + coords[i].y, BMP_HEIGTH-1)][0] = r;
        input_image[min(x_offset + coords[i].x, BMP_HEIGTH-1)][min(y_offset + coords[i].y, BMP_HEIGTH-1)][1] = g;
        input_image[min(x_offset + coords[i].x, BMP_HEIGTH-1)][min(y_offset + coords[i].y, BMP_HEIGTH-1)][2] = b;
    }
}
void outputImage(unsigned char (*input_image)[BMP_HEIGTH][BMP_CHANNELS],
                    char*  output_file_path, coordinate centers[],
                    int* cellCount){

    const coordinate red[] = {
        {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}, {0, 8},
        {1, 2}, {2, 1}, {3, 1}, {4, 0}, {5, 0}, {6, 0},
        {7, 0}, {8, 1}, {9, 1}, {10, 2}, {11, 3}, {12, 4},
        {12, 5}, {13, 6}, {14, 7}, {14, 9}, {13, 11}, {12, 12},
        {11, 13}, {11, 14}, {11, 15}, {11, 16}, {10, 17}
    };
    const coordinate blue[] = {
        {3, 9}, {4, 9}, {5, 8}, {6, 8}, {7, 8}, {4, 11}, {5, 11},
        {6, 10}, {7, 10}, {13, 8}, {14, 8}, {15, 8}, {16, 9}, {17, 9},
        {13, 10}, {14, 10}, {15, 11}, {16, 11}
    };

    const coordinate black[] = {
        {8, 5}, {8, 6}, {9, 8}, {9, 10}, {10, 5}, {10, 6}, {10, 8},
        {10, 9}, {11, 8}, {11, 10}
    };
    coordinate corner;
    for (int j = 0; j < *cellCount; j++) {
        corner.x = max(centers[j].x - 9, 0);
        corner.y = max(centers[j].y - 9,0);
        set_color(input_image, red, sizeof(red) / sizeof(red[0]), 255, 0, 0, corner.x, corner.y);
        set_color(input_image, blue, sizeof(blue) / sizeof(blue[0]), 0, 0, 255, corner.x, corner.y);
        set_color(input_image, black, sizeof(black) / sizeof(black[0]), 0, 0, 0, corner.x, corner.y);
        set_color(input_image, centers, *cellCount, 0, 255, 0, 0, 0);
    }
    write_bitmap(input_image, output_file_path);

} // Used at the end of detect cells, to output image with crosses

void outputGSImage(unsigned char (*gs_image)[BMP_HEIGTH],
                    char*  output_file_path){

    for (int x = 0; x < BMP_WIDTH; x++) {
        for (int y = 0; y < BMP_WIDTH; y++) {
            if (gs_image[x][y] == 3) {
                output_image[x][y][2] = 255;
                continue;
            }
            for (int c = 0; c < BMP_CHANNELS; c++) {
                output_image[x][y][c] = gs_image[x][y] * 255;
            }
        }
    }

    write_bitmap(output_image, output_file_path);

}

void inclusionFrameDrawer(unsigned char (*inputImage)[BMP_HEIGTH][BMP_CHANNELS], int x, int y, int jBreak, int kBreak) {
    for (int k = (-HALF_AREA) + 2; k < HALF_AREA; k++) {
        for (int j = (-HALF_AREA) + 2; j < HALF_AREA; j++) {
            inputImage[(x + k)][(y + j)][0] = 0;
            inputImage[(x + k)][(y + j)][1] = 250;
            inputImage[(x + k)][(y + j)][2] = 0;
        }
    }
}

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

void detectCells(unsigned char (*gs_image)[BMP_HEIGTH], int* cellCount, coordinate centers[]){
    unsigned char whiteDetected = 0;

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
                    if (gs_image[x + k][y + j] == 1) {
                        whiteDetected++;
                    }
                    imageCopy[x + k][y + j] = gs_image[x + k][y + j];
                    gs_image[x + k][y + j] = 0;
                }
            }
            if (whiteDetected >= MINIMUM_WHITE_IN_CELL) {

                centers[*cellCount].x = x;
                centers[*cellCount].y = y;
                (*cellCount)++;
            }
            else if (whiteDetected) {
                printf("YO");
                for (int k = (-HALF_AREA) + 2; k < HALF_AREA; k++) {
                    for (int j = (-HALF_AREA) + 2; j < HALF_AREA; j++) {
                        gs_image[x + k][y + j] = imageCopy[x + k][y + j];
                    }
                }
            }
        }
    }
}

void removeCells(unsigned char (*gs_image)[BMP_HEIGTH], int minNumberOfCells) {
    int cellCount = 0;
    for (int x = HALF_AREA-1; x < BMP_WIDTH - HALF_AREA; x++)
    {
        for (int y = HALF_AREA-1; y < BMP_HEIGTH - HALF_AREA; y++){
            if (exclusionFrame(gs_image, &x, &y)) {
                continue;
            }
            for (int k = (-HALF_AREA) + 2; k < HALF_AREA; k++) {
                for (int j = (-HALF_AREA) + 2; j < HALF_AREA; j++) {
                    if(gs_image[x + k][y + j]) {
                        cellCount++;
                    }
                    if (cellCount >= minNumberOfCells) {
                        break;
                    }
                }
                if (cellCount >= minNumberOfCells) {
                    break;
                }
            }

            if (cellCount > 0 && cellCount < minNumberOfCells) {
                for (int k = (-HALF_AREA) + 2; k < HALF_AREA; k++) {
                    for (int j = (-HALF_AREA) + 2; j < HALF_AREA; j++) {
                        gs_image[x + k][y + j] = 0;
                    }
                }
            }
        }
    }
}

