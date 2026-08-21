#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static bool lights[1000][1000] = {0};

char input[] = "./input.txt";

typedef enum { ON, OFF, TOGGLE } State;

int set_lights(int a, int b, int c, int d, State state) {
    for (int x = a; x <= c; x++) {
        for (int y = b; y <= d; y++) {
            switch (state) {
            case ON:
                lights[x][y] = 1;
                break;
            case OFF:
                lights[x][y] = 0;
                break;
            case TOGGLE:
                lights[x][y] = !lights[x][y];
                break;
            }
        }
    }
    return 0;
}
int count_lights() {
    uint32_t total = 0;
    for (int x = 0; x < 1000; x++) {
        for (int y = 0; y < 1000; y++) {
            total += lights[x][y];
        }
    }
    return total;
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
        int a, b, c, d;
        switch (buffer[1]) {
        case 'u':
            switch (buffer[6]) {
            case 'n':
                sscanf(&buffer[8], "%d,%d through %d, %d", &a, &b, &c, &d);
                set_lights(a, b, c, d, ON);
                printf("ON %d, %d, %d, %d\n", a, b, c, d);
                break;
            case 'f':
                sscanf(&buffer[9], "%d,%d through %d, %d", &a, &b, &c, &d);
                set_lights(a, b, c, d, OFF);
                printf("OFF %d, %d, %d, %d\n", a, b, c, d);
                break;
            }
            break;
        case 'o':
            sscanf(&buffer[7], "%d,%d through %d, %d", &a, &b, &c, &d);
            set_lights(a, b, c, d, TOGGLE);
            printf("TOGGLE %d, %d, %d, %d\n", a, b, c, d);
            break;
        }
    }
    printf("Total = %d\n", count_lights());
}
