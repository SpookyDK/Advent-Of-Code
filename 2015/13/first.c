#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char input[] = "./input.txt";

struct relation {
    uint64_t affectedGuest;
    uint64_t affectingGuest;
    int32_t amount;
};
struct person {
    uint64_t name;
};

int calculateScore(struct person *persons, int p, struct relation *relations, int r) {
    int64_t temp_score = 0;
    int matches = 0;
    for (int i = 0; i < r; i++) { // we start by looking at the relation between person 0 and -1/8
        if (relations[i].affectingGuest == persons[0].name && relations[i].affectedGuest == persons[p - 1].name) { // case1
            temp_score += relations[i].amount;
            matches++;
        }
        if (relations[i].affectedGuest == persons[0].name && relations[i].affectingGuest == persons[p - 1].name) { // case2
            temp_score += relations[i].amount;
            matches++;
        }
    }
    // printf("mathes = %d\n", matches);
    for (int j = 1; j < p; j++) {
        matches = 0;
        for (int i = 0; i < r; i++) { // we start by looking at the relation between person 0 and -1/8
            if (relations[i].affectingGuest == persons[j - 1].name && relations[i].affectedGuest == persons[j].name) { // case1
                temp_score += relations[i].amount;
            }
            if (relations[i].affectedGuest == persons[j - 1].name && relations[i].affectingGuest == persons[j].name) { // case2
                temp_score += relations[i].amount;
            }
        }
        // printf("mathes = %d\n", matches);
    }
    return temp_score;
}
int its = 0;
int score = 0;
// implementation of heaps algorithm, to avoid the mess i made a couple of advents ago, which was basicly this..
void printPermutations(struct person *arr, int n, struct relation *relations, int r) {
    int *c = calloc(n, sizeof(int));
    int temp_score = calculateScore(arr, n, relations, r);
    if (temp_score > score) {
        score = temp_score;
    }
    its++;

    int i = 0;
    while (i < n) {
        if (c[i] < i) {
            if (i % 2 == 0) {
                struct person temp = arr[0];
                arr[0] = arr[i];
                arr[i] = temp;
            } else {
                struct person temp = arr[c[i]];
                arr[c[i]] = arr[i];
                arr[i] = temp;
            }

            for (int i = 0; i < n; i++) {
                printf("%.8s\n", (char *)&arr[i].name);
            }
            int temp_score = calculateScore(arr, n, relations, r);
            if (temp_score > score) {
                score = temp_score;
            }
            printf("\n");
            its++;
            c[i]++;
            i = 0;
        } else {
            c[i] = 0;
            i++;
        }
    }
    free(c);
}
int main() {

    FILE *fptr = fopen(input, "r");
    if (fptr == NULL) {
        printf("Failed to open file %s\n", input);
        return 1;
    }
    uint32_t buffersize = 512;

    char buffer[buffersize];
    uint32_t lines = 0;
    while (fgets(buffer, buffersize, fptr) != NULL) {
        lines++;
    }
    rewind(fptr);
    uint16_t PersonCount = (1 + sqrtf(1 + 4 * lines)) / 2; // a bit cheating. would not actually work i think
    printf("Found %d persons\n", PersonCount);
    struct person *global_persons = calloc(PersonCount, sizeof(struct person));
    struct relation *global_relations = calloc(lines, sizeof(struct relation));

    int relationCount = 0;
    while (fgets(buffer, buffersize, fptr) != NULL) {
        char affectedGuest[64] = {0};
        char affectingGuest[64] = {0};
        char type[8];
        uint32_t amount = 0;
        // Alice would gain 54 happiness units by sitting next to Bob.
        sscanf(buffer, "%s would %s %d happiness units by sitting next to %[^.].", affectedGuest, type, &amount, affectingGuest);
        printf("%s, %s, %d, %s\n", affectedGuest, type, amount, affectingGuest);
        uint64_t affectedGuestInt;
        uint64_t affectingGuestInt;
        memcpy(&affectedGuestInt, affectedGuest, sizeof(affectedGuestInt));
        memcpy(&affectingGuestInt, affectingGuest, sizeof(affectingGuestInt));
        if (type[0] == 'l') {
            amount = amount * -1;
        }
        struct relation temp = {.affectedGuest = affectedGuestInt, .affectingGuest = affectingGuestInt, .amount = amount};
        global_relations[relationCount] = temp;
        for (int i = 0; i < PersonCount; i++) { // look for affected quest
            if (global_persons[i].name == affectedGuestInt) {
                break;
            } else if (global_persons[i].name == 0) {
                global_persons[i].name = affectedGuestInt;
                break;
            }
        }
        for (int i = 0; i < PersonCount; i++) { // look for affecting quest
            if (global_persons[i].name == affectingGuestInt) {
                break;
            } else if (global_persons[i].name == 0) {
                global_persons[i].name = affectingGuestInt;
                break;
            }
        }
        relationCount++;
    }

    printPermutations(global_persons, PersonCount, global_relations, relationCount);
    for (int i = 0; i < PersonCount; i++) {
        printf("%ld\n", global_persons[i].name);
    }
    printf("\n");
    for (int i = 0; i < relationCount; i++) {
        printf("%ld, %ld\n", global_relations[i].affectedGuest, global_relations[i].affectingGuest);
    }
    printf("Its = %d, Biggest score = %d\n", its, score);
    fclose(fptr);
    free(global_relations);
    free(global_persons);
    return 0;
}
