#include "fileio.h"

// Fungsi utama input / output

int showMenu() {
    int choice;
    
    printf("\n");
    printf("==========================================\n");
    printf("|      BIG CHALLENGE WORD COUNTER        |\n");
    printf("==========================================\n");
    printf("| 1. Load Data, Sort & Save to Binary    |\n");
    printf("| 2. Show Top-N Words (Detail Table)     |\n");
    printf("| 3. Exit                                |\n");
    printf("==========================================\n");
    printf("Pilihan Anda >> ");

    // Anti-Crash jika user input huruf
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n'); // Bersihkan buffer
        return 0; 
    }

    return choice;
}