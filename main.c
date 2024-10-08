//To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
//To run (linux/mac): ./main.out example.bmp example_inv.bmp

//To compile (win): gcc cbmp.c dc.c main.c -o main.exe -std=c99
//To run (win): main.exe example.bmp example_inv.bmp

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"
#include "dc.h"
#include <time.h>

#ifdef ENABLE_TIMING
    #define START_TIMER clock_t start = clock();
    #define END_TIMER \
    clock_t end = clock(); \
    printf("Time taken: %f ms\n", (double)(end - start) * 1000.0 / CLOCKS_PER_SEC);
#else
    #define START_TIMER
    #define END_TIMER
#endif

#include <string.h>

//Declaring the array to store the image (unsigned char = unsigned 8 bit)
unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char gs_image[BMP_WIDTH][BMP_HEIGTH];

//Main function
int main(/*int argc, char** argv*/) {

    /*
    //argc counts how may arguments are passed
    //argv[0] is a string with the name of the program
    //argv[1] is the first command line argument (input image)
    //argv[2] is the second command line argument (output image)

    //Checking that 2 arguments are passed
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <output file path> <output file path>\n", argv[0]);
        exit(1);
    }

    printf("Example program - 02132 - A1\n");*/


    //Load image from file

   // char input_path[] = "samples/easy/3easy.bmp";
   // char input_path[] = "samples/impossible/1impossible.bmp";
//   char input_path[] = "samples/medium/1medium.bmp";
    char input_path[] = "samples/easy/3easy.bmp";
    char output_path[] = "output/output.bmp";
    char gs_output_path1[] = "output/output_gs1.bmp";
    char gs_output_path2[] = "output/output_gs2.bmp";
    for (int i = 2; i < 3; i++) {
        coordinate center[1000];
        char str1[100] = "samples/impossible/";
        char str2[20];
        sprintf(str2, "%d", i+1);
        strcat(str1, str2);
        strcat(str1, "impossible.bmp");
        char out1[100] = "output/output";
        char out2[20];
        sprintf(out2, "%d", i+1);
        strcat(out1, out2);
        strcat(out1, ".bmp");

        read_bitmap( str1, input_image);
        //greyscale image
        //START_TIMER
        greyscale(input_image, gs_image);
        //END_TIMER
        //outputGSImage(gs_image, gs_output_path);

        //erode image
        int cellCount = 0;
        char done = 0;
        detectCells(gs_image, &cellCount, center, HALF_AREA+6);
        splitCells(gs_image, gs_output_path1);
        outputGSImage(gs_image, gs_output_path2);

        while (!done) {
            erodeImage(gs_image, &done);
            detectCells(gs_image, &cellCount, center, HALF_AREA);
        }
        printf("%d", cellCount);
        //Save image to file
        outputImage(input_image, out1, center, &cellCount);
        printf(" Done!\n");
    }
    return 0;
}