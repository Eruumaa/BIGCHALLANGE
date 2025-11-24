/* file: process/process.c */
#include "process.h"

// --- FUNGSI INIT, CLEAN, ADD (TETAP SAMA, TIDAK BERUBAH) ---

void initMemory(AlphabetGroup data[]) {
    for (int i = 0; i < 26; i++) {
        data[i].count = 0;
        data[i].capacity = 10; 
        data[i].entries = (WordEntry *)malloc(data[i].capacity * sizeof(WordEntry));
    }
}

void cleanWord(char *word) {
    int j = 0;
    char temp[100];
    for (int i = 0; word[i] != '\0'; i++) {
        if (isalpha(word[i])) {
            temp[j++] = tolower(word[i]);
        }
    }
    temp[j] = '\0';
    strcpy(word, temp);
}

void addWordToMemory(char *word, AlphabetGroup data[]) {
    if (strlen(word) < 2) return;

    int index = word[0] - 'a'; 
    if (index < 0 || index > 25) return; 

    AlphabetGroup *group = &data[index];

    // Cek duplikat
    for (int i = 0; i < group->count; i++) {
        if (strcmp(group->entries[i].word, word) == 0) {
            group->entries[i].frequency++;
            return;
        }
    }

    // Resize jika penuh
    if (group->count == group->capacity) {
        group->capacity *= 2;
        group->entries = (WordEntry *)realloc(group->entries, group->capacity * sizeof(WordEntry));
    }

    strcpy(group->entries[group->count].word, word);
    group->entries[group->count].frequency = 1;
    group->count++;
}

void processTextFile(const char *filename, AlphabetGroup data[]) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("[ERROR] File tidak ditemukan di: %s\n", filename);
        printf("Pastikan folder 'data' sudah dibuat dan file ada di dalamnya.\n");
        return;
    }

    char buffer[256];
    printf("[PROCESS] Sedang membaca %s ...\n", filename);

    while (fscanf(fp, "%255s", buffer) == 1) {
        if (buffer[0] == '<') continue;
        cleanWord(buffer);
        addWordToMemory(buffer, data);
    }
    
    fclose(fp);
    printf("[SUCCESS] Data berhasil dimuat ke RAM.\n");
}

// --- FUNGSI SORTING BARU (BUBBLE SORT) ---

// Helper function: Menentukan apakah posisi A dan B harus ditukar?
// Mengembalikan 1 (True) jika posisi SALAH dan perlu tukar.
int needSwap(WordEntry a, WordEntry b) {
    // Kriteria 1: Frekuensi (Harus Descending / Besar ke Kecil)
    // Jika frekuensi B lebih besar dari A, berarti B harusnya di depan -> TUKAR
    if (b.frequency > a.frequency) return 1;
    if (b.frequency < a.frequency) return 0; // Sudah benar, A lebih besar

    // Kriteria 2: Panjang Kata (Harus Descending / Panjang ke Pendek)
    // Jika frekuensi sama, cek panjang
    int lenA = strlen(a.word);
    int lenB = strlen(b.word);
    if (lenB > lenA) return 1; // B lebih panjang, pindah ke depan -> TUKAR
    if (lenB < lenA) return 0;

    // Kriteria 3: Abjad (Harus Ascending / A ke Z)
    // Jika frekuensi sama DAN panjang sama, cek abjad
    // strcmp(a, b) > 0 artinya 'a' lebih besar dari 'b' (contoh: "zorro" vs "apel") -> TUKAR
    if (strcmp(a.word, b.word) > 0) return 1;

    return 0; // Tidak perlu tukar
}

void sortAllData(AlphabetGroup data[]) {
    printf("[PROCESS] Sedang mengurutkan data (Metode: Bubble Sort)...\n");
    printf("          Mohon bersabar, ini mungkin memakan waktu.\n");

    // Loop untuk setiap huruf (A-Z)
    for (int i = 0; i < 26; i++) {
        int n = data[i].count;
        if (n < 2) continue; // Kalau datanya 0 atau 1, tidak usah sort

        // Algoritma Bubble Sort
        for (int j = 0; j < n - 1; j++) {
            for (int k = 0; k < n - j - 1; k++) {
                
                // Cek apakah data[k] dan data[k+1] perlu ditukar
                if (needSwap(data[i].entries[k], data[i].entries[k+1])) {
                    
                    // Proses Tukar (Swap)
                    WordEntry temp = data[i].entries[k];
                    data[i].entries[k] = data[i].entries[k+1];
                    data[i].entries[k+1] = temp;
                }
            }
        }
    }
    printf("[SUCCESS] Sorting selesai.\n");
}