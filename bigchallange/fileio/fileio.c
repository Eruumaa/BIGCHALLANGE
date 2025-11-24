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

    // Validasi input (Anti-Crash jika user input huruf)
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n'); // Bersihkan buffer
        return 0; 
    }

    return choice;
}

// FUNGSI SAVE (Format: [Abjad][Jml][Len][Kata][Freq])
void saveToBinary(AlphabetGroup data[], const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        printf("[ERROR] Gagal membuat file binary.\n");
        return;
    }

    for (int i = 0; i < 26; i++) {
        char abjad = 'a' + i;
        int count = data[i].count;

        // 1. [Abjad]
        fwrite(&abjad, sizeof(char), 1, fp);
        // 2. [Jumlah Kata di Abjad ini]
        fwrite(&count, sizeof(int), 1, fp);

        for (int j = 0; j < count; j++) {
            char *wordPtr = data[i].entries[j].word;
            int len = strlen(wordPtr);
            int freq = data[i].entries[j].frequency;

            // 3. [Panjang Kata]
            fwrite(&len, sizeof(int), 1, fp);
            // 4. [Kata]
            fwrite(wordPtr, sizeof(char), len, fp);
            // 5. [Frekuensi]
            fwrite(&freq, sizeof(int), 1, fp);
        }
    }

    fclose(fp);
    printf("[IO] Data tersimpan (Format Lengkap 5 Komponen) di '%s'.\n", filename);
}

// FUNGSI READ (Tabel 5 Kolom)
void readBinaryAndShow(const char *filename, int n) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("[ERROR] File binary belum ada. Jalankan menu 1 dulu!\n");
        return;
    }

    printf("\n");
    printf("TAMPILAN DATA LENGKAP (%d Kata Teratas per Abjad)\n", n);
    
    // Header Tabel 5 Kolom
    printf("================================================================================\n");
    printf("| %-5s | %-8s | %-7s | %-30s | %-11s |\n", 
           "ABJAD", "JUMLAH", "PANJANG", "KATA", "FREKUENSI");
    printf("================================================================================\n");

    int dataFound = 0; 

    for (int i = 0; i < 26; i++) {
        char abjad;
        int count;

        // Baca Header Group: [Abjad] & [Jumlah Kata]
        if (fread(&abjad, sizeof(char), 1, fp) != 1) break;
        fread(&count, sizeof(int), 1, fp);

        if (count > 0) {
            dataFound = 1;
            int printedCount = 0;

            for (int j = 0; j < count; j++) {
                int len;
                int freq;
                char buffer[256]; 

                // Baca Detail Kata: [Panjang] -> [Kata] -> [Frekuensi]
                fread(&len, sizeof(int), 1, fp);
                
                if (len >= 256) len = 255; // Safety
                fread(buffer, sizeof(char), len, fp);
                buffer[len] = '\0'; // Null terminator

                fread(&freq, sizeof(int), 1, fp);

                // Tampilkan hanya jika belum mencapai batas N
                if (printedCount < n) {
                    // Kolom Abjad & Jml Data hanya muncul di baris pertama grup
                    char abjadDisplay = (printedCount == 0) ? (abjad - 32) : ' '; // Uppercase
                    
                    // Variabel bantu untuk print Jml Data (hanya baris pertama)
                    // Jika bukan baris pertama, kita cetak string kosong ""
                    // Tapi karena %d butuh int, kita akali dengan logika printf terpisah atau conditional value
                    
                    // Cetak Bagian Kiri (Abjad & Jumlah)
                    if (printedCount == 0) {
                        printf("|   %c   | %-8d ", abjadDisplay, count);
                    } else {
                        printf("|       |          "); // Kosongkan kolom ini
                    }

                    // Cetak Bagian Kanan (Panjang, Kata, Frekuensi)
                    printf("| %-7d | %-30s | %11d |\n", len, buffer, freq);
                    
                    printedCount++;
                }
            }
            
            // Garis pemisah antar abjad
            if (printedCount > 0) {
                printf("|-------+----------+---------+--------------------------------+-------------|\n");
            }
        }
    }

    if (!dataFound) {
        printf("|                 TIDAK ADA DATA DITEMUKAN                                  |\n");
    }
    
    printf("===============================================================================\n");
    fclose(fp);
}