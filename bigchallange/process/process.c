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

// --- FUNGSI HELPER: BLACKLIST KATA SAMPAH ---
// Fungsi ini mengecek apakah kata termasuk "sampah" sisa domain/url
int isIgnoredWord(const char *word) {
    // Daftar kata yang HARUS DIBUANG (Blacklist)
    // Kamu bisa tambah kata lain di sini jika masih ada yang bocor
    const char *blacklist[] = {
        "com", "www", "http", "https", "html", "net", "org", "co", "id", "gov", "edu", NULL
    };
    
    for (int i = 0; blacklist[i] != NULL; i++) {
        if (strcmp(word, blacklist[i]) == 0) {
            return 1; // TRUE: Kata ini ada di blacklist, buang!
        }
    }
    return 0; // FALSE: Kata aman
}

void cleanWord(char *word) {
    for (int i = 0; word[i] != '\0'; i++) {
        word[i] = tolower(word[i]);
    }
}

// --- FUNGSI ADD (DENGAN FILTER BLACKLIST) ---
void addWordToMemory(char *word, AlphabetGroup data[]) {
    // 1. Validasi Panjang
    if (strlen(word) < 2) return;

    // 2. Validasi Blacklist (Cek apakah ini "com", "www", dll?)
    if (isIgnoredWord(word)) return; // Stop, jangan simpan!

    int index = tolower(word[0]) - 'a'; 
    if (index < 0 || index > 25) return; 

    AlphabetGroup *group = &data[index];

    // Cek Duplikat
    for (int i = 0; i < group->count; i++) {
        if (strcmp(group->entries[i].word, word) == 0) {
            group->entries[i].frequency++;
            return;
        }
    }

    // Resize Memori
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

// --- FUNGSI UTAMA: PROCESS TEXT FILE (Parser Cerdas + Skip URL/CSS) ---
void processTextFile(const char *filename, AlphabetGroup data[]) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("[ERROR] File tidak ditemukan di: %s\n", filename);
        return;
    }

    printf("[PROCESS] Membaca %s (Filter: URL Tag, CSS Block, & Domain Ext)...\n", filename);

    char buffer[256]; 
    char tagName[64]; 
    int bufIdx = 0;
    int c;
    
    int skippingContent = 0; 

    while ((c = fgetc(fp)) != EOF) {
        
        // 1. Deteksi Awal Tag '<'
        if (c == '<') {
            if (bufIdx > 0) {
                buffer[bufIdx] = '\0';
                cleanWord(buffer);
                addWordToMemory(buffer, data); // Filter 'com' terjadi di dalam fungsi ini
                bufIdx = 0;
            }

            // Baca Nama Tag
            int tIdx = 0;
            int nextC = fgetc(fp);

            while (nextC != EOF && isspace(nextC)) {
                nextC = fgetc(fp);
            }

            while (nextC != EOF && !isspace(nextC) && nextC != '>') {
                if (tIdx < 63) tagName[tIdx++] = tolower(nextC);
                nextC = fgetc(fp);
            }
            tagName[tIdx] = '\0'; 

            // Cek Skip Mode (style, script, url)
            if (strcmp(tagName, "style") == 0 || 
                strcmp(tagName, "script") == 0 || 
                strcmp(tagName, "url") == 0) {
                skippingContent = 1; 
            }
            else if (strcmp(tagName, "/style") == 0 || 
                     strcmp(tagName, "/script") == 0 || 
                     strcmp(tagName, "/url") == 0) {
                skippingContent = 0;
            }

            // Habiskan sisa atribut
            if (nextC != '>') {
                while ((c = fgetc(fp)) != EOF && c != '>') {}
            }
            continue; 
        }

        // 2. Mode Skip
        if (skippingContent) {
            continue;
        }

        // 3. Ambil Teks Biasa
        if (isalpha(c)) {
            if (bufIdx < 255) {
                buffer[bufIdx++] = tolower(c);
            }
        } else {
            if (bufIdx > 0) {
                buffer[bufIdx] = '\0';
                cleanWord(buffer);
                addWordToMemory(buffer, data); // Filter 'com' akan dicek di sini
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
    printf("[SUCCESS] Parsing selesai. Data bersih.\n");
}

// --- FUNGSI SORTING ---
int compareWords(const void *a, const void *b) {
    WordEntry *entryA = (WordEntry *)a;
    WordEntry *entryB = (WordEntry *)b;

    // 1. Frekuensi (Desc)
    if (entryB->frequency != entryA->frequency) {
        return entryB->frequency - entryA->frequency;
    }
    // 2. Panjang Kata (Desc)
    int lenA = strlen(entryA->word);
    int lenB = strlen(entryB->word);
    if (lenB != lenA) {
        return lenB - lenA; 
    }
    // 3. Abjad (Asc)
    return strcmp(entryA->word, entryB->word);
}

void sortAllData(AlphabetGroup data[]) {
    printf("[PROCESS] Sedang mengurutkan data...\n");
    for (int i = 0; i < 26; i++) {
        if (data[i].count > 0) {
            qsort(data[i].entries, data[i].count, sizeof(WordEntry), compareWords);
        }
    }
    printf("[SUCCESS] Sorting selesai.\n");
}