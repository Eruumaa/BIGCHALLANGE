/* file: process/process.c */
#include "process.h"

// --- FUNGSI INIT ---
void initMemory(AlphabetGroup data[]) {
    for (int i = 0; i < 26; i++) {
        data[i].count = 0;
        data[i].capacity = 50; 
        data[i].entries = (WordEntry *)malloc(data[i].capacity * sizeof(WordEntry));
    }
}

// --- FUNGSI HELPER: BLACKLIST ---
int isIgnoredWord(const char *word) {
    const char *blacklist[] = {
        "com", "www", "http", "https", "html", "net", "org", "co", "id", "gov", "edu", NULL
    };
    for (int i = 0; blacklist[i] != NULL; i++) {
        if (strcmp(word, blacklist[i]) == 0) return 1;
    }
    return 0;
}

void cleanWord(char *word) {
    for (int i = 0; word[i] != '\0'; i++) {
        word[i] = tolower(word[i]);
    }
}

// --- FUNGSI ADD ---
void addWordToMemory(char *word, AlphabetGroup data[]) {
    if (strlen(word) < 2) return;
    if (isIgnoredWord(word)) return; 

    int index = tolower(word[0]) - 'a'; 
    if (index < 0 || index > 25) return; 

    AlphabetGroup *group = &data[index];

    for (int i = 0; i < group->count; i++) {
        if (strcmp(group->entries[i].word, word) == 0) {
            group->entries[i].frequency++;
            return;
        }
    }

    if (group->count == group->capacity) {
        group->capacity *= 2;
        WordEntry *temp = (WordEntry *)realloc(group->entries, group->capacity * sizeof(WordEntry));
        if (!temp) exit(1);
        group->entries = temp;
    }

    strcpy(group->entries[group->count].word, word);
    group->entries[group->count].frequency = 1;
    group->count++;
}

// --- FUNGSI PROCESS TEXT (PARSER CERDAS) ---
void processTextFile(const char *filename, AlphabetGroup data[]) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("[ERROR] File tidak ditemukan di: %s\n", filename);
        return;
    }

    printf("[PROCESS] Membaca %s (Parsing & Filter Active)...\n", filename);

    char buffer[256]; 
    char tagName[64]; 
    int bufIdx = 0;
    int c;
    int skippingContent = 0; 

    while ((c = fgetc(fp)) != EOF) {
        if (c == '<') {
            if (bufIdx > 0) {
                buffer[bufIdx] = '\0';
                cleanWord(buffer);
                addWordToMemory(buffer, data);
                bufIdx = 0;
            }

            int tIdx = 0;
            int nextC = fgetc(fp);
            while (nextC != EOF && isspace(nextC)) nextC = fgetc(fp);
            while (nextC != EOF && !isspace(nextC) && nextC != '>') {
                if (tIdx < 63) tagName[tIdx++] = tolower(nextC);
                nextC = fgetc(fp);
            }
            tagName[tIdx] = '\0'; 

            if (strcmp(tagName, "style") == 0 || strcmp(tagName, "script") == 0 || strcmp(tagName, "url") == 0) {
                skippingContent = 1; 
            } else if (strcmp(tagName, "/style") == 0 || strcmp(tagName, "/script") == 0 || strcmp(tagName, "/url") == 0) {
                skippingContent = 0;
            }

            if (nextC != '>') {
                while ((c = fgetc(fp)) != EOF && c != '>') {}
            }
            continue; 
        }

        if (skippingContent) continue;

        if (isalpha(c)) {
            if (bufIdx < 255) buffer[bufIdx++] = tolower(c);
        } else {
            if (bufIdx > 0) {
                buffer[bufIdx] = '\0';
                cleanWord(buffer);
                addWordToMemory(buffer, data);
                bufIdx = 0;
            }
        }
    }
    
    if (bufIdx > 0) {
        buffer[bufIdx] = '\0';
        cleanWord(buffer);
        addWordToMemory(buffer, data);
    }

    fclose(fp);
    printf("[SUCCESS] Parsing selesai.\n");
}

// --- LOGIKA SORTING (SELECTION SORT) ---

// Fungsi Helper: Menentukan apakah kandidat (B) lebih baik posisinya daripada (A)?
// Return 1 jika B harus menggeser A (B lebih prioritas)
// Return 0 jika A sudah benar di posisinya
int isBetterPosition(WordEntry candidate, WordEntry currentBest) {
    // 1. Cek Frekuensi (Harus Descending / Besar ke Kecil)
    if (candidate.frequency > currentBest.frequency) return 1; 
    if (candidate.frequency < currentBest.frequency) return 0;

    // 2. Jika Frekuensi sama, Cek Panjang Kata (Harus Descending)
    int lenCand = strlen(candidate.word);
    int lenBest = strlen(currentBest.word);
    if (lenCand > lenBest) return 1;
    if (lenCand < lenBest) return 0;

    // 3. Jika Panjang sama, Cek Abjad (Harus Ascending / A ke Z)
    // strcmp(a, b) < 0 artinya 'a' lebih kecil (lebih awal di kamus) daripada 'b'
    if (strcmp(candidate.word, currentBest.word) < 0) return 1;

    return 0;
}

void sortAllData(AlphabetGroup data[]) {
    printf("[PROCESS] Sedang mengurutkan data (Metode: Selection Sort)...\n");

    // Loop untuk setiap huruf (A-Z)
    for (int i = 0; i < 26; i++) {
        int n = data[i].count;
        if (n < 2) continue; // Skip jika data cuma 0 atau 1

        // Algoritma Selection Sort
        for (int j = 0; j < n - 1; j++) {
            // Anggap elemen di posisi 'j' adalah yang terbaik sementara
            int bestIdx = j; 

            // Cari elemen yang lebih baik di sisa array (j+1 sampai akhir)
            for (int k = j + 1; k < n; k++) {
                if (isBetterPosition(data[i].entries[k], data[i].entries[bestIdx])) {
                    bestIdx = k; // Ditemukan kandidat baru yang lebih pas
                }
            }

            // Jika ditemukan yang lebih baik, lakukan SWAP
            if (bestIdx != j) {
                WordEntry temp = data[i].entries[j];
                data[i].entries[j] = data[i].entries[bestIdx];
                data[i].entries[bestIdx] = temp;
            }
        }
    }
    printf("[SUCCESS] Sorting selesai.\n");
}