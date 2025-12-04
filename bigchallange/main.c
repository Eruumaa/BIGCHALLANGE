#include <stdio.h>
#include <stdlib.h>
#include "fileio/fileio.h"

int main () {
    AlphabetGroup dictionary[26];
    
    int choice ;

    while (1) {
        clearScreen();

        choice = showMenu();

        if (choice == 1) {
            processTextFile("data/Data-50k.txt", dictionary);
            sortAllData(dictionary);
            saveToBinary(dictionary, "output.bin");

            waitUser();
        }
    }
    return 0;
}