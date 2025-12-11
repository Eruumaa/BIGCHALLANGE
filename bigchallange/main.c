#include <stdio.h>
#include <stdlib.h>
#include "fileio/fileio.h"
#include "process/process.h"

int main () {
    AlphabetGroup dictionary[26];

    initMemory(dictionary);
    
    int choice ;

    while (1) {
        clearScreen();

        choice = showMenu();

        if (choice == 1) {
            processTextFile("data/Data-50k.txt", dictionary);
            sortingData(dictionary);
            saveToBinary(dictionary, "output.bin");

            waitUser();
        } else if (choice == 2) {
            int n;
            printf("Masukkan nilai n: ");

            if (scanf("%d", &n) == 1) {
                while (getchar() != '\n');
                readBinaryAndShow("output.bin", n);
            } else {
                while (getchar() != '\n');
                printf("[ERROR] Input harus angka!\n");
            }
            waitUser();
        } else if (choice == 3) {
            printf("Program Berakhir\n");
            break;
        } else {
            printf("Pilihan tidak valid. Coba lagi.\n");
            waitUser();
        }
    }
    return 0;
}