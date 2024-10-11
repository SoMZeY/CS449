#include "commands.h"

uint16_t infoCommand(char *argv[])
{
    // Try to open the specified file
    FILE *file = fopen(argv[2], "rb");

    // Check if it was successful
    if (!file)
    {
        printf("ERROR: File not found.\n");
        return 0;
    }

    // Personal debugging
    // for (int i = 0; i < argc; i ++)
    // {
    //     printf("Argument: %s\n", argv[i]);
    // }
    
    // Check the format of the bmp file now
    // Create the object for it, and then fread it, since we used the pragma pack 
    // the data will be perfectly aligned with fields in structs. Thanks for that tip
    BMPFileHeader bmpFile;
    BMPDIBHeader dib;

    // Validate using the created utility function
    uint16_t result = validateBMPFile(file, &bmpFile, &dib);

    // Make sure it passed, otherwise stop the program.
    if (result == 0)
    {
        fclose(file);
        return 0;
    }

    // printf("Success");

    //After the checks passed, we can write the info
    printf("=== BMP Header ===\n");
    printf("Type: BM\n");
    printf("Size: %u\n", bmpFile.fileSize);
    printf("Reserved 1: %u\n", bmpFile.reserved1);
    printf("Reserved 2: %u\n", bmpFile.reserved2);
    printf("Image offset: %u\n", bmpFile.pixelOffset);
    printf("\n=== DIB Header ===\n");
    printf("Size: %u\n", dib.headerSize);
    printf("Width: %d\n", dib.width);
    printf("Height: %d\n", dib.height);
    printf("# color planes: %u\n", dib.planes);
    printf("# bits per pixel: %u\n", dib.bitsPerPixel);
    printf("Compression scheme: %u\n", dib.compression);
    printf("Image size: %u\n", dib.imageSize);
    printf("Horizontal resolution: %d\n", dib.xResolution);
    printf("Vertical resolution: %d\n", dib.yResolution);
    printf("# colors in palette: %u\n", dib.colorsUsed);
    printf("# important colors: %u\n", dib.importantColors);

    // Close the file
    fclose(file);
    return 1;
}

uint16_t revealCommand(char* argv[])
{
    // Open the file in binary format, and allow reading and writing (+helps with that)
    FILE *file = fopen(argv[2], "rb+");

    if (!file)
    {
        printf("ERROR: File not found.\n");
        return 0;
    }

    // Create the header and the bimp variables
    BMPFileHeader bmpFile;
    BMPDIBHeader dib;

    // Validate them using created utility functions 
    uint16_t result = validateBMPFile(file, &bmpFile, &dib);

    // Check for the result and stop running if failed
    if (result == 0)
    {
        fclose(file);
        return 0;
    }

    /*
    * According to the cppreference; If the stream is open in binary mode,
    * the new position is exactly offset bytes measured from the beginning
    * of the file if origin is SEEK_SET
    */

    // So follow the instructions
    fseek(file, bmpFile.pixelOffset, SEEK_SET);

    // Since the bmp needs to be multiple of 4 we need to calculate padding.
    // First calculate the row size in bytes
    int rowSize = dib.width * 3;
    // Calculate how many padding bytes we need to skip
    int padding = (4 - (rowSize % 4)) % 4;

    // Iterate over each pixel
    for (int y = 0; y < dib.height; y++)
    {
        for (int x = 0; x < dib.width; x++)
        {
            // Array in the BGR format. colors[0] == B, colors[1] == G, colors[2] == R
            uint8_t colors[3];
        
            // Read three bytes (representing one pixel) at a time and stores them in the colors array
            uint8_t pixelsRead = fread(colors, sizeof(uint16_t), 3, file);

            // Check if it read exactly 3 pixels, otherwise this can cause problems
            if (pixelsRead != 3)
            {
                printf("ERROR: Failed to read pixel data.\n");
                fclose(file);
                return 0;
            }

            // Perform masking
            for (int i = 0; i < 3; i++)
            {
                /*
                *  Perform upperbit masking and then shift it to the right (colors[i] & 0xF0) >> 4
                *  Perform lowerbit masking and then shift it to the left  (colors[i] & 0x0F) << 4
                *  Or them together to combine them
                *  Do this for each of the colors
                */
                colors[i] = ((colors[i] & 0xF0) >> 4) | ((colors[i] & 0x0F) << 4);
            }

            // As said in the instructions we need to traceback before writing, since its "too smart"
            fseek(file, -3, SEEK_CUR);

            // Write the new colors
            pixelsRead = fwrite(colors, sizeof(uint8_t), 3, file);

            // Needs to be exactly 3 pixels read to ensure correctness
            if (pixelsRead != 3)
            {
                printf("ERROR: Failed to write pixel data.\n");
                fclose(file);
                return 0;
            }
        }
        
        // Skip the padding bytes, and check for success
        if (fseek(file, padding, SEEK_CUR) != 0)
        {
            printf("ERROR: Failed to skip padding.\n");
            fclose(file);
            return 0;
        }
    }

    // Close the file
    fclose(file);
    return 1;
}

uint16_t hideCommand(char *argv[])
{
    return 0;
}

uint16_t validateBMPFile(FILE *file, BMPFileHeader *bmpFile, BMPDIBHeader *dib)
{
    fread(bmpFile, sizeof(BMPFileHeader), 1, file);

    // Check for the parsing error, according to the cppreference, we need to check feof and ferror
    if (feof(file))
    {
        printf("Error reading: unexpected end of file\n");
        return 0;
    }
    else if (ferror(file))
    {
        printf("Error reading\n");
        return 0;
    }

    // Verify that the file is a BMP by checking if type == 'BM'
    if (bmpFile->format != 0x4D42)
    {
        printf("ERROR: The format is not supported.\n");
        return 0;
    }

    // Read DIB Header
    fread(dib, sizeof(BMPDIBHeader), 1, file);

    // Check for the parsing error, according to the cppreference, we need to check feof and ferror
    if (feof(file))
    {
        printf("ERROR: Unexpected end of file while reading DIB header.\n");
        return 0;
    }
    else if (ferror(file))
    {
        printf("ERROR: Error reading DIB header.\n");
        return 0;
    }

    // DIB header needs to be 40 in size
    if (dib->headerSize != 40)
    {
        printf("ERROR: The format is not supported.\n");
        return 0;
    }

    // Verify the image is 24 bits per pixel
    if (dib->bitsPerPixel != 24)
    {
        printf("ERROR: The format is not supported.\n");
        return 0;
    }

    return 1;
}
