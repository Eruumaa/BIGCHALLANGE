/* file: fileio/fileio.h */
#ifndef FILEIO_H
#define FILEIO_H

#include <stdio.h>
#include <stdlib.h>

// Kita include process.h agar fileio kenal struct 'AlphabetGroup'
// Menggunakan "../" untuk mundur satu folder lalu masuk ke process
#include "../process/process.h"

// --- PROTOTYPE FUNGSI IO ---
int showMenu();
void saveToBinary(AlphabetGroup data[], const char *filename);
void readBinaryAndShow(const char *filename, int n);

#endif