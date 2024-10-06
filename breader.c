#include "breader.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


int min(int a, int b) {
    return a > b ? b : a;
}
int max(int a, int b) {
    return a > b ? a : b;
}

void outputHelper(FILE* bmp_output, coordinate centers[], int16_t cellCount, int8_t pixelDataOffset);
void read_bmp(unsigned char* gs_arr, char path[]) { // 0
    FILE* bmp = fopen(path, "rb"); // 8
    if (bmp == NULL) {
        throw_error("Could not read the file");
    }

    int8_t pixelDataOffset;
    fseek(bmp, 10, SEEK_SET);
    fread(&pixelDataOffset, sizeof(int8_t), 1, bmp);

    int16_t width, height;
    fseek(bmp, 18, SEEK_SET);
    fread(&width, sizeof(int16_t), 1, bmp);
    fseek(bmp, 22, SEEK_SET);
    fread(&height, sizeof(int16_t), 1, bmp);

    if (width != BMP_WIDTH || height != BMP_HEIGTH) {
        throw_error("Height and width must be 950 px");
    }

    int8_t bytesPerPixel = 3;

    fseek(bmp, pixelDataOffset, SEEK_SET);

    int16_t rowSize = (width * bytesPerPixel + 3) & (~3);

    unsigned char whiteness; // 17

    for (int16_t y = 0; y < height; y++) {
        for (int16_t x = 0; x < width; x++) {

            if (!x || !(949-y) || x == width-1 || (949-y) == height-1) {
                CLEAR_BIT(gs_arr, x, (949-y));
                fseek(bmp, 3, SEEK_CUR);
                continue;
            }

            fread(&whiteness, sizeof(unsigned char), 1, bmp); // 29

            if (whiteness > 90) {
                SET_BIT(gs_arr, x, (949-y));
            }
            fseek(bmp, 2, SEEK_CUR);
        }
        fseek(bmp, rowSize - (width * bytesPerPixel), SEEK_CUR);
    }
    fclose(bmp);
}

void write_bmp (unsigned char* gs_arr, char input_path[], char output_path[], coordinate centers[], int16_t cellCount) {
    FILE* bmp_input = fopen(input_path, "rb");
    if (bmp_input == NULL) {
        throw_error("Could not read the input file");
    }
    FILE* bmp_output = fopen(output_path, "wb");
    if (bmp_output == NULL) {
        fclose(bmp_input);
        throw_error("Could not read the output file");
    }

    unsigned char* header = (unsigned char*)calloc(54, sizeof(unsigned char)); // 55 + 17 = 72
    fread(header, sizeof(unsigned char), 54, bmp_input);
    fwrite(header, sizeof(unsigned char), 54, bmp_output);
    free(header);

    int8_t pixelDataOffset;
    fseek(bmp_input, 10, SEEK_SET);
    fread(&pixelDataOffset, sizeof(int8_t), 1, bmp_input);

    int16_t width, height;
    fseek(bmp_input, 18, SEEK_SET);
    fread(&width, sizeof(int16_t), 1, bmp_input);
    fseek(bmp_input, 22, SEEK_SET);
    fread(&height, sizeof(int16_t), 1, bmp_input);

    if (width != BMP_WIDTH || height != BMP_HEIGTH) {
        fclose(bmp_input);
        fclose(bmp_output);
        throw_error("Height and width must be 950 px");
    }

    int8_t bytesPerPixel = 3;


    fseek(bmp_input, pixelDataOffset, SEEK_SET);
    fseek(bmp_output, pixelDataOffset, SEEK_SET);
    int16_t rowSize = (width * bytesPerPixel + 3) & (~3);
    int16_t paddingSize = rowSize - (width * bytesPerPixel);

    unsigned char pixel[3];  // Buffer for a single pixel (RGB)

    // Loop through each row and pixel - 30 2 pointer 4 ints + 2 chars + 1 chararray
    for (int16_t y = 0; y < height; y++) {
        for (int16_t x = 0; x < width; x++) {
            fread(pixel, sizeof(unsigned char), 3, bmp_input);
            fwrite(pixel, sizeof(unsigned char), 3, bmp_output);  // 90
        }
        fseek(bmp_input, paddingSize, SEEK_CUR);

        for (int16_t p = 0; p < paddingSize; p++) {
            fputc(0x00, bmp_output);
        }
    }
    outputHelper(bmp_output, centers, cellCount, pixelDataOffset); // 259 + 32 = 291

    fclose(bmp_input);
    fclose(bmp_output);
}
typedef struct {
    int8_t x;
    int8_t y;
} lionCoord;

void set_color(FILE* bmp_output, lionCoord coords[], uint8_t size, char r, char g, char b, int16_t offsetX, int16_t offsetY, int8_t pixelDataOffset) {
    // 7
    int8_t bytesPerPixel = 3;
    int16_t rowSize = (BMP_WIDTH * bytesPerPixel + 3) & (~3);

    // Bitmap uses BGR for some reason
    unsigned char pixel[] = {b, g, r} ; // 14
    for (int16_t i = 0; i < size; i++) {
        int pixelOffset = pixelDataOffset + ((BMP_HEIGTH - 1 - (coords[i].y + offsetY)) * rowSize) + ((coords[i].x + offsetX) * bytesPerPixel);
        fseek(bmp_output, pixelOffset, SEEK_SET);
        fwrite(pixel, sizeof(unsigned char), 3, bmp_output);
    }
}

void outputHelper(FILE* bmp_output, coordinate centers[], int16_t cellCount, int8_t pixelDataOffset){

    lionCoord red[] = {
        {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}, {0, 8},
        {1, 2}, {2, 1}, {3, 1}, {4, 0}, {5, 0}, {6, 0},
        {7, 0}, {8, 1}, {9, 1}, {10, 2}, {11, 3}, {12, 4},
        {12, 5}, {13, 6}, {14, 7}, {14, 9}, {13, 11}, {12, 12},
        {11, 13}, {11, 14}, {11, 15}, {11, 16}, {10, 17}
    };
    lionCoord blue[] = {
        {3, 9}, {4, 9}, {5, 8}, {6, 8}, {7, 8}, {4, 11}, {5, 11},
        {6, 10}, {7, 10}, {13, 8}, {14, 8}, {15, 8}, {16, 9}, {17, 9},
        {13, 10}, {14, 10}, {15, 11}, {16, 11}
    };

    lionCoord black[] = {
        {8, 5}, {8, 6}, {9, 8}, {9, 10}, {10, 5}, {10, 6}, {10, 8},
        {10, 9}, {11, 8}, {11, 10}
    };
    printf("size of ars %llu\n", sizeof(red)); //228 + 3 = 231
    fseek(bmp_output, 0, SEEK_SET);
    coordinate corner;
    for (int16_t j = 0; j < (cellCount); j++) { // 237
        corner.x = max(centers[j].x - 9, 0) + 3;
        corner.y = max(centers[j].y - 9, 0) + 3;
        set_color(bmp_output, red, sizeof(red) / sizeof(red[0]), 255, 0, 0, corner.x, corner.y, pixelDataOffset); // 237 + 22 = 259
        set_color(bmp_output, blue, sizeof(blue) / sizeof(blue[0]), 0, 0, 255, corner.x, corner.y,  pixelDataOffset);
        set_color(bmp_output, black, sizeof(black) / sizeof(black[0]), 0, 0, 0, corner.x, corner.y,  pixelDataOffset);
        //set_color(input_image, centers, *cellCount, 0, 255, 0, 0, 0);
    }
}

void write_gs_bmp (unsigned char* gs_arr, char input_path[], char output_path[], coordinate centers[], int16_t cellCount) {
    FILE* bmp_input = fopen(input_path, "rb");
    if (bmp_input == NULL) {
        throw_error("Could not read the input file");
    }
    FILE* bmp_output = fopen(output_path, "wb");
    if (bmp_output == NULL) {
        fclose(bmp_input);
        throw_error("Could not read the output file");
    }

    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, bmp_input);
    fwrite(header, sizeof(unsigned char), 54, bmp_output);


    unsigned int pixelDataOffset;
    fseek(bmp_input, 10, SEEK_SET);
    fread(&pixelDataOffset, sizeof(unsigned int), 1, bmp_input);

    int width, height;
    fseek(bmp_input, 18, SEEK_SET);
    fread(&width, sizeof(int), 1, bmp_input);
    fread(&height, sizeof(int), 1, bmp_input);

    if (width != BMP_WIDTH || height != BMP_HEIGTH) {
        fclose(bmp_input);
        fclose(bmp_output);
        throw_error("Height and width must be 950 px");
    }

    int bytesPerPixel = 3;

    fseek(bmp_input, pixelDataOffset, SEEK_SET);
    fseek(bmp_output, pixelDataOffset, SEEK_SET);
    int rowSize = (width * bytesPerPixel + 3) & (~3);
    int paddingSize = rowSize - (width * bytesPerPixel);

    unsigned char pixel[3];  // Buffer for a single pixel (RGB)

    // Loop through each row and pixel
    for (int16_t y = 0; y < height; y++) {
        for (int16_t x = 0; x < width; x++) {
            if (GET_BIT(gs_arr, x, (949 - y) )) {
                pixel[0] = 255;
                pixel[1] = 255;
                pixel[2] = 255;
            } else {
                pixel[0] = 0;
                pixel[1] = 0;
                pixel[2] = 0;
            }
            fwrite(pixel, sizeof(unsigned char), 3, bmp_output);
        }
        fseek(bmp_input, paddingSize, SEEK_CUR);

        for (int16_t p = 0; p < paddingSize; p++) {
            fputc(0x00, bmp_output);
        }
    }
    fclose(bmp_input);
    fclose(bmp_output);
}

void throw_error(char* message)
{
    fprintf(stderr, "%s\n", message);
    exit(1);
}