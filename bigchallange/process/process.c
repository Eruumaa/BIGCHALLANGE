#include "process.h"

void initMemory(AlphabetGroup data[]) {
    for (int i = 0; i < 26; i++) {
        data[i].count = 0;
        data[i].capacity = 50;
        data[i].entries = (WordEntry *)malloc(data[i].capacity * sizeof(WordEntry));
    }
}

int isIgnoredWord(const char * word) {
    const char *blacklist[] = {
        "com", "www", "https", "html", "net", "net", "org", "co", "id", "gov", "edu", NULL
    };
    for (int i = 0; blacklist[i] != NULL; i++) {
        if (strcmp(word, blacklist[i]) == 0) return 1;
    }
    return 0;
}

void addWordToMemory(char *Word, AlphabetGroup data[]) {
    int index = tolower(Word[0]) - 'a';
    if (strlen(Word) < 2) return;
    if (isIgnoredWord(Word)) return;
    if (index < 0 || index > 25) return;

    AlphabetGroup *group = &data[index];
    for (int i = 0; i < group->count; i++) {
        if (strcmp(group->entries[i].word, Word) == 0) {
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
    
    str(group->entries[group->count].word, Word);
    group->entries[group->count].frequency = 1;
    group->count++;
}

void processTextFile(const char *filename, AlphabetGroup data[]) {
    char buffer[256]; 
    char tagName[64]; 
    int bufIdx = 0;
    int c;
    int skippingContent = 0; 

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("[ERROR] File tidak ada di: %s\n", filename);
        return;
    }
    printf("[PROCESS] Membaca file %s...\n", filename);

    while ((c = fgetc(fp)) != EOF) {
        if (c == '<') {
            if (bufIdx > 0) {
                buffer[bufIdx] = '\0';
                cleanWord(buffer);
                addWordToMemory(buffer, data);
                bufIdx = 0;
            }
    }
}