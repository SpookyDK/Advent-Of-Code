// JSON is the worst format in every possible definition of the word
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char input[] = "./input.txt";

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
    while (fgets(buffer, buffersize, fptr) != NULL) {
        char *bufptr = &buffer[0];
        int i = 0;
        while (*bufptr != '\n' && *bufptr != '\0') {
            if (*bufptr >= '0' && *bufptr <= '9' ||
                *bufptr == '-') { // we just search for number by looking between the asciii values for numbers
                char *endptr;
                int64_t value = strtol(bufptr, &endptr, 10);
                if (endptr != bufptr) {
                    sum += value;
                    printf("value= %ld\n", value);
                    bufptr = endptr;
                    continue;
                }
            }
            bufptr++;
        }
    }

    free(buffer);
    printf("SUM = %ld\n", sum);
    fclose(fptr);
}
