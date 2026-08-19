#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char filepath[] = "./input.txt";

struct package_measures {
    uint32_t x, y, z;
};
uint64_t ribbon = 0;

uint64_t get_required_paper(struct package_measures measurements) {
    uint64_t required = 2 * measurements.x * measurements.y + 2 * measurements.x * measurements.z + 2 * measurements.y * measurements.z;
    uint64_t temp_ribbon = 0;
    if (measurements.x < measurements.z) {
        if (measurements.y < measurements.z) {
            required += measurements.x * measurements.y;
            temp_ribbon += measurements.x + measurements.x + measurements.y + measurements.y;
        } else {
            required += measurements.x * measurements.z;
            temp_ribbon += measurements.x + measurements.x + measurements.z + measurements.z;
        }
    } else {
        if (measurements.x < measurements.y) {
            required += measurements.x * measurements.z;
            temp_ribbon += measurements.x + measurements.x + measurements.z + measurements.z;
        } else {
            required += measurements.z * measurements.y;
            temp_ribbon += measurements.z + measurements.z + measurements.y + measurements.y;
        }
    }

    temp_ribbon += measurements.x * measurements.y * measurements.z;
    ribbon += temp_ribbon;
    printf("X:%d, Y:%d, Z:%d = %ld\n", measurements.x, measurements.y, measurements.z, temp_ribbon);
    return required;
}

struct package_measures parse_line(char *line) {
    struct package_measures package;
    char *ptr = line;
    char *endptr;
    package.x = strtol(ptr, &endptr, 10);
    ptr = endptr;
    if (*ptr == 'x')
        ptr++;
    package.y = strtol(ptr, &endptr, 10);
    ptr = endptr;
    if (*ptr == 'x')
        ptr++;
    package.z = strtol(ptr, &endptr, 10);
    return package;
}
int main() {

    FILE *fptr = fopen(filepath, "r");
    fseek(fptr, 0, SEEK_END);
    uint64_t size = ftell(fptr);
    rewind(fptr);
    uint64_t read = 0;
    char buffer[256];
    uint64_t total = 0;
    while (fgets(buffer, sizeof(buffer), fptr) != NULL) {
        struct package_measures test = parse_line(buffer);
        total += get_required_paper(test);
    }
    printf("size %dB\n", size);
    printf("total %ld\n", total);
    printf("ribobn %ld\n", ribbon);

    fclose(fptr);

    return 0;
}
