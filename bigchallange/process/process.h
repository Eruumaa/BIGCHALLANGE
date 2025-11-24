/* file: process/process.h */
#ifndef PROCESS_H
#define PROCESS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// --- STRUKTUR DATA UTAMA ---
// Disimpan di sini karena 'process' adalah pemilik logika data

typedef struct {
    char word[100];
    int frequency;
} WordEntry;

typedef struct {
    WordEntry *entries; 
    int count;          
    int capacity;       
} AlphabetGroup;

// --- PROTOTYPE FUNGSI LOGIC ---
void initMemory(AlphabetGroup data[]);
void processTextFile(const char *filename, AlphabetGroup data[]);
void cleanWord(char *word);
void addWordToMemory(char *word, AlphabetGroup data[]);
void sortAllData(AlphabetGroup data[]);

#endif