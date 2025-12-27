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

// Fungsi membaca file text dalam tag <title> dan <body> saja menggunakan tokenisasi
int processTextFile(const char *filename, AlphabetGroup data[]) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("[ERROR] Gagal membuka file %s\n", filename);
        return 1;
    }
    printf("[PROCESS] Membaca file %s...\n", filename);

    char lineBuffer[8192];   
    char cleanContent[8192]; 
    int inTag = 0;
    int inTitle = 0;
    int inBody = 0;
    int inUrl = 0;
    int inSkip = 0;
    char tagName[64];
    int tagIdx = 0;
    int captureTagName = 0;
    
    // Perulangan yang membaca baris perbaris
    while (fgets(lineBuffer, sizeof(lineBuffer), fp)) {
        int cleanIdx = 0;
        // Perulangan untuk membuang tag HTML dan mengambil teks
        for (int i = 0; lineBuffer[i] != '\0'; i++) {
            char c = lineBuffer[i];
            if (c == '<') {
                inTag = 1;
                captureTagName = 1;
                tagIdx = 0;
                tagName[0] = '\0';
                cleanContent[cleanIdx++] = ' '; 
                continue;
            }
            if (c == '>') {
                inTag = 0;
                captureTagName = 0;
                tagName[tagIdx] = '\0';
                // Kondisi untuk mengecek nama tag
                if (strcmp(tagName, "url") == 0) inUrl = 1;
                else if (strcmp(tagName, "/url") == 0) inUrl = 0;
                else if (strcmp(tagName, "title") == 0) inTitle = 1;
                else if (strcmp(tagName, "/title") == 0) inTitle = 0;
                else if (strcmp(tagName, "body") == 0) inBody = 1;
                else if (strcmp(tagName, "/body") == 0) inBody = 0;
                else if (strcmp(tagName, "script") == 0 || strcmp(tagName, "style") == 0) inSkip = 1;
                else if (strcmp(tagName, "/script") == 0 || strcmp(tagName, "/style") == 0) inSkip = 0;
                cleanContent[cleanIdx++] = ' '; 
                continue;
            }
            if (inTag) {
                if (captureTagName) {
                    // Pengguanaan isspace untuk mendeteksi akhir dari tag html
                    if (isspace(c)) {
                        captureTagName = 0;
                    } else if (tagIdx < 63) {
                        tagName[tagIdx++] = tolower(c);
                        tagName[tagIdx] = '\0';
                    }
                }
            } else {
                if ((inTitle || inBody) && !inUrl && !inSkip) {
                    // Penggunaan isalpha untuk membersihkan teks 
                    if (isalpha(c)) {
                        cleanContent[cleanIdx++] = c;
                    } else {
                        cleanContent[cleanIdx++] = ' ';
                    }
                }
            }
        }
        cleanContent[cleanIdx] = '\0';
        // Tokenisasi dengan strtok
        const char *delimiters = " \t\n\r1234567890!@#$%^&*()-_=+[]{}\\|;:'\",.<>/?`~";
        
        char *token = strtok(cleanContent, delimiters);
        while (token != NULL) {
            wordToMemory(token, data);
            token = strtok(NULL, delimiters);
        }
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