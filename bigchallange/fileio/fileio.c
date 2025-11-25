#include "fileio.h"

// Membersihkan layar (Cross-Platform)
void clearScreen() {
    #ifdef _WIN32
        system("cls");   // Perintah untuk Windows
    #else
        system("clear"); // Perintah untuk Linux/Mac
    #endif
}

// System pause
void waitUser() {
    printf("\nTekan [ENTER] untuk kembali ke menu utama...");
    // Menunggu user menekan Enter sekali
    getchar(); 
}

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
    While (getchar() != '\n');

    return choice;
}

// Fungsi save
void saveToBinary(AlphabetGroup data[], const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        printf("Gagal membuat file binary.\n");
        return;
    }

    // Loop dari huruf 'a' sampai 'z'
    for (int i = 0; i < 26; i++) {
        char abjad = 'a' + i;
        int count = data[i].count;

        // Simpan huruf dan jumlah katanya
        fwrite(&abjad, sizeof(char), 1, fp);
        fwrite(&count, sizeof(int), 1, fp);

        // Simpan semua kata yang dimulai dari huruf ini
        for (int j = 0; j < count; j++) {
            char *wordPtr = data[i].entries[j].word;
            int len = strlen(wordPtr);
            int freq = data[i].entries[j].frequency;
            
            // Simpan panjang kata, isi kata, dan frekuensinya
            fwrite(&len, sizeof(int), 1, fp);
            fwrite(wordPtr, sizeof(char), len, fp);
            fwrite(&freq, sizeof(int), 1, fp);
        }
    }

    fclose(fp);
    printf("Data tersimpan di '%s'.\n", filename);
}