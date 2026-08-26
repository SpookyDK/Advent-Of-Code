#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char input[] = "./input.txt";
struct city {
    uint32_t id; // lets assume the first 4 letters are unique in each city
    bool visited;
};
struct connection {
    struct city *start;
    struct city *end;
    uint16_t distance;
};

int connectionCount = 0;
int main() {
    FILE *fptr = fopen(input, "r");

    if (fptr == NULL) {
        printf("Failed to open file\n");
    }

    char buffer[256];
    // lets assume all cities connect, so connections = lines
    uint32_t lines = 0;
    while (fgets(buffer, 256, fptr) != NULL) {
        lines++;
    }
    connectionCount = lines;
    struct connection connections[connectionCount];
    uint16_t cityCount = (1 + sqrtf(1 + 8 * lines)) / 2;
    struct city cities[8] = {0};
    printf("lines %d\n", lines);
    printf("cities %d\n", cityCount);

    rewind(fptr);
    lines = 0;
    while (fgets(buffer, 256, fptr) != NULL) {
        // start by looking at the first city
        struct city *first;
        uint32_t id;
        memcpy(&id, buffer, sizeof(uint32_t));
        for (int c = 0; c < cityCount; c++) {
            if (cities[c].id == 0) {
                cities[c].id = id;
                first = &cities[c];
                break;
            }
            if (cities[c].id == id) {
                first = &cities[c];
                break;
            }
        }
        // Now we look for the second city by looking for a space;
        int i = 0;
        struct city *second;
        while (buffer[i] != '\n') {
            if (buffer[i] == ' ') {
                i += 4;
                memcpy(&id, &buffer[i], sizeof(uint32_t));
                for (int c = 0; c < cityCount; c++) {
                    if (cities[c].id == 0) {
                        cities[c].id = id;
                        second = &cities[c];
                        break;
                    }
                    if (cities[c].id == id) {
                        second = &cities[c];
                        break;
                    }
                }
                break;
            }
            i++;
        }
        // now we look for the distance by searching for the =
        uint32_t length;
        while (buffer[i] != '\n') {
            if (buffer[i] == '=') {
                i += 2;
                length = atoi(&buffer[i]);
                break;
            }
            i++;
        }
        struct connection temp = {.start = first, .end = second, .distance = length};
        connections[lines] = temp;
        lines++;
        printf("%d\n", length);
    }
    printf("## Cities ##\n");
    for (int i = 0; i < cityCount; i++) {
        printf("city %.4s\n", (char *)&cities[i].id);
    }

    printf("## CONNECTIONS ##\n");
    for (int i = 0; i < connectionCount; i++) {
        printf("%d, %.4s, %d, %.4s\n", i, (char *)&connections[i].start->id, connections[i].distance, (char *)&connections[i].end->id);
    }

    // Lets start by keeping track of the cities we have visited
    printf("break\n");
    struct city *visitedCities[cityCount];
    struct city *visitedCitiesShortest[cityCount];
    uint32_t offsets[cityCount];
    uint32_t total_combs = 0;
    uint32_t min_distance = 0b1111111111111111111111111111111;
    for (int i = 0; i < cityCount; i++) {
        for (int k = 0; k < cityCount; k++) {
            cities[k].visited = false;
            offsets[k] = 0;
        }
        visitedCities[0] = &cities[i];
        visitedCities[0]->visited = true;
        printf("###Starting city = %.4s\n", (char *)&cities[i].id);
        int16_t depth = 1;
        while (depth != 0) {
            bool found_new_city = true;
            while (found_new_city) {
                found_new_city = false;
                while (offsets[depth] < cityCount) { // now we loop through all cities checking if they are not visited.
                    printf("eval %d, %.4s ", offsets[depth], (char *)&cities[offsets[depth]].id);
                    if (cities[offsets[depth]].visited == false) {      // if the city we find isnt visited
                        cities[offsets[depth]].visited = true;          // set the city to visited
                        visitedCities[depth] = &cities[offsets[depth]]; // set the city in our visited list
                        found_new_city = true;
                        printf("Adding city = %.4s\n", (char *)&cities[offsets[depth]].id);

                        offsets[depth] += 1;
                        depth += 1;
                        break;
                    }
                    offsets[depth] += 1;
                }
            }
            printf("No new cities to explore\n");
            if (depth >= 0 && depth < cityCount) {
                offsets[depth] = 0; // resets our offset so we can scan again later.
                printf("depth reset %d\n", depth);
            }
            for (int j = 0; j < cityCount; j++) {
                printf(" %.4s, %d :", (char *)&visitedCities[j]->id, visitedCities[j]->visited);
            }
            if (depth == cityCount - 1) { // only count when we are at last level
                total_combs += 1;
                uint32_t total_distance = 0;
                for (int d = 1; d < cityCount; d++) {
                    bool found = false;
                    for (int i = 0; i < connectionCount; i++) {
                        if ((connections[i].end == visitedCities[d] || connections[i].start == visitedCities[d]) &&
                            (connections[i].end == visitedCities[d - 1] || connections[i].start == visitedCities[d - 1])) {
                            printf("Found path %d, %.4s, %.4s\n", connections[i].distance, (char *)&connections[i].start->id,
                                   (char *)&connections[i].end->id);
                            total_distance += connections[i].distance;
                            printf("distance %d\n", connections[i].distance);
                            found = true;
                        }
                    }
                    printf("found %d", found);
                }
                printf("Path distance =%d\n", total_distance);
                if (total_distance < min_distance) {
                    min_distance = total_distance;
                    memcpy(visitedCitiesShortest, visitedCities, sizeof(visitedCities));
                }
            }
            printf("D=%d, OD=%d\n", depth, offsets[depth]);
            for (int j = depth - 1; j < cityCount; j++) {
                visitedCities[j]->visited = false;
            }
            depth += -1; // goes a step back.
            printf("\n");
        }
    }
    printf("Total combs = %d\n", total_combs);
    printf("smallest distance = %d\n", min_distance);
    for (int j = 0; j < cityCount; j++) {
        printf(" %.4s, %d :", (char *)&visitedCitiesShortest[j]->id, visitedCitiesShortest[j]->visited);
    }
}
