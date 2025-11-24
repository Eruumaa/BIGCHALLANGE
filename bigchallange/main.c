/* file: main.c */
#include <stdio.h>
#include <stdlib.h>
#include "process/process.h"
#include "fileio/fileio.h"

int main() {
    AlphabetGroup dictionary[26];
    
    initMemory(dictionary);

    int choice;
    while (1) {
        // 1. Bersihkan Layar dulu sebelum menampilkan menu
        clearScreen();
        
        // 2. Tampilkan Menu
        choice = showMenu();

        if (choice == 1) {
            // Jalankan proses
            processTextFile("data/Data-50K.txt", dictionary);
            sortAllData(dictionary);
            saveToBinary(dictionary, "dictionary.bin");
            
            // 3. Pause agar user bisa baca pesan "SUCCESS"
            waitUser();
            
        } else if (choice == 2) {
            int n;
            printf("Masukkan nilai n (jumlah kata): ");
            // Validasi input n
            if (scanf("%d", &n) == 1) {
                // Bersihkan buffer lagi setelah scanf n
                while(getchar() != '\n'); 
                readBinaryAndShow("dictionary.bin", n);
            } else {
                while(getchar() != '\n'); // flush
                printf("[ERROR] Input harus angka!\n");
            }

            // 3. Pause agar user bisa baca TABEL
            waitUser();

        } else if (choice == 3) {
            printf("Exiting... Ganbatte Akil!\n");
            break;
        } else {
            printf("Pilihan tidak valid. Coba lagi.\n");
            // Pause sebentar biar user sadar salah input
            waitUser();
        }
    }
    
    // Optional: Free memory code here
    return 0;
}