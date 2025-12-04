#include "fileio.h"

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear"); 
    #endif
}

void waitUser() {
    printf("\nTekan [ENTER] untuk kembali ke menu utama...");
    getchar(); 
}

int showMenu() {
    int choice;
    
    printf("\n");
    printf("==========================================\n");
    printf("|      BIG CHALLENGE WORD COUNTER        |\n");
    printf("==========================================\n");
    printf("| 1. Baca Data                           |\n");
    printf("| 2. Menampilkan data ke-n Kata          |\n");
    printf("| 3. Keluar                              |\n");
    printf("==========================================\n");
    printf("Pilih >> ");


    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        return 0; 
    }
    While (getchar() != '\n');

    return choice;
}

void saveToBinary(AlphabetGroup data[], const char *filename) {
    FILE *fp = fopen(filename, "wb");

    if (!fp) {
        printf("Gagal membuat file binary.\n");
        return;
    }

    for (int i = 0; i < 26; i++) {
        char abjad = 'a' + i;
        int count = data[i].count;
        fwrite(&abjad, sizeof(char), 1, fp);
        fwrite(&count, sizeof(int), 1, fp);

        for (int j = 0; j < count; j++) {
            char *wordPtr = data[i].entries[j].word;
            int len = strlen(wordPtr);
            int freq = data[i].entries[j].frequency;
            fwrite(&len, sizeof(int), 1, fp);
            fwrite(wordPtr, sizeof(char), len, fp);
            fwrite(&freq, sizeof(int), 1, fp);
        }
    }

    fclose(fp);
    printf("Data tersimpan di '%s'.\n", filename);
}

void readBinaryAndShow(const char *filename, int n) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("File binary belum ada. Jalankan menu 1 terlebih dahulu!\n");
        return;
    }

    printf("\n");
    printf("TAMPILAN DATA LENGKAP (%d Kata Teratas per Abjad)\n", n);
    
    printf("================================================================================\n");
    printf("| %-5s | %-11s | %-7s | %-30s | %-11s |\n", 
           "ABJAD", "JUMLAH DATA", "PANJANG", "KATA", "FREKUENSI");
    printf("================================================================================\n");

    int totalData = 0;
    
    for (int i = 0; i < 26; i++) {
        char abjad;
        int count;

        if (fread(&abjad, sizeof(char), 1, fp) != 1) break;
        fread(&count, sizeof(int), 1, fp);
        
        char abjadDisplay = abjad - 32;

        if (count > 0) {
            totalData = 1;
            int printedCount = 0;

            for (int j = 0; j < count; j++) {
                int len;
                int freq;
                char buffer[256]; 

                fread(&len, sizeof(int), 1, fp);
                if (len >= 256) len = 255; 
                fread(buffer, sizeof(char), len, fp);
                buffer[len] = '\0'; 
                fread(&freq, sizeof(int), 1, fp);
                
                if (printedCount < n) {
                    if (printedCount == 0) {
                        printf("|   %c   | %-11d ", abjadDisplay, count);
                    } else {
                        printf("|       |             "); 
                    }
                    printf("| %-7d | %-30s | %11d |\n", len, buffer, freq);
                    printedCount++;
                }
            }
        } 
        else {
            printf("|   %c   | %-11d | %-7s | %-30s | %-11s |\n", 
                   abjadDisplay, 0, "-", "-", "-");
        }

        printf("|-------|-------------|---------|--------------------------------|-------------|\n");
    }

    if (!totalData) {
        printf("|                     FILE KOSONG / TIDAK ADA DATA                             |\n");
    }
    
    printf("================================================================================\n");
    fclose(fp);
}
