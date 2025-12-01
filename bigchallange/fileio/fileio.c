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

// Fungsi read
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

        // Membaca header per abjad dari file biner
        if (fread(&abjad, sizeof(char), 1, fp) != 1) break;
        fread(&count, sizeof(int), 1, fp);
        
        // Huruf diubah ke uppercase
        char abjadDisplay = abjad - 32;

        // Jika ada data untuk abjad tersebut (tampilkan data, jumlah kata, dan maksimal n kata teratas)
        if (count > 0) {
            totalData = 1;
            int printedCount = 0;

            // Loop untuk membaca data kata
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

        printf("|-------+-------------+---------+--------------------------------+-------------|\n");
    }

    if (!totalData) {
        printf("|                     FILE KOSONG / TIDAK ADA DATA                             |\n");
    }
    
    printf("================================================================================\n");
    fclose(fp);
}
