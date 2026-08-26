#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char input[] = "1113122113";
char *buffer;
uint64_t length;

int main() {
    uint64_t buffer_size = 4096;
    buffer = malloc(buffer_size);
    memcpy(buffer, input, sizeof(input)); // copy into working buffer.
    char *temp_buffer;
    temp_buffer = malloc(buffer_size);

    for (int it = 0; it < 40; it++) { // repeat 40 times
        uint64_t index = 0;
        uint64_t newIndex = 0;

        while (buffer[index] != '\0') {
            uint8_t chr = buffer[index];
            uint64_t amount = 1;
            index++;
            while (buffer[index] == chr && buffer[index] != '\0') {
                index++;
                amount++;
            }
            if (newIndex + 3 >= buffer_size) {
                buffer_size *= 2;
                buffer = realloc(buffer, buffer_size);
                temp_buffer = realloc(temp_buffer, buffer_size);
            }
            temp_buffer[newIndex] = amount + '0';
            temp_buffer[newIndex + 1] = chr;
            newIndex += 2;
        }
        temp_buffer[newIndex] = '\0';
        printf("Iteration %d: length = %d\n", it + 1, newIndex);
        char *swap = buffer;
        buffer = temp_buffer;
        temp_buffer = swap;
    }
    length = 0;
    while (buffer[length] != '\0') {
        length++;
    }
    free(buffer);
    free(temp_buffer);
    printf("length = %d\n", length);
}
