#include <stdio.h>
#include <stdlib.h>
#include "fileio/fileio.h"
#include "process/process.h"

AlphabetGroup words[26];

int main () {

    clearMemory(words);
    
    int choice ;

    while (1) {
        clearScreen();

        choice = showMenu();

        if (choice == 1) {
            char filename[50];
            clearMemory(words);
            printf("Masukkan nama file input (txt): ");
            scanf("%s", filename);

            FILE *read = fopen (filename, "r");
            if (read == NULL) {
                printf("[ERROR] File '%s' tidak ditemukan!\n", filename);
                waitUser();
                continue;
            }
            fclose(read);

            clearMemory(words);
            processTextFile(filename, words);
            sortingData(words);
            saveToBinary(words, "Data.bin");
            waitUser();
        } else if (choice == 2) {
            int n;
            FILE *checkFile = fopen ("Data.bin", "rb");

            if (checkFile == NULL) {
                printf("[INFO] File binary belum ada. Memproses data langsung...\n");
                clearMemory(words);
                if (processTextFile("Data-50K.txt", words) == 1) {
                    waitUser();
                    continue;
                }
                sortingData(words);
                saveToBinary(words, "Data.bin");
                printf("[INFO] Data berhasil diproses dan disimpan.\n");
            } else {
                fclose(checkFile);
            }

            printf("Masukkan nilai n (0 < n <= 25): ");
            if (scanf("%d", &n) == 1) {
                while (getchar() != '\n');
                if (n > 0 && n <= 25) {
                    readBinaryAndShow("Data.bin", n);
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