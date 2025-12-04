#ifndef FILEIO_H
#define FILEIO_H

#include <stdio.h>
#include <stdlib.h>
#include "../process/process.h"

int showMenu();
void saveToBinary(AlphabetGroup data[], const char *filename);
void readBinaryAndShow(const char *filename, int n);
void clearScreen();
void waitUser();

#endif