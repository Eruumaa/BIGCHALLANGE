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