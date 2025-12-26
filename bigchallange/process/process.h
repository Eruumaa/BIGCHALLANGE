#ifndef PROCESS_H
#define PROCESS_H
#define MAX_WORDS_GROUP 15000
#define MAX_WORDS_LEN 50

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char word[MAX_WORDS_LEN];
    int frequency;
} WordEntry;

typedef struct {
    WordEntry entries[MAX_WORDS_GROUP]; 
    int count;               
} AlphabetGroup;

int processTextFile(const char *filename, AlphabetGroup data[]);
void clearMemory(AlphabetGroup data[]);
void cleanWord(char *Word);
void wordToMemory(char *word, AlphabetGroup data[]);
void sortingData(AlphabetGroup data[]);

#endif