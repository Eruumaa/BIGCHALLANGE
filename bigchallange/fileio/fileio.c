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

    // PERBAIKAN DI SINI:
    // Cek apakah scanf berhasil membaca angka (return 1 = sukses)
    if (scanf("%d", &choice) != 1) {
        // Jika user mengetik huruf/simbol, scanf akan gagal (return 0)
        // Kita harus membersihkan "sampah" input tersebut dari buffer
        while (getchar() != '\n'); 
        
        // Return angka acak (misal 0) agar dianggap "Pilihan tidak valid" oleh main()
        return 0; 
    }

    return choice;
}

void saveToBinary(AlphabetGroup data[], const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        printf("[ERROR] Gagal membuat file binary.\n");
        return;
    }

    for (int i = 0; i < 26; i++) {
        fwrite(&data[i].count, sizeof(int), 1, fp);
        if (data[i].count > 0) {
            fwrite(data[i].entries, sizeof(WordEntry), data[i].count, fp);
        }
    }

    fclose(fp);
    printf("[IO] Data berhasil disimpan ke '%s'.\n", filename);
}

// --- FUNGSI TAMPILAN TABEL ---
void readBinaryAndShow(const char *filename, int n) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("[ERROR] File binary belum ada. Jalankan menu 1 dulu!\n");
        return;
    }

    printf("\n");
    printf("TAMPILAN DATA (%d Kata Teratas per Abjad)\n", n);
    
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

            int limit = (count < n) ? count : n;
            
            for (int j = 0; j < limit; j++) {
                char abjadDisplay = (j == 0) ? ('A' + i) : ' '; 
                printf("|   %c   | %-30s | %12d |\n", abjadDisplay, tempBuffer[j].word, tempBuffer[j].frequency);
            }
            printf("|-------+--------------------------------+--------------|\n");
            free(tempBuffer);
        }
    }

    if (!dataFound) {
        printf("|       TIDAK ADA DATA DITEMUKAN                       |\n");
    }
    printf("========================================================\n");

    fclose(fp);
}