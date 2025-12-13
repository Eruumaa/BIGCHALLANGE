#include "process.h"

void cleanWord(char *word) {
    for (int i = 0; word[i] != '\0'; i++) {
        word[i] = tolower(word[i]);
    }
}

void initMemory(AlphabetGroup data[]) {
    for (int i = 0; i < 26; i++) {
        data[i].count = 0;
    }
}

int blackListWord(const char * word) {
    const char *blacklist[] = {
        "com", "www", "https", "html", "net", "net", "org", "co", "id", "gov", "edu", "body", "title",
        "head", "div", "span", "br", "href", "script", "style", "link", "meta", "class", "src", "img", "width", "height",
        "table", "tr", "td", "strong", "em", "li", "ul", "ol", "url", "xml", NULL
    };
    for (int i = 0; blacklist[i] != NULL; i++) {
        if (strcmp(word, blacklist[i]) == 0) return 1;
    }
    return 0;
}

void wordToMemory(char *Word, AlphabetGroup data[]) {
    int index = tolower(Word[0]) - 'a';

    if (Word == NULL || *Word == '\0') return;

    cleanWord(Word);
    
    for (int i = 0; Word[i] != '\0'; i++) {
        if(!isalpha(Word[i])) return;
    }
    if (strlen(Word) < 1) return;
    if (strlen(Word) >= MAX_WORDS_LEN) return;
    if (blackListWord(Word)) return;
    if (index < 0 || index > 25) return;

    AlphabetGroup *group = &data[index];
    for (int i = 0; i < group->count; i++) {
        if (strcmp(group->entries[i].word, Word) == 0) {
            group->entries[i].frequency++;
        return;
        }
    }
    if (group->count >= MAX_WORDS_GROUP) {
        return;
    }
    strcpy(group->entries[group->count].word, Word);
    group->entries[group->count].frequency = 1;
    group->count++;
}

void removeTag(char *line, int *insideTag) {
    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == '<') {
            *insideTag = 1;
            line[i] = ' ';
            continue;
        }
        if (line[i] == '>') {
            *insideTag = 0;
            line[i] = ' ';
            continue;
        }
        if (*insideTag) {
            line[i] = ' ';
        }
    }
}

void processTextFile(const char *filename, AlphabetGroup data[]) {
    char linebuffer[4096];
    int insideTag = 0;
    
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf ("[ERROR] Gagal membuka file %s, file tidak ada\n", filename);
        return;
    }
    printf("[PROCESS] Membaca file %s\n", filename);

    while (fgets(linebuffer, sizeof(linebuffer), fp)) {
        removeTag(linebuffer, &insideTag);

        int writeIdx = 0;
        for (int readIdx = 0; linebuffer[readIdx] != '\0'; readIdx++) {
            if (linebuffer[readIdx] != '.') {
                linebuffer[writeIdx++] = linebuffer[readIdx];
            }
        }
        linebuffer[writeIdx] = '\0';

        const char *ignore = " \t\n\r,;:?!/()[]{}'\"-_=&|1234567890<>+*";
        char *token = strtok(linebuffer, ignore);        
        while (token != NULL) {
            cleanWord(token);
            wordToMemory(token, data);
            token = strtok(NULL, ignore);
        }
    }
    fclose(fp);

    printf("[SUCCESS] Parsing selesai.\n");
}

int pickPosition(WordEntry candidate, WordEntry currentBest) {
    if (candidate.frequency > currentBest.frequency) return 1;
    if (candidate.frequency < currentBest.frequency) return 0;

    int lenCand = strlen(candidate.word);
    int lenBest = strlen(currentBest.word);
    if (lenCand > lenBest) return 1;
    if (lenCand< lenBest) return 0;

    if (strcmp(candidate.word, currentBest.word) > 0) return 1;

    return 0;
}

void sortingData(AlphabetGroup data[]) {
    printf("[PROCESS] Sedang mengurutkan data (Metode: Selection Sort)...\n");

    for (int i = 0; i < 26; i++) {
        int n = data[i].count;
        if (n < 2) continue;

        for (int j = 0; j < n - 1; j++) {
            int bestIdx = j; 
            for (int k = j + 1; k < n; k++) {
                if (pickPosition(data[i].entries[k], data[i].entries[bestIdx])) {
                    bestIdx = k;
                }
            }
            if (bestIdx != j) {
                WordEntry temp = data[i].entries[j];
                data[i].entries[j] = data[i].entries[bestIdx];
                data[i].entries[bestIdx] = temp;
            }
        }
    }
    printf("[SUCCESS] Sorting selesai.\n");
}