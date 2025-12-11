#ifndef PROCESS_H
#define PROCESS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char word[100];
    int frequency;
} WordEntry;

typedef struct {
    WordEntry *entries; 
    int count;          
    int capacity;       
} AlphabetGroup;

void initMemory(AlphabetGroup data[]);
void processTextFile(const char *filename, AlphabetGroup data[]);
void cleanWord(char *Word);
void wordToMemory(char *word, AlphabetGroup data[]);
void sortingData(AlphabetGroup data[]);

#endif