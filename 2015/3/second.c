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
int32_t SantacurrentX = 0;
int32_t SantacurrentY = 0;
int32_t RobotcurrentY = 0;
int32_t RobotcurrentX = 0;
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
                SantacurrentY += 1;
                break;

            case 'v':
                total_moves.down++;
                SantacurrentY -= 1;
                break;
            case '<':
                total_moves.left++;
                SantacurrentX -= 1;
                break;
            case '>':
                total_moves.right++;
                SantacurrentX += 1;
                break;
            }

            if (SantacurrentX < maxNegX) {
                maxNegX = SantacurrentX;
            }
            if (SantacurrentX > maxPosX) {
                maxPosX = SantacurrentX;
            }
            if (SantacurrentY < maxNegY) {
                maxNegY = SantacurrentY;
            }
            if (SantacurrentY > maxPosY) {
                maxPosY = SantacurrentY;
            }
            i++;
            switch (buffer[i]) {
            case '^':
                total_moves.up++;
                RobotcurrentY += 1;
                break;

            case 'v':
                total_moves.down++;
                RobotcurrentY -= 1;
                break;
            case '<':
                total_moves.left++;
                RobotcurrentX -= 1;
                break;
            case '>':
                total_moves.right++;
                RobotcurrentX += 1;
                break;
            }

            if (RobotcurrentX < maxNegX) {
                maxNegX = RobotcurrentX;
            }
            if (RobotcurrentX > maxPosX) {
                maxPosX = RobotcurrentX;
            }
            if (RobotcurrentY < maxNegY) {
                maxNegY = RobotcurrentY;
            }
            if (RobotcurrentY > maxPosY) {
                maxPosY = RobotcurrentY;
            }
        }
    }
    rewind(fptr);
    uint32_t rowCount, columnCount;
    rowCount = maxPosY - maxNegY + 20;    // 20 extra padding
    columnCount = maxPosX - maxNegX + 20; // 20 extra padding
    uint32_t table[rowCount][columnCount];
    for (int x = 0; x < rowCount; x++) {
        for (int y = 0; y < columnCount; y++) {
            table[x][y] = 0;
        }
    }
    table[0 - maxNegY + 10][0 - maxNegX + 10] = 1;
    SantacurrentY = 0;
    SantacurrentX = 0;

    RobotcurrentY = 0;
    RobotcurrentX = 0;
    read = 0;
    while (read < size) {
        uint64_t current_read = fread(buffer, sizeof(char), 256, fptr);
        read += current_read;
        for (int i = 0; i < current_read; i++) {
            printf("%c", buffer[i]);
            switch (buffer[i]) {
            case '^':
                SantacurrentY += 1;
                break;

            case 'v':
                SantacurrentY -= 1;
                break;
            case '<':
                SantacurrentX -= 1;
                break;
            case '>':
                SantacurrentX += 1;
                break;
            }
            printf("X:%d;Y:%d\n", SantacurrentX - maxNegX + 10, SantacurrentY - maxNegY + 10);
            table[SantacurrentY - maxNegY + 10][SantacurrentX - maxNegX + 10] += 1;
            i++;
            switch (buffer[i]) {
            case '^':
                RobotcurrentY += 1;
                break;

            case 'v':
                RobotcurrentY -= 1;
                break;
            case '<':
                RobotcurrentX -= 1;
                break;
            case '>':
                RobotcurrentX += 1;
                break;
            }
            printf("X:%d;Y:%d\n", RobotcurrentX - maxNegX + 10, RobotcurrentY - maxNegY + 10);
            table[RobotcurrentY - maxNegY + 10][RobotcurrentX - maxNegX + 10] += 1;
        }
    }
    uint32_t total_unique = 0;
    for (int x = 0; x < rowCount; x++) {
        for (int y = 0; y < columnCount; y++) {
            if (table[x][y] != 0) {
                total_unique++;
                printf(GREEN "%d." RESET, table[x][y]);
            } else {
                printf(RED "%d." RESET, table[x][y]);
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
