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

// FUNGSI READ (REVISI: TAMPILKAN SEMUA ABJAD A-Z)
void readBinaryAndShow(const char *filename, int n) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("[ERROR] File binary belum ada. Jalankan menu 1 dulu!\n");
        return;
    }

    printf("\n");
    printf("TAMPILAN DATA LENGKAP (%d Kata Teratas per Abjad)\n", n);
    
    printf("================================================================================\n");
    printf("| %-5s | %-8s | %-7s | %-30s | %-11s |\n", 
           "ABJAD", "JML DATA", "PANJANG", "KATA", "FREKUENSI");
    printf("================================================================================\n");

    int totalData = 0; // Cek apakah file kosong total

    // Loop A-Z (Wajib iterasi 26 kali)
    for (int i = 0; i < 26; i++) {
        char abjad;
        int count;

        // Baca Header: [Abjad] & [Jumlah]
        if (fread(&abjad, sizeof(char), 1, fp) != 1) break;
        fread(&count, sizeof(int), 1, fp);

        // Siapkan tampilan huruf kapital (a -> A)
        char abjadDisplay = abjad - 32; 

        // KASUS 1: ADA DATA (Count > 0)
        if (count > 0) {
            totalData = 1;
            int printedCount = 0;

            for (int j = 0; j < count; j++) {
                int len;
                int freq;
                char buffer[256]; 

                // Baca detail kata
                fread(&len, sizeof(int), 1, fp);
                if (len >= 256) len = 255; 
                fread(buffer, sizeof(char), len, fp);
                buffer[len] = '\0'; 
                fread(&freq, sizeof(int), 1, fp);

                // Tampilkan sesuai batas N
                if (printedCount < n) {
                    if (printedCount == 0) {
                        // Baris Pertama: Tampilkan Abjad & Jumlah
                        printf("|   %c   | %-8d ", abjadDisplay, count);
                    } else {
                        // Baris Selanjutnya: Kosongkan kolom Abjad & Jumlah
                        printf("|       |          "); 
                    }

                    printf("| %-7d | %-30s | %11d |\n", len, buffer, freq);
                    printedCount++;
                }
            }
        } 
        // KASUS 2: TIDAK ADA DATA (Count == 0) -> Tampilkan Baris Kosong
        else {
            printf("|   %c   | %-8d | %-7s | %-30s | %-11s |\n", 
                   abjadDisplay, 0, "-", "-", "-");
        }

        // Garis Pemisah (Selalu cetak setiap ganti huruf)
        printf("|-------+----------+---------+--------------------------------+-------------|\n");
    }

    if (!totalData) {
        printf("|                 FILE KOSONG / TIDAK ADA DATA                              |\n");
        printf("================================================================================\n");
    } else {
        printf("================================================================================\n");
    }

    fclose(fp);
}