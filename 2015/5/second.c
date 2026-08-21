#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

char input[] = "./input.txt";

int is_word_good(char *buffer) {
    char *chr = buffer;

    bool hasXyX = false;
    bool hasXXyXX = false;
    while (*chr != '\n') {

        // checking for the efe pattern, with a bound check
        if (*(chr + 1) != '\n') {
            if (*chr == *(chr + 2)) {
                hasXyX = true;
            }
        }
        // the aa x.. aa check
        char a = *chr;
        char b = *(chr + 1);
        char *tmp = chr + 2;
        while (*tmp != '\n') {
            if (*tmp == a && *(tmp + 1) == b) {
                // should be a double double
                hasXXyXX = true;
            }
            tmp++;
        }
        chr++;
    }
    if (hasXXyXX && hasXyX) {
        printf("GOOD %s", buffer);
        return 1;
    } else {
        printf("BAD %s", buffer);
        return 0;
    };
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
        goodWords += is_word_good(buffer);
    }
    printf("goodwords = %ld\n", goodWords);
}
