#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

char input[] = "./input.txt";

int main() {
    FILE *fptr = fopen(input, "r");

    if (fptr == NULL) {
        printf("Failed to open file \" %s \" \n", input);
    }

    char buffer[256];
    uint32_t total_raw = 0;
    uint32_t total_raw_old = 0;
    uint32_t parsed_chars = 0;
    while (fgets(buffer, 256, fptr) != NULL) {
        int extra = 4;
        int i = 0;
        int subtract = 2;
        while (buffer[i] != '\n') {
            printf("%c", buffer[i]);
            if (buffer[i] == '\\') {
                extra += 2;
                subtract += 1;              // substract backslash
                if (buffer[i + 1] == 'x') { // if \x then we should substract 2
                    extra += -1;
                    subtract += 2;
                }
                if (buffer[i + 1] == '\\') {
                    i += 2;
                    continue;
                }
            }
            i++;
        }

        parsed_chars += (i - subtract);
        total_raw += i + extra;
        total_raw_old += i;
        printf(" %d, %d, %d \n", i + extra, subtract, i - subtract);
    }
    printf("total_raw_old = %d, total total_raw_new = %d, difference = %d\n", total_raw_old, total_raw, total_raw - total_raw_old);
}
