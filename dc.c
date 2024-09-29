#include "dc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];

unsigned char inputImageDraw[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];

void setInputImage(unsigned char inputImageGet[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]) {
    for (int i = 0; i < BMP_WIDTH; i++) {
        for (int j = 0; j < BMP_HEIGTH; j++) {
            for (int k = 0; k < BMP_CHANNELS; k++) {
                inputImageDraw[i][j][k] = inputImageGet[i][j][k];
            }
        }
    }
}

unsigned char (*getInputImageDraw())[BMP_HEIGTH][BMP_CHANNELS] {
    return inputImageDraw;
}



void greyscale(unsigned char (*input_image)[BMP_HEIGTH][BMP_CHANNELS],
               unsigned char (*gs_image)[BMP_HEIGTH]){
    unsigned char THRESHOLD = 90;
    unsigned char white[] = {255, 255, 255};
    unsigned char black = 0;
    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            if (input_image[x][y][0] > THRESHOLD) {

                gs_image[x][y] = 1;

            } else {
                gs_image[x][y] = 0;
            }

            if (!x || !y || x == BMP_WIDTH-1 || y == BMP_HEIGTH-1) {
                gs_image[x][y] = 0;
            }
        }
    }
}
void erodeImage(unsigned char (*gs_image)[BMP_HEIGTH], char* done, int dims[2], int offset[2]){
    *done = 1;
    for (int i = offset[0]; i < dims[0]; i++) {
        for (int j = offset[1]; j < dims[1]; j++) {
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
            *done = 0;
        }

    }
    for (int i = offset[0]; i < dims[0]; i++) {
        for (int j = offset[1]; j < dims[1]; j++) {
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

void set_color(unsigned char (*input_image)[BMP_HEIGTH][BMP_CHANNELS], const struct coordinate* coords, int count, int r, int g, int b, int x_offset, int y_offset) {
    for (int i = 0; i < count; i++) {
        input_image[min(x_offset + coords[i].x, BMP_HEIGTH-1)][min(y_offset + coords[i].y, BMP_HEIGTH-1)][0] = r;
        input_image[min(x_offset + coords[i].x, BMP_HEIGTH-1)][min(y_offset + coords[i].y, BMP_HEIGTH-1)][1] = g;
        input_image[min(x_offset + coords[i].x, BMP_HEIGTH-1)][min(y_offset + coords[i].y, BMP_HEIGTH-1)][2] = b;
    }
}
void outputImage(unsigned char (*input_image)[BMP_HEIGTH][BMP_CHANNELS],
                    char*  output_file_path, struct coordinate centers[],
                    int* cellCount){

    const struct coordinate red[] = {
        {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}, {0, 8},
        {1, 2}, {2, 1}, {3, 1}, {4, 0}, {5, 0}, {6, 0},
        {7, 0}, {8, 1}, {9, 1}, {10, 2}, {11, 3}, {12, 4},
        {12, 5}, {13, 6}, {14, 7}, {14, 9}, {13, 11}, {12, 12},
        {11, 13}, {11, 14}, {11, 15}, {11, 16}, {10, 17}
    };
    const struct coordinate blue[] = {
        {3, 9}, {4, 9}, {5, 8}, {6, 8}, {7, 8}, {4, 11}, {5, 11},
        {6, 10}, {7, 10}, {13, 8}, {14, 8}, {15, 8}, {16, 9}, {17, 9},
        {13, 10}, {14, 10}, {15, 11}, {16, 11}
    };

    const struct coordinate black[] = {
        {8, 5}, {8, 6}, {9, 8}, {9, 10}, {10, 5}, {10, 6}, {10, 8},
        {10, 9}, {11, 8}, {11, 10}
    };
    struct coordinate corner;
    for (int j = 0; j < *cellCount; j++) {
        corner.x = max(centers[j].x - 9, 0);
        corner.y = max(centers[j].y - 9,0);
        set_color(input_image, red, sizeof(red) / sizeof(red[0]), 255, 0, 0, corner.x, corner.y);
        set_color(input_image, blue, sizeof(blue) / sizeof(blue[0]), 0, 0, 255, corner.x, corner.y);
        set_color(input_image, black, sizeof(black) / sizeof(black[0]), 0, 0, 0, corner.x, corner.y);
        //set_color(input_image, centers, *cellCount, 0, 255, 0, 0, 0);
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
            inputImageDraw[(x + k)][(y + j)][0] = 0;
            inputImageDraw[(x + k)][(y + j)][1] = 250;
            inputImageDraw[(x + k)][(y + j)][2] = 0;
        }
    }
}

char exclusionFrame(unsigned char (*gs_image)[BMP_HEIGTH], int *x, int *y, char area) {

    int x_ = *x - area + 1;
    int y_ = *y - area + 1;
    for (int i = 0;  i < area*2; i++) {
        if (gs_image[x_ + i][y_]) {

            return 1;
        }
        if (gs_image[x_][y_ + i]) {
            return 1;
        }
        if (gs_image[x_ + i][y_ + area*2 -1]) {
            return 1;
        }
        if (gs_image[x_ + area*2 - 1][y_ + i]) {
            return 1;
        }
    }

    return 0;
}



void detectCells(unsigned char (*gs_image)[BMP_HEIGTH], int* cellCount, struct coordinate centers[]){
    char whiteDetected = 0;

    for (int x = HALF_AREA-1; x < BMP_WIDTH - HALF_AREA; x++)
    {
        for (int y = HALF_AREA-1; y < BMP_HEIGTH - HALF_AREA; y++)
        {
            whiteDetected = 0;

            if (exclusionFrame(gs_image, &x, &y, HALF_AREA)) {
                continue;
            }
            for (int k = (-HALF_AREA) + 2; k < HALF_AREA; k++) {
                for (int j = (-HALF_AREA) + 2; j < HALF_AREA; j++) {
                    if (gs_image[x + k][y + j] == 1) {
                        whiteDetected = 1;
                    }
                    gs_image[x + k][y + j] = 0;
                }
            }
            if (whiteDetected == 1) {

                centers[*cellCount].x = x;
                centers[*cellCount].y = y;

                //inclusionFrameDrawer(inputImageDraw, x, y, HALF_AREA, HALF_AREA);

                (*cellCount)++;
            }
        }
    }
    for (int i = 0; i < *cellCount; i++) {
        gs_image[centers[i].x][centers[i].y] = 3;
    }
}
void splitCellsExclusionFrame(unsigned char (*gs_image)[BMP_HEIGTH], int *x, int *y ) {
    int x_ = *x - HALF_AREA + 1;
    int y_ = *y - HALF_AREA + 1;
    for (int i = 0;  i < HALF_AREA*2; i++) {
        if (gs_image[x_ + i][y_]) {

            gs_image[x_ + i][y_] = 0;
        }
        if (gs_image[x_][y_ + i]) {
            gs_image[x_][y_ + i] = 0;
        }
        if (gs_image[x_ + i][y_ + HALF_AREA*2 -1]) {
            gs_image[x_ + i][y_ + HALF_AREA*2 -1] = 0;
        }
        if (gs_image[x_ + HALF_AREA*2 - 1][y_ + i]) {
            gs_image[x_ + HALF_AREA*2 - 1][y_ + i] = 0;
        }
    }

}
typedef struct {
    struct coordinate top;
    struct coordinate btm;
} Square;
char hasOverlap(Square a, Square b) {
    if (a.top.x >= b.btm.x || b.top.x >= a.btm.x)
        return 0;

    // Check if one square is above or below the other
    if (a.btm.y <= b.top.y || b.btm.y <= a.top.y)
        return 0;

    return 1;  // They overlap
}
// Function to merge two overlapping squares
Square merge(Square a, Square b) {
    Square merged;
    merged.top.x = (a.top.x < b.top.x) ? a.top.x : b.top.x;
    merged.top.y = (a.top.y < b.top.y) ? a.top.y : b.top.y;
    merged.btm.x = (a.btm.x > b.btm.x) ? a.btm.x : b.btm.x;
    merged.btm.y = (a.btm.y > b.btm.y ) ? a.btm.y : b.btm.y;
    return merged;
}
int removeOverlaps(Square squares[], int n) {
    char merged = 1; // Variable to check if any merges happened
    while (merged) { // Loop until no more merges
        merged = 0; // Reset merged flag
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (hasOverlap(squares[i], squares[j])) {
                    // Merge squares[i] and squares[j]
                    squares[i] = merge(squares[i], squares[j]);

                    // Shift elements to remove squares[j]
                    for (int k = j; k < n - 1; k++) {
                        squares[k] = squares[k + 1];
                    }
                    n--; // Reduce count of squares
                    merged = 1; // A merge has occurred
                    j--; // Check the same j again since it's now the next square
                }
            }
        }
    }
    return n; // Return the new number of squares
}

void removeIslands(unsigned char (*gs_image)[BMP_HEIGTH], struct coordinate locations[], char * count) {
    char temp = 0;
    char checkCount = 0;
    Square sqs[*count];
    for (int i = 0; i < *count; i++) {
        struct coordinate top = {max(locations[i].x - (HALF_AREA + 10), 0),
                            max(locations[i].y - (HALF_AREA + 10), 0)};
        struct coordinate btm = {min(locations[i].x + (HALF_AREA + 10), BMP_WIDTH),
                                    min(locations[i].y + (HALF_AREA + 10), BMP_WIDTH)};
        Square t = {top, btm};
        sqs[i] = t;
    }
    checkCount = removeOverlaps(sqs, *count);
    printf("%d \n", checkCount);
    for (int i = 0; i < checkCount; i++) {
        for (int j = 0; j < checkCount; j++) {
            if (j == i) {
                continue;
            }
            if(hasOverlap(sqs[i], sqs[j])) {
                printf("overlap");
            }
        }
        printf("%d %d %d %d \n", sqs[i].top.x, sqs[i].top.y, sqs[i].btm.x, sqs[i].btm.y);
    }


    for (int i = 0; i < checkCount; i++) {
        int dims[] = {sqs[i].btm.x, sqs[i].btm.y};
        int offset[] = {sqs[i].top.x +1,sqs[i].top.y +1};
        for (int j = 0; j < 4; j++) {
            erodeImage(gs_image, &temp, dims, offset);
        }
    }
    /*
    int dims[] = {BMP_WIDTH, BMP_HEIGTH};
    int offset[] = {1,1};
    for (int i = 0; i<4; i++) {
        erodeImage(gs_image, &temp, dims, offset);
    }*/

    char area = HALF_AREA-5;
    for (int x = area-1; x < BMP_WIDTH - area; x++)
    {
        for (int y = area-1; y < BMP_HEIGTH - area; y++)
        {
            if (exclusionFrame(gs_image, &x, &y, area)) {
                continue;
            }
            for (int k = (-area) + 2; k < area; k++) {
                for (int j = (-area) + 2; j < area; j++) {
                    gs_image[x + k][y + j] = 0;
                }
            }
        }
    }


}

void splitCells(unsigned char (*gs_image)[BMP_HEIGTH]){
  //  char numberOfWhites = 0;
    char onlyWhite = 1;
    char area = HALF_AREA+2;
    struct coordinate locations[300];
    char count = 0;
    for (int x = area-1; x < BMP_WIDTH - area; x++)
    {
        for (int y = area-1; y < BMP_HEIGTH - area; y++)
        {
          //  numberOfWhites = 0;
            onlyWhite = 1;
            for (int k = (-area) + 2; k < area; k++) {
                for (int j = (-area) + 2; j < area; j++) {
                    if (gs_image[x + k][y + j] == 0) {
                        //numberOfWhites++;
                        onlyWhite = 0;
                    }
                }
            }
            if (onlyWhite) {
                struct coordinate loc = {x, y};
                locations[count] = loc;
                count++;
                splitCellsExclusionFrame(gs_image, &x, &y);
            }
        }
    }
    removeIslands(gs_image, locations, &count);


}


