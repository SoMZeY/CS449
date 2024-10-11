#ifndef COMMANDS_H
#define COMMANDS_H

#include "string.h"
#include "stdlib.h"
#include "stdio.h"

#include "headerStructs.h"

uint16_t infoCommand(char *argv[]);
uint16_t revealCommand(char *argv[]);
uint16_t hideCommand(char *argv[]);

// Helper functions
uint16_t validateBMPFile(FILE *file, BMPFileHeader *bmpFile, BMPDIBHeader *dib);

#endif