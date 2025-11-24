#ifndef PROCESS_H
#define PROCESS_H

#define MAX_WORDS 50000
#define MAX_WORD_LENGTH 128

typedef struct {
    char word[MAX_WORD_LENGTH];
    int freq;
} WordData;

typedef struct {
    WordData words[MAX_WORDS];
    int count;
} AlphabetData;

// Membersihkan string → hapus digit & tanda baca
void clean_string(char *str);

// Menambahkan / menambah frekuensi kata
void insert_word(const char *token, AlphabetData alphabet_list[]);

// Mengurutkan kata per abjad berdasarkan frekuensi desc, 
// jika sama → panjang desc, jika sama → alfabet desc
void sort_alphabet_data(AlphabetData alphabet_list[]);

#endif
