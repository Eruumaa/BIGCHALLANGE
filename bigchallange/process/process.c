#include "process.h"

void cleanWord(char *word) {
    for (int i = 0; word[i] != '\0'; i++) {
        word[i] = tolower(word[i]);
    }
}

void clearMemory(AlphabetGroup data[]) {
    for (int i = 0; i < 26; i++) {
        data[i].count = 0;
    }
}

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

void wordToMemory(char *Word, AlphabetGroup data[]) {
    if (Word == NULL || *Word == '\0') return;
    if (strlen(Word) < 2) return;
    if (strlen(Word) >= MAX_WORDS_LEN) return;

    cleanWord(Word);

    if (!isalpha(Word[0])) return;
    if (blackListWord(Word)) return;

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

int processTextFile(const char *filename, AlphabetGroup data[]) {
    char linebuffer[20000];
    
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf ("[ERROR] Gagal membuka file %s, file tidak ada\n", filename);
        return 1;
    }
    printf("[PROCESS] Membaca file %s\n", filename);

    while (fgets(linebuffer, sizeof(linebuffer), fp)) {
        char *startBody = strstr(linebuffer, "<body");
        char *endBody = strstr(linebuffer, "</body");

        if (startBody == NULL || endBody == NULL) {
            continue;
        }
        startBody += 6;
        *endBody = '\0';

        int writeIdx = 0;
        for (int readIdx = 0; linebuffer[readIdx] != '\0'; readIdx++) {
            if (startBody[readIdx] != '.') {
                startBody[writeIdx++] = startBody[readIdx];
            }
        }
        startBody[writeIdx] = '\0';

        const char *ignore = " \t\n\r,;:?!/()[]{}'\"-_=&|1234567890<>+*";
        char *token = strtok(startBody, ignore);        
        while (token != NULL) {
            wordToMemory(token, data);
            token = strtok(NULL, ignore);
        }
    }
    fclose(fp);

    printf("[SUCCESS] Parsing selesai\n");
    return 0;
}

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
    return strcmp(candidate.word, currentBest.word) > 0;
}

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