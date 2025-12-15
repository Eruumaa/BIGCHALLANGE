#include <stdio.h>
#include <stdlib.h>
#include "fileio/fileio.h"
#include "process/process.h"

AlphabetGroup dictionary[26];

int main () {

    fixMemory(dictionary);
    
    int choice ;

    while (1) {
        clearScreen();

        choice = showMenu();

        if (choice == 1) {
            char filename[50];
            fixMemory(dictionary);
            printf("\nMasukkan nama file input (txt): ");
            scanf("%s", filename);

            FILE *read = fopen (filename, "r");
            if (read == NULL) {
                printf("[ERROR] File '%s' tidak ditemukan!\n", filename);
                waitUser();
                continue;
            }
            fclose(read);

            fixMemory(dictionary);
            processTextFile(filename, dictionary);
            sortingData(dictionary);
            saveToBinary(dictionary, "output.bin");
            waitUser();
        } else if (choice == 2) {
            FILE *checkFile = fopen ("output.bin", "rb");

            if (checkFile == NULL) {
                printf("[INFO] File binary belum ada. Memproses data langsung...\n");
                fixMemory(dictionary);
                processTextFile("data/Data-50K.txt", dictionary);
                sortingData(dictionary);
                saveToBinary(dictionary, "output.bin");
                printf("[INFO] Data berhasil diproses dan disimpan.\n");
            } else {
                fclose(checkFile);
            }
            int n;
            printf("Masukkan nilai n (0 < n <= 25): ");

            if (scanf("%d", &n) == 1) {
                while (getchar() != '\n');
                if (n > 0 && n <= 25) {
                    readBinaryAndShow("output.bin", n);
                } else {
                    printf("[ERROR] Nilai n harus antara 1 sampai 25!\n\n");
                }
            } else {
                while (getchar() != '\n');
                printf("[ERROR] Input harus angka!\n");
            }
            waitUser();
        } 
        else if (choice == 3) {
            printf("Program Berakhir\n");
            break;
        } else {
            printf("Pilihan tidak valid. Coba lagi.\n");
            waitUser();
        }
    }
    return 0;
}