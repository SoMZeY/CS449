#pragma once

#include "stdint.h"

#pragma pack(1)

typedef struct
{
    uint16_t format;
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t pixelOffset;
} BMPFileHeader;

typedef struct
{
    uint32_t headerSize;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitsPerPixel;
    uint32_t compression;
    uint32_t imageSize;
    int32_t xResolution;
    int32_t yResolution;
    uint32_t colorsUsed;
    uint32_t importantColors;
} BMPDIBHeader;

#pragma pack() // Maybe this should be #pragma pack() to reset to default? But im just gonna follow the hints section