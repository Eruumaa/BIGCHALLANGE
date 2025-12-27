#include "process.h"

// Fungsi mengubah setiap kata menjadi huruf kecil
void cleanWord(char *word) {
    for (int i = 0; word[i] != '\0'; i++) {
        word[i] = tolower(word[i]);
    }
}

// Fungsi untuk reset isi memori
void clearMemory(AlphabetGroup data[]) {
    for (int i = 0; i < 26; i++) {
        data[i].count = 0;
    }
}

// Fungsi untuk memastikan kata-kata tertentu tidak masuk
int blackListWord(const char * word) {
    const char *blacklist[] = {
        "com", "www", "https", "html", "net", "net", "org", "co", "id", "gov", "edu", "body", "title", "read", "head", 
        "div", "span", "br", "href", "script", "style", "link", "meta", "class", "src", "img", "width", "height", "table", 
        "tr", "td", "em", "li", "ul", "ol", "url", "xml", NULL
    };
    for (int i = 0; blacklist[i] != NULL; i++) {
        if (strcmp(word, blacklist[i]) == 0) return 1;
    }
    return 0;
}

// Fungsi untuk memasukkan tiap-tiap kata ke data utama
void wordToMemory(char *Word, AlphabetGroup data[]) {
if (Word == NULL || *Word == '\0') return;
    
    int len = strlen(Word);
    if (len < 2) return;              
    if (len >= MAX_WORDS_LEN) return;  

    cleanWord(Word); 

    // Penggunaan isalpha untuk memastikan kata yang disimpan diawali dengan huruf
    if (!isalpha(Word[0])) return;   
    if (blackListWord(Word)) return; 

    // Penggunaan tolower untuk memastikan index'A' sama dengan 'a' diawali dari 0
    int index = tolower(Word[0]) - 'a';
    if (index < 0 || index > 25) return;

    AlphabetGroup *group = &data[index];
    for (int i = 0; i < group->count; i++) {
        if (strcmp(group->entries[i].word, Word) == 0) {
            group->entries[i].frequency++;
        return;
        }
    }
    if (group->count >= MAX_WORDS_GROUP) return;

    strcpy(group->entries[group->count].word, Word);
    group->entries[group->count].frequency = 1;
    group->count++;
}

// Fungsi membaca file text dalam tag <title> dan <body> saja
int processTextFile(const char *filename, AlphabetGroup data[]) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("[ERROR] Gagal membuka file %s\n", filename);
        return 1;
    }
    printf("[PROCESS] Membaca file %s ...\n", filename);

    char buffer[256];   
    char tagName[64];   
    int bufIdx = 0;
    int c;
    int inTitle = 0; 
    int inBody = 0;  
    int inUrl = 0;   
    int inSkip = 0;

    while ((c = fgetc(fp)) != EOF) {
        if (c == '<') {
            if (bufIdx > 0) {
                buffer[bufIdx] = '\0';
                if ((inTitle || inBody) && !inUrl && !inSkip) {
                    wordToMemory(buffer, data);
                }
                bufIdx = 0;
            }

            int tIdx = 0;
            int nextC = fgetc(fp);
            
            // Penggunaaan isspace untuk melewati jika jumpa spasi, enter, tab sampai jumpa huruf lain
            while (nextC != EOF && isspace(nextC)) nextC = fgetc(fp); 
            while (nextC != EOF && !isspace(nextC) && nextC != '>') {
                if (tIdx < 63) tagName[tIdx++] = tolower(nextC);
                nextC = fgetc(fp);
            }
            tagName[tIdx] = '\0';
            
            // Mengabaikan string dalam tag <url>
            if (strcmp(tagName, "url") == 0) inUrl = 1;
            else if (strcmp(tagName, "/url") == 0) inUrl = 0;

            // Menghimpun kata diantara <title>
            else if (strcmp(tagName, "title") == 0) inTitle = 1;
            else if (strcmp(tagName, "/title") == 0) inTitle = 0;

            // Menghimpun kata diantara <body>
            else if (strcmp(tagName, "body") == 0) inBody = 1;
            else if (strcmp(tagName, "/body") == 0) inBody = 0;

            // Skip script/style 
            else if (strcmp(tagName, "script") == 0 || strcmp(tagName, "style") == 0) inSkip = 1;
            else if (strcmp(tagName, "/script") == 0 || strcmp(tagName, "/style") == 0) inSkip = 0;

            // Menghabiskan sisa tag sampai ke '>'
            if (nextC != '>') {
                while ((c = fgetc(fp)) != EOF && c != '>') {}
            }
            continue; 
        }

        // Mengambil kata
        if ((inTitle || inBody) && !inUrl && !inSkip) {
            // Penggunaan isalpha untuk menyimpan karakter ke dalam buffer jika hanya huruf
            if (isalpha(c)) {
                if (bufIdx < MAX_WORDS_LEN - 1) {
                    buffer[bufIdx++] = (char)c;
                }
            } else {
                // Jika jumpa pemisah spasi/tanda baca
                if (bufIdx > 0) {
                    buffer[bufIdx] = '\0';
                    wordToMemory(buffer, data);
                    bufIdx = 0;
                }
            }
        }
    }

    if (bufIdx > 0 && (inTitle || inBody) && !inUrl && !inSkip) {
        buffer[bufIdx] = '\0';
        wordToMemory(buffer, data);
    }

    fclose(fp);
    printf("[SUCCESS] Parsing selesai.\n");
    return 0;
}

// Fungsi untuk menentukan posisi kata untuk membantu proses sorting 
int pickPosition(WordEntry candidate, WordEntry currentBest) {
    int lenCand = strlen(candidate.word);
    int lenBest = strlen(currentBest.word);
    // Posisi Frekuensi
    if (candidate.frequency != currentBest.frequency) {
        return candidate.frequency > currentBest.frequency;
    }
    // Posisi Panjang kata
    if (lenCand != lenBest) {
        return lenCand > lenBest;
    }
    // Posisi Abjad
    return strcmp(candidate.word, currentBest.word) < 0;
}

// Fungsi sorting menggunakan metode insertion sort
void sortingData(AlphabetGroup data[]) {
    printf("[PROCESS] Sedang mengurutkan data...\n");

    for (int i = 0; i < 26; i++) {
        int n = data[i].count;
        if (n < 2) continue;

        for (int j = 1; j < n; j++) {
            WordEntry key = data[i].entries[j];
            int k = j - 1;

            while (k >= 0 && pickPosition(key, data[i].entries[k])) {
                data[i].entries [k + 1] = data[i].entries[k];
                k = k - 1;
            }
            data[i].entries[k + 1] = key;
        }
    }
    printf("[SUCCESS] Sorting selesai.\n");
}