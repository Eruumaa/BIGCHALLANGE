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
            "com", "www", "https", "html", "net", "net", "org", "co", "id", "gov", "edu", NULL
        };
        for (int i = 0; blacklist[i] != NULL; i++) {
            if (strcmp(word, blacklist[i]) == 0) return 1;
        }
        return 0;
    }

    void wordToMemory(char *Word, AlphabetGroup data[]) {
        int index = tolower(Word[0]) - 'a';
        if (strlen(Word) < 2) return;
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

    void processTextFile(const char *filename, AlphabetGroup data[]) {
        char buffer[256]; 
        char tagName[64]; 
        int bufIdx = 0;
        int tIdx = 0;
        int nextC;
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
                    wordToMemory(buffer, data);
                    bufIdx = 0;
                }
                tIdx = 0;
                nextC = fgetc(fp);

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
                    wordToMemory(buffer, data);
                    bufIdx = 0;
                }
            }
        }
        
        if (bufIdx > 0) {
            buffer[bufIdx] = '\0';
            cleanWord(buffer);
            wordToMemory(buffer, data);
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

        if (strcmp(candidate.word, currentBest.word) < 0) return 1;

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