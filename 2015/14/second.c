#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct reindeer {
    int distance;
    int points;
    int speed;
    int sprint_time;
    int rest_time;
    int time_in_action;
    bool resting;
};
struct reindeer reindeers[16];
char input[] = "./input.txt";

uint32_t time_limit = 2503;

int main() {
    FILE *fptr = fopen(input, "r");
    if (fptr == NULL) {
        printf("Failed to open file %s\n", input);
        return 1;
    }

    char buffer[512] = {0};

    uint32_t current_deer = 0;
    while (fgets(buffer, 512, fptr) != NULL) {
        int speed = 0, time = 0, rest = 0;
        // Vixen can fly 8 km/s for 8 seconds, but then must rest for 53 seconds.
        int matched = sscanf(buffer, "%*s can fly %d km/s for %d seconds, but then must rest for %d seconds.", &speed, &time, &rest);
        printf("%d, %d, %d, %d\n", matched, speed, time, rest);
        printf("%s", buffer);
        reindeers[current_deer] = (struct reindeer){
            .distance = 0, .points = 0, .speed = speed, .sprint_time = time, .rest_time = rest, .time_in_action = 0, .resting = false};
        current_deer++;
    }
    for (int t = 0; t < time_limit; t++) {
        uint32_t furthest_index = 0;
        uint32_t furthest_distance = 0;
        for (int i = 0; i < current_deer; i++) {
            if (reindeers[i].resting) {
                reindeers[i].time_in_action++;
                if (reindeers[i].time_in_action == reindeers[i].rest_time) {
                    reindeers[i].resting = false;
                    reindeers[i].time_in_action = 0;
                }

            } else if (!reindeers[i].resting) {
                reindeers[i].distance += reindeers[i].speed;
                reindeers[i].time_in_action++;
                if (reindeers[i].time_in_action == reindeers[i].sprint_time) {
                    reindeers[i].resting = true;
                    reindeers[i].time_in_action = 0;
                }
            }
            if (reindeers[i].distance > furthest_distance) {
                furthest_distance = reindeers[i].distance;
                furthest_index = i;
            }
        }
        reindeers[furthest_index].points += 1;
    }
    for (int i = 0; i < current_deer; i++) {
        printf("i = %d, points = %d\n", i, reindeers[i].points);
    }
    fclose(fptr);
}
