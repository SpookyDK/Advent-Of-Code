// TODO:   fgets on the file
// TODO: Look for naughty words
// TODO: count vowels
// TODO: look for doubles
// TODO: break if naughty
//
//
//
//
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

char input[] = "./input.txt";

int is_word_good(char *buffer) {
    printf("%s\n", buffer);
    char *chr = buffer;
    uint32_t vowels = 0;

    bool hasDouble = false;
    bool badWord = false;
    while (*chr != '\n') {
        // vowel check
        switch (*chr) {
        case 'a':
            vowels++;
            break;
        case 'e':
            vowels++;
            break;
        case 'i':
            vowels++;
            break;
        case 'o':
            vowels++;
            break;
        case 'u':
            vowels++;
            break;
        }
        if (*chr == *(chr + 1)) {
            hasDouble = true;
        }
        if (*chr == 'a' && *(chr + 1) == 'b') {
            badWord = true;
            break;
        }
        if (*chr == 'c' && *(chr + 1) == 'd') {
            badWord = true;
            break;
        }
        if (*chr == 'p' && *(chr + 1) == 'q') {
            badWord = true;
            break;
        }
        if (*chr == 'x' && *(chr + 1) == 'y') {
            badWord = true;
            break;
        }
        chr++;
    }
    if (vowels >= 3 && !badWord && hasDouble) {
        return 1;
    } else {
        return 0;
    }
}
int main() {
    char buffer[256];
    FILE *fptr = fopen(input, "r");
    if (fptr == NULL) {
        printf("Failed to open file\n");
        return 1;
    }

    fseek(fptr, 0, SEEK_END);
    uint64_t size = ftell(fptr);
    rewind(fptr);
    uint64_t read = 0;
    uint64_t goodWords = 0;
    while (fgets(buffer, sizeof(buffer), fptr) != NULL) {
        printf("%s\n", buffer);
        goodWords += is_word_good(buffer);
    }
    printf("goodwords = %ld\n", goodWords);
}
