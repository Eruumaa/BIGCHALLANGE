//fileio.c berisi implementasi dari tiga fungsi utama, yaitu membaca file teks (.txt), menyimpan data ke file biner (.bin), membaca kembali data dari file biner (.bin)
#include "fileio.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
//Fungsi void read_text_file untuk mengubah file mentah (Data-50K.txt) menjadi database kata terstruktur dalam array alfabet
void read_text_file(const char *filename, AlphabetData alphabet_list[]) { //Fungsi ini berfungsi untuk membaca file teks.txt baris demi baris, lalu mengabaikan teks dalam <url>...</url>, hanya mengambil isi <title> dan <body>, menghapus digit, tanda baca -> jadi huruf kecil, memecah menjadi kata (strtok), dan memasukkan kata ke struktur data (pakai insert_word())  
    FILE *fp = fopen(filename, "r"); 
    if (!fp) {
        printf("Tidak dapat membuka file teks!\n");
        return;
    }

    char buffer[10000];
    int in_url = 0, in_title = 0, in_body = 0;

    while (fgets(buffer, sizeof(buffer), fp)) {

        if (strstr(buffer, "<url>")) in_url = 1;
        if (strstr(buffer, "</url>")) in_url = 0;

        if (strstr(buffer, "<title>")) in_title = 1;
        if (strstr(buffer, "</title>")) in_title = 0;

        if (strstr(buffer, "<body>")) in_body = 1;
        if (strstr(buffer, "</body>")) in_body = 0;

        if (in_url) continue;

        if (in_title || in_body) {

            clean_string(buffer);

            char *token = strtok(buffer, " ");  //Disini berfungsi untuk memecah baris menjadi per kata, setiap kata dimasukkan ke struktur melalui insert_word()
            while (token) {
                if (strlen(token) > 0)
                    insert_word(token, alphabet_list);
                token = strtok(NULL, " ");
            }
        }
    }

    fclose(fp);
}
// Fungsi void save_to_binary untuk menyimpan data yang sudah diurutkan ke file biner agar bisa dibaca lagi pada menu
void save_to_binary(const char *filename, AlphabetData alphabet_list[]) { //Membaca file.bin hasil penyimpanan
    FILE *fp = fopen(filename, "wb");
    if (!fp) { printf("Gagal membuat file binari!\n"); return; }

    for (int i = 0; i < 26; i++) {
        char abjad = 'a' + i;
        fwrite(&abjad, sizeof(char), 1, fp); //menyimpan abjad a, b, c, ...z
        fwrite(&alphabet_list[i].count, sizeof(int), 1, fp); //Jumlah kata yang dimulai huruf itu

        for (int j = 0; j < alphabet_list[i].count; j++) {
            WordData *w = &alphabet_list[i].words[j];
            int len = strlen(w->word); //Foemat biner sesuai instruksi Big Challenge

            fwrite(&len, sizeof(int), 1, fp);
            fwrite(w->word, sizeof(char), len, fp);
            fwrite(&w->freq, sizeof(int), 1, fp);
        }
    }

    fclose(fp);
}
//Mengembalikan data dari file biner ke array agar bisa ditampilkan
void read_from_binary(const char *filename, AlphabetData alphabet_list[]) { //Membaca file.bin hasil penyimpanan
    FILE *fp = fopen(filename, "rb");
    if (!fp) { printf("File binari tidak ditemukan!\n"); return; }

    for (int i = 0; i < 26; i++) {
        char abjad;
        fread(&abjad, sizeof(char), 1, fp); //Membaca Abjad

        fread(&alphabet_list[i].count, sizeof(int), 1, fp); //Membaca jumlah kata

        for (int j = 0; j < alphabet_list[i].count; j++) {
            int len;
            fread(&len, sizeof(int), 1, fp); //membaca setiap kata yang dimana ini merekonstruksi struktur data seperti awal

            fread(alphabet_list[i].words[j].word, sizeof(char), len, fp);
            alphabet_list[i].words[j].word[len] = '\0';

            fread(&alphabet_list[i].words[j].freq, sizeof(int), 1, fp);
        }
    }

    fclose(fp);
}
