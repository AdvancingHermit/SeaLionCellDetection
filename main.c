//To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
//To run (linux/mac): ./main.out example.bmp example_inv.bmp

//To compile (win): gcc cbmp.c dc.c main.c -o main.exe -std=c99
//To run (win): main.exe example.bmp example_inv.bmp

#include <stdlib.h>
#include <stdio.h>
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
#include "breader.h"
#include "cbmp.h"

unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];


//Main function
int main(/*int argc, char** argv*/) {
    const int n = ((950*950) / 8) + 1;
    unsigned char* gs_arr = (unsigned char*)calloc(n, sizeof(unsigned char));
    if (!gs_arr) {
        printf("Memory allocation failed!\n");
        return 1;
    }

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


    char output_path[] = "output/output.bmp";
    char gs_output_path[] = "output/gs_output.bmp";


    for (int i = 0; i < 1; i++) {
        coordinate center[1000];
        char str1[100] = "samples/impossible/";
        char str2[20];
        sprintf(str2, "%d", i+1);
        strcat(str1, str2);
        strcat(str1, "impossible.bmp");

        //read_bitmap( str1, input_image);
        read_bmp(gs_arr, str1);



        int numWhite = 0;
        printf("Number of white points: %d\n", numWhite);

        int cellCount = 0;
        char done = 0;
        write_gs_bmp(gs_arr, str1, gs_output_path, center, &cellCount);

        detectCells(gs_arr, &cellCount, center, HALF_AREA+6);
        splitCells(gs_arr);
        for (int x = 0; x < 12; x++) {
            erodeImage(gs_arr, &done);
            detectCells(gs_arr, &cellCount, center, HALF_AREA);
        }

        for (int x = 0; x < cellCount; x++) {
            printf("(%d, %d) \n", center[x].x, center[x].y);
        }

        printf("%d", cellCount);


        //Save image to file

        write_bmp(gs_arr, str1, output_path, center, &cellCount);

        printf("Done!\n");
    }

    free(gs_arr);
    return 0;
}