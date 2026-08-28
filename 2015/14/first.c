#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char input[] = "./input.txt";

uint32_t time_limit = 2503;

int main() {
    FILE *fptr = fopen(input, "r");
    if (fptr == NULL) {
        printf("Failed to open file %s\n", input);
        return 1;
    }

    char buffer[512] = {0};

    uint32_t largest_distance = 0;
    while (fgets(buffer, 512, fptr) != NULL) {
        int speed = 0, time = 0, rest = 0;
        // Vixen can fly 8 km/s for 8 seconds, but then must rest for 53 seconds.
        int matched = sscanf(buffer, "%*s can fly %d km/s for %d seconds, but then must rest for %d seconds.", &speed, &time, &rest);
        printf("%d, %d, %d, %d\n", matched, speed, time, rest);
        printf("%s", buffer);
        uint32_t time_passed = 0;
        int distance = 0;
        while (time_passed <= time_limit) {
            if ((time_passed + time) <= time_limit) {
                time_passed += time;      // increment time passed
                distance += speed * time; // increment our speed
            } else {                      // if we cant fly the entire duration
                uint32_t dif = time_limit - time_passed;
                distance += speed * dif;
                printf("this\n");
                break;
            }
            time_passed += rest;
            printf("time passed = %d   %d\n", time_passed, distance);
        }
        printf("distance = %d, distance\n", distance);
        if (distance > largest_distance) {
            largest_distance = distance;
        }
    }
    printf("largest = %d\n", largest_distance);
    fclose(fptr);
}
