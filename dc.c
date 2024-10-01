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
coordinate marked[950*950];

void erodeImage(unsigned char* gs_arr, char* done){
    *done = 1;

    int counter = 0;
    for (int i = 1; i < BMP_WIDTH; i++) {
        for (int j = 1; j < BMP_HEIGTH; j++) {
            if(GET_BIT(gs_arr, i, j) && GET_BIT(gs_arr, i-1, j)  && GET_BIT(gs_arr, i+1, j)  && GET_BIT(gs_arr, i, j-1)  && GET_BIT(gs_arr, i, j+1) ) {
                continue;
            }
            if (!GET_BIT(gs_arr, i, j) ) {
                continue;
            }
            marked[counter].x = i;
            marked[counter].y = j;
            counter++;
            *done = 0;
        }

    }
   for (int i = 0; i < counter; i++) {
       CLEAR_BIT(gs_arr, marked[i].x , marked[i].y);
   }



} //gs = greyscaled.

/*
void set_color(unsigned char* gs_arr, const coordinate* coords, int count, int r, int g, int b, int x_offset, int y_offset) {
    for (int i = 0; i < count; i++) {
        input_image[min(x_offset + coords[i].x, BMP_HEIGTH-1)][min(y_offset + coords[i].y, BMP_HEIGTH-1)][0] = r;
        input_image[min(x_offset + coords[i].x, BMP_HEIGTH-1)][min(y_offset + coords[i].y, BMP_HEIGTH-1)][1] = g;
        input_image[min(x_offset + coords[i].x, BMP_HEIGTH-1)][min(y_offset + coords[i].y, BMP_HEIGTH-1)][2] = b;
    }
}*/
 // Used at the end of detect cells, to output image with crosses
void outputGSImage(unsigned char* gs_arr,
                    char*  output_file_path){

    for (int x = 0; x < BMP_WIDTH; x++) {
        for (int y = 0; y < BMP_WIDTH; y++) {
            for (int c = 0; c < BMP_CHANNELS; c++) {

                output_image[x][y][c] = GET_BIT(gs_arr, x, y) * 255;
            }
        }
    }

    //write_bmp(output_image, output_file_path);

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

char exclusionFrame(unsigned char* gs_arr, int *x, int *y, char area) {

    int x_ = *x - area + 1;
    int y_ = *y - area + 1;
    for (int i = 0;  i < area*2; i++) {
        if (GET_BIT(gs_arr, x_ + i, y_)) {
            return 1;
        }
        if (GET_BIT(gs_arr, x_, y_ + i)) {
            return 1;
        }
        if (GET_BIT(gs_arr, x_ + i,y_ + area*2 -1)) {
            return 1;
        }
        if (GET_BIT(gs_arr, x_ + area*2 - 1,y_ + i)) {
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
                    if (GET_BIT(gs_arr, y + k, x + j) == 1) {
                        whiteDetected = 1;
                    }
                    CLEAR_BIT(gs_arr, y + k, x + j);
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
}
void splitCellsExclusionFrame(unsigned char* gs_arr, int *x, int *y ) {
    int x_ = *x - HALF_AREA + 1;
    int y_ = *y - HALF_AREA + 1;
    for (int i = 0;  i < HALF_AREA*2; i++) {
        if (GET_BIT(gs_arr, x_ + i, y_)) {

            CLEAR_BIT(gs_arr, x_ + i,y_);
        }
        if (GET_BIT(gs_arr, x_,y_ + i)) {
            CLEAR_BIT(gs_arr, x_ ,y_ + i);
        }
        if (GET_BIT(gs_arr, x_ + i, y_ + HALF_AREA*2 -1)) {
            CLEAR_BIT(gs_arr, x_ + i, y_ + HALF_AREA*2 -1);
        }
        if (GET_BIT(gs_arr, x_ + HALF_AREA*2 - 1, y_ + i)) {
            CLEAR_BIT(gs_arr, x_ + HALF_AREA*2 - 1, y_ + i);
        }
    }

}
void removeIslands(unsigned char* gs_arr) {
    char temp = 0;
    for (int i = 0; i<4; i++) {
        erodeImage(gs_arr, &temp);
    }

    char whiteDetected = 0;
    char area = HALF_AREA-5;
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
                    if (GET_BIT(gs_arr, x + k, y + j)) {
                        whiteDetected = 1;
                    }
                    CLEAR_BIT(gs_arr, x + k, y + j);
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
                    if (!GET_BIT(gs_arr, x + k, y + j)) {
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


