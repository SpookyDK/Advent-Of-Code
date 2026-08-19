#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define RESET "\x1b[0m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define CYAN "\x1b[36m"
char filepath[] = "./input.txt";
struct moves {
    uint32_t up, down, right, left;
};
struct moves total_moves;
int32_t maxPosX = 0;
int32_t maxNegX = 0;
int32_t maxPosY = 0;
int32_t maxNegY = 0;
int32_t currentX;
int32_t currentY;

int main() {

    FILE *fptr = fopen(filepath, "r");
    fseek(fptr, 0, SEEK_END);
    uint64_t size = ftell(fptr);
    rewind(fptr);
    char buffer[256];
    uint64_t read = 0;
    while (read < size) {
        uint64_t current_read = fread(buffer, sizeof(char), 256, fptr);
        read += current_read;
        for (int i = 0; i < current_read; i++) {
            printf("%c", buffer[i]);
            switch (buffer[i]) {
            case '^':
                total_moves.up++;
                currentY += 1;
                break;

            case 'v':
                total_moves.down++;
                currentY -= 1;
                break;
            case '<':
                total_moves.left++;
                currentX -= 1;
                break;
            case '>':
                total_moves.right++;
                currentX += 1;
                break;
            }

            if (currentX < maxNegX) {
                maxNegX = currentX;
            }
            if (currentX > maxPosX) {
                maxPosX = currentX;
            }
            if (currentY < maxNegY) {
                maxNegY = currentY;
            }
            if (currentY > maxPosY) {
                maxPosY = currentY;
            }
        }
    }
    rewind(fptr);
    uint32_t rowCount, columnCount;
    rowCount = maxPosY - maxNegY;
    columnCount = maxPosX - maxNegX;
    uint32_t table[rowCount][columnCount];
    for (int x = 0; x < rowCount; x++) {
        for (int y = 0; y < columnCount; y++) {
            table[x][y] = 0;
        }
    }
    currentY = 0;
    currentX = 0;

    read = 0;
    while (read < size) {
        uint64_t current_read = fread(buffer, sizeof(char), 256, fptr);
        read += current_read;
        for (int i = 0; i < current_read; i++) {
            printf("%c", buffer[i]);
            switch (buffer[i]) {
            case '^':
                currentY += 1;
                break;

            case 'v':
                currentY -= 1;
                break;
            case '<':
                currentX -= 1;
                break;
            case '>':
                currentX += 1;
                break;
            }
            printf("X:%d;Y:%d\n", currentX - maxNegX, currentY - maxNegY);
            table[currentY - maxNegY][currentX - maxNegX] += 1;
        }
    }
    uint32_t total_unique = 1;
    for (int x = 0; x < rowCount; x++) {
        for (int y = 0; y < columnCount; y++) {
            if (table[x][y] != 0) {
                total_unique++;
                printf(GREEN ".%d." RESET, table[x][y]);
            } else {
                printf(RED ".%d." RESET, table[x][y]);
            }
        }
        printf("\n");
    }
    printf("Total moves UP:%d, DOWN:%d, RIGHT:%d, LEFT:%d\n", total_moves.up, total_moves.down, total_moves.right, total_moves.left);
    printf("Max: +X %d -X %d +Y %d -Y %d\n", maxPosX, maxNegX, maxPosY, maxNegY);
    printf("unique = %d\n", total_unique);
    fclose(fptr);

    return 0;
}
