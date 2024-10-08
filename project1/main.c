#include "headerStructs.h"
#include "stdlib.h"

int main(int argc, char *argv[])
{
    // We should have atleast 3 arguments
    // argv[0] for the program itself
    // argv[1] for the info so we can just compare strings
    // argv[2] for the image

    if (argc != 3 || strcmp(argv[1], "--info"))
    {
        printf("ERROR: Missing arguments.");
    }

    // for (int i = 0; i < argc, i++) 
    // {
    //     printf();
    // }


    // After the checks passed, we can write the info
    // printf("=== BMP Header ===\n");
    // printf("Type: BM\n");
    // printf("Size: %u\n", fileHeader.fileSize);
    // printf("Reserved 1: %u\n", fileHeader.reserved1);
    // printf("Reserved 2: %u\n", fileHeader.reserved2);
    // printf("Image offset: %u\n", fileHeader.pixelOffset);

    // printf("\n=== DIB Header ===\n");
    // printf("Size: %u\n", dibHeader.headerSize);
    // printf("Width: %d\n", dibHeader.width);
    // printf("Height: %d\n", dibHeader.height);
    // printf("# color planes: %u\n", dibHeader.planes);
    // printf("# bits per pixel: %u\n", dibHeader.bitsPerPixel);
    // printf("Compression scheme: %u\n", dibHeader.compression);
    // printf("Image size: %u\n", dibHeader.imageSize);
    // printf("Horizontal resolution: %d\n", dibHeader.xResolution);
    // printf("Vertical resolution: %d\n", dibHeader.yResolution);
    // printf("# colors in palette: %u\n", dibHeader.colorsUsed);
    // printf("# important colors: %u\n", dibHeader.importantColors);

    // Close the file
    fclose(file);
    return 0;
}