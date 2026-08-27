// JSON is the worst format in every possible definition of the word
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char input[] = "./test.txt";

int main() {
    FILE *fptr = fopen(input, "r");

    if (fptr == NULL) {
        printf("Failed to load file %s\n", input);
        return 0;
    }

    uint64_t sum = 0;
    char *buffer;
    uint32_t buffersize = 1024 * 128;
    buffer = malloc(buffersize);
    bool in_array = false;
    bool has_red = false;
    int bracket_level = 0;

    while (fgets(buffer, buffersize, fptr) != NULL) {
        char *bufptr = &buffer[0];
        int i = 0;
        while (*bufptr != '\n' && *bufptr != '\0') {

            if (*bufptr == '[' || *bufptr == ']') {
                in_array = !in_array;
                bufptr++;
                continue;
            }
            if (*bufptr == '{') {
                bracket_level += 1;
                bufptr++;
                continue;
            }
            if (*bufptr == '{') {
                bracket_level -= 1;
                bufptr++;
                continue;
            }
            if (has_red) {
                if (bracket_level == -1) {
                    has_red = false;
                }
            } else {
                if (bufptr[0] == 'r' && bufptr[1] == 'e' && bufptr[2] == 'd') {
                    has_red = true;
                    bufptr += 3;
                    bracket_level = 0;
                    printf("Found red");
                    continue;
                }
                if (*bufptr >= '0' && *bufptr <= '9' ||
                    *bufptr == '-') { // we just search for number by looking between the asciii values for numbers
                    char *endptr;
                    int64_t value = strtol(bufptr, &endptr, 10);
                    if (endptr != bufptr) {
                        sum += value;
                        // printf("value= %ld\n", value);
                        bufptr = endptr;
                        continue;
                    }
                }
            }
            bufptr++;
        }
    }

    free(buffer);
    printf("SUM = %ld\n", sum);
    fclose(fptr);
}
