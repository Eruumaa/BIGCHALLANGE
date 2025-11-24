/* file: fileio/fileio.c */
#include "fileio.h"

// --- FUNGSI UTAMA IO ---

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

    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n'); 
        return 0; 
    }

    return choice;
}

// FUNGSI SAVE (TETAP SAMA)
void saveToBinary(AlphabetGroup data[], const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        printf("[ERROR] Gagal membuat file binary.\n");
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
    printf("[IO] Data tersimpan di '%s'.\n", filename);
}

// FUNGSI READ (REVISI LAYOUT: JUMLAH DATA)
void readBinaryAndShow(const char *filename, int n) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("[ERROR] File binary belum ada. Jalankan menu 1 dulu!\n");
        return;
    }

    printf("\n");
    printf("TAMPILAN DATA LENGKAP (%d Kata Teratas per Abjad)\n", n);
    
    // Header Tabel
    // Lebar Kolom: Abjad(5), Jumlah Data(11), Panjang(7), Kata(30), Frekuensi(11)
    // Total Lebar Garis = 5+11+7+30+11 + (3*4) + 2 = ~78 karakter
    printf("====================================================================================\n");
    printf("| %-5s | %-11s | %-7s | %-30s | %-11s |\n", 
           "ABJAD", "JUMLAH DATA", "PANJANG", "KATA", "FREKUENSI");
    printf("====================================================================================\n");

    int totalData = 0; 

    for (int i = 0; i < 26; i++) {
        char abjad;
        int count;

        if (fread(&abjad, sizeof(char), 1, fp) != 1) break;
        fread(&count, sizeof(int), 1, fp);

        char abjadDisplay = abjad - 32; // Uppercase

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
                        // Baris Pertama: Cetak Abjad & JUMLAH DATA
                        // %-11d menyesuaikan dengan lebar header "JUMLAH DATA"
                        printf("|   %c   | %-11d ", abjadDisplay, count);
                    } else {
                        // Baris Selanjutnya: Kosongkan kolom (11 spasi)
                        printf("|       |             "); 
                    }

                    // Cetak Sisa Data
                    printf("| %-7d | %-30s | %11d |\n", len, buffer, freq);
                    printedCount++;
                }
            }
        } 
        else {
            // Jika data kosong
            printf("|   %c   | %-11d | %-7s | %-30s | %-11s |\n", 
                   abjadDisplay, 0, "-", "-", "-");
        }

        // Garis Pemisah (Disesuaikan dengan lebar kolom baru)
        printf("|-------+-------------+---------+--------------------------------+-------------|\n");
    }

    if (!totalData) {
        printf("|                     FILE KOSONG / TIDAK ADA DATA                             |\n");
    }
    
    printf("====================================================================================\n");
    fclose(fp);
}