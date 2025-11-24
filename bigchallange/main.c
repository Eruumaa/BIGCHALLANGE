/* file: main.c */
#include <stdio.h>
#include <stdlib.h>
#include "process/process.h"
#include "fileio/fileio.h"

int main() {
    AlphabetGroup dictionary[26];
    
    // Inisialisasi memori
    initMemory(dictionary);

    int choice;
    while (1) {
        choice = showMenu();

        if (choice == 1) {
            // PERUBAHAN DI SINI: Menambahkan folder "data/"
            processTextFile("data/Data-50K.txt", dictionary);
            
            sortAllData(dictionary);
            saveToBinary(dictionary, "dictionary.bin");
            
        } else if (choice == 2) {
            int n;
            printf("Masukkan nilai n (jumlah kata): ");
            scanf("%d", &n);
            readBinaryAndShow("dictionary.bin", n);

        } else if (choice == 3) {
            printf("Exiting... Ganbatte Akil!\n");
            break;
        } else {
            printf("Pilihan tidak valid.\n");
        }
    }
    return 0;
}