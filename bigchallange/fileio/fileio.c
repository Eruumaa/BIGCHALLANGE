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
    printf("| 2. Show Top-N Words (Table View)       |\n");
    printf("| 3. Exit                                |\n");
    printf("==========================================\n");
    printf("Pilihan Anda >> ");
    scanf("%d", &choice);
    return choice;
}

void saveToBinary(AlphabetGroup data[], const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        printf("[ERROR] Gagal membuat file binary.\n");
        return;
    }

    for (int i = 0; i < 26; i++) {
        // Tulis jumlah kata
        fwrite(&data[i].count, sizeof(int), 1, fp);
        
        // Tulis array struct jika ada isinya
        if (data[i].count > 0) {
            fwrite(data[i].entries, sizeof(WordEntry), data[i].count, fp);
        }
    }

    fclose(fp);
    printf("[IO] Data berhasil disimpan ke '%s'.\n", filename);
}

// --- FUNGSI TAMPILAN TABEL RAPI ---
void readBinaryAndShow(const char *filename, int n) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("[ERROR] File binary belum ada. Jalankan menu 1 dulu!\n");
        return;
    }

    printf("\n");
    printf("TAMPILAN DATA (%d Kata Teratas per Abjad)\n", n);
    
    // Header Tabel dengan Garis Ganda agar terlihat tegas
    printf("========================================================\n");
    printf("| %-5s | %-30s | %-12s |\n", "ABJAD", "KATA", "FREKUENSI");
    printf("========================================================\n");

    int count;
    WordEntry *tempBuffer;
    int dataFound = 0; 

    for (int i = 0; i < 26; i++) {
        fread(&count, sizeof(int), 1, fp);

        if (count > 0) {
            dataFound = 1;
            tempBuffer = (WordEntry *)malloc(count * sizeof(WordEntry));
            fread(tempBuffer, sizeof(WordEntry), count, fp);

            // Batasi tampilan sebanyak N atau sebanyak data yang ada
            int limit = (count < n) ? count : n;
            
            for (int j = 0; j < limit; j++) {
                // Kolom Abjad: Hanya ditampilkan di baris pertama tiap grup huruf
                // Jika baris ke-0, tampilkan huruf (misal 'A'). Jika tidak, tampilkan spasi.
                char abjadDisplay = (j == 0) ? ('A' + i) : ' '; 
                
                // FORMAT PRINTF TABEL:
                // %-5c   : Karakter rata kiri (tapi karena kita pakai 1 huruf, posisinya diatur manual agar center)
                // %-30s  : String rata kiri (lebar 30 char) untuk Kata
                // %12d   : Angka rata kanan (lebar 12 digit) untuk Frekuensi
                
                printf("|   %c   | %-30s | %12d |\n", abjadDisplay, tempBuffer[j].word, tempBuffer[j].frequency);
            }
            
            // Garis pemisah antar huruf (single line)
            printf("|-------+--------------------------------+--------------|\n");

            free(tempBuffer);
        }
    }

    if (!dataFound) {
        printf("|       TIDAK ADA DATA DITEMUKAN                       |\n");
    }
    
    // Penutup Tabel (Garis Ganda)
    printf("=========================================================\n");

    fclose(fp);
}