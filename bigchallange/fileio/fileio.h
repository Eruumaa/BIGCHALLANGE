//Fileio.h adalah header file untuk modul file input/output, di file ini hanya mendefinisikan fungsi-fungsi input/output bukan proses kerjanya, implementasinya ada di fileio.c
#ifndef FILEIO_H
#define FILEIO_H

#include "../process/process.h"

void read_text_file(const char *filename, AlphabetData alphabet_list[]);
void save_to_binary(const char *filename, AlphabetData alphabet_list[]);
void read_from_binary(const char *filename, AlphabetData alphabet_list[]);

#endif
