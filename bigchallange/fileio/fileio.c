#include "fileio.h"

// Membersihkan layar terminal
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear"); 
    #endif
}

void waitUser() {
    // Menahan program sampai user menekan ENTER
    printf("\nTekan [ENTER] untuk kembali ke menu utama...");
    getchar(); 
}

// Menu program
int showMenu() {
    int choice;
    
    printf("\n");
    printf("=================================================\n");
    printf("|             WORD COUNTER PROGRAM              |\n");
    printf("================================================|\n");
    printf("| 1. Baca Data                                  |\n");
    printf("| 2. Menampilkan n kata & Baca Data (jika Belum)|\n");
    printf("| 3. Keluar                                     |\n");
    printf("=================================================\n");
    printf("Pilih >> ");

    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        return 0; 
    }
    while (getchar() != '\n');

    return choice;
}

// Menyimpan seluruh data kata ke file biner
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

// Membaca file biner dan menampilkan maksimal n kata per abjad
void readBinaryAndShow(const char *filename, int n) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("File binary belum ada\n");
        return;
    }
    printf("\n");
    printf("abjad {kata (frekuensi)}\n");
    
    for (int i = 0; i < 26; i++) {
        char abjad;
        int count;

        if (fread(&abjad, sizeof(char), 1, fp) != 1) break;
        fread(&count, sizeof(int), 1, fp);
        
        printf("%c     {", abjad);

        if (count > 0) {
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
                    if (printedCount > 0) {
                        printf(", ");
                        if (printedCount % 5 == 0) {
                            printf("\n       ");
                        }
                    }
                    printf("%s (%d)", buffer, freq);
                    printedCount++; 
                }
            }
        }
        printf("}\n");
    } 
    fclose(fp);
}
