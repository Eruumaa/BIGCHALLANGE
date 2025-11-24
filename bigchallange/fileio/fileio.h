/* file: fileio/fileio.h */
#ifndef FILEIO_H
#define FILEIO_H

#include <stdio.h>
#include <stdlib.h>

// Include process.h agar kenal struct 'AlphabetGroup'
#include "../process/process.h"

// --- PROTOTYPE FUNGSI IO ---
int showMenu();
void saveToBinary(AlphabetGroup data[], const char *filename);
void readBinaryAndShow(const char *filename, int n);

// [BARU] Fungsi Utilitas Tampilan
void clearScreen();
void waitUser();

#endif