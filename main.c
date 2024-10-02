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

    for (int i = 4; i < 5; i++) {
        coordinate center[305];
        char str1[100] = "samples/medium/";
        char str2[20];
        sprintf(str2, "%d", i+1);
        strcat(str1, str2);
        strcat(str1, "medium.bmp");

        //read_bitmap( str1, input_image);
        read_bmp(gs_arr, str1);

        int cellCount = 0;
        char done = 0;
        write_gs_bmp(gs_arr, str1, gs_output_path, center, &cellCount);

        detectCells(gs_arr, &cellCount, center, HALF_AREA+6);
        splitCells(gs_arr);
        while (!done) {
            erodeImage(gs_arr, &done);
            detectCells(gs_arr, &cellCount, center, HALF_AREA);
        }

        write_bmp(gs_arr, str1, output_path, center, &cellCount);

        printf("%u Done!\n", cellCount);
    }

    free(gs_arr);
    return 0;
}