//To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
//To run (linux/mac): ./main.out example.bmp example_inv.bmp

//To compile (win): gcc cbmp.c dc.c main.c -o main.exe -std=c99
//To run (win): main.exe example.bmp example_inv.bmp

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"
#include "dc.h"





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

    char input_path[] = "samples/impossible/1impossible.bmp";
    char output_path[] = "output/output.bmp";
    char gs_output_path[] = "output/output_gs.bmp";
    coordinate center[1000];


    read_bitmap(input_path, input_image);
    //greyscale image

    greyscale(input_image, gs_image);

    //outputGSImage(gs_image, gs_output_path);


    removeCells(gs_image, 20);
    //erode image

    outputGSImage(gs_image, gs_output_path);

    int cellCount = 0;

    for (int i = 25; i > 0; i--) {
        erodeImage(gs_image);
        detectCells(gs_image, &cellCount, center);
    }

    printf("%d", cellCount);
    //Save image to file
    outputImage(input_image, output_path, center, &cellCount);

    printf("Done!\n");
    return 0;
}