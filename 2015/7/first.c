#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// i think of this as a depth first search.  we want to know the result of a.
// Therefore we make a "inverse" linked list, or something like struct (dependency*, dependency*, enum, output_val);
typedef enum { AND, OR, RSHIFT, LSHIFT, NOT, INIT } STATE;
struct operation {
    struct operation *dep1;
    struct operation *dep2;
    STATE op_type;
    uint32_t shift_amount;
    uint16_t val;
    struct operation *out;
};
char input[] = "./input.txt";
// just to be shure,  just index with [a][b] to get value or [a][0] to get single value
struct operation opperations[256][256] = {0};

int build_dependencies() {
    FILE *fptr = fopen(input, "r");

    if (fptr == NULL) {
        printf("failed to open file\n");
    }
    fseek(fptr, 0, SEEK_END);
    uint32_t size = ftell(fptr);
    rewind(fptr);
    char buffer[256] = {0};
    while (fgets(buffer, 256, fptr) != NULL) {
        char x, y;
        struct operation tempOP = {0};
        int i = 0;
        while (buffer[i] != '\n') {
            i++;
        }
        i -= 1;
        if (buffer[i - 1] == ' ') {
            y = 0;
            x = buffer[i];
        } else {
            y = buffer[i];
            x = buffer[i - 1];
        }

        uint32_t offset = 0;
        // The not case
        if (buffer[0] == 'N') {
            tempOP.op_type = NOT;
            if (buffer[5] == ' ') {
                buffer[5] = 0;
            }
            tempOP.dep1 = &opperations[buffer[4]][buffer[5]];
            opperations[x][y] = tempOP;
            printf("OP=NOT\n");
            continue;
        }
        // Handle number case.
        if (buffer[0] >= 48 && buffer[0] <= 57) {
            // should be a number now
            tempOP.dep1 = NULL;
            char *endptr;
            tempOP.shift_amount = strtol(buffer, &endptr, 10);
            printf("%d NUM ", tempOP.shift_amount);
            offset = endptr - buffer - 1;
        } else {
            if (buffer[1] == ' ') {
                tempOP.dep1 = &opperations[buffer[0]][0];
                offset = 0;
            } else {
                tempOP.dep1 = &opperations[buffer[0]][buffer[1]];
                offset = 1;
            }
        }
        switch (buffer[offset + 2]) {
        case 'R': {
            tempOP.op_type = RSHIFT;
            offset += 9;
            printf("OP=RSHIFT\n");
            break;
        }
        case 'L': {
            tempOP.op_type = LSHIFT;
            offset += 9;
            printf("OP=LSHIFT\n");
            break;
        }
        case 'O': {
            tempOP.op_type = OR;
            offset += 3;
            printf("OP=OR\n");
            break;
        }
        case 'A': {
            tempOP.op_type = AND;
            offset += 4;
            printf("OP=AND\n");
            break;
        }
        case '-': {
            tempOP.op_type = INIT;
            printf("OP=INIT\n");
            tempOP.val = tempOP.shift_amount;
        }
        }
        // Now offset should point to dep 2. which is either a number or letters
        // handles the number case first
        if (tempOP.op_type == LSHIFT || tempOP.op_type == RSHIFT) {
            printf("string %s", &buffer[offset]);
            sscanf(&buffer[offset], " %d ", &tempOP.shift_amount);
            printf("to shift %d\n", tempOP.shift_amount);
        } else if (buffer[offset + 1] == ' ') {
            tempOP.dep2 = &opperations[buffer[offset]][0];
        } else {
            tempOP.dep2 = &opperations[buffer[offset]][buffer[offset + 1]];
        }
        tempOP.out = &opperations[x][y];
        opperations[x][y] = tempOP;
    }
}
int solve_dependencies(char a, char b) {
    struct operation goal = opperations[a][b];
    struct operation current = goal;
    switch (current.op_type) {
    case AND: {
        printf("OP TYPE AND\n");
        break;
    }
    case OR: {
        printf("OP TYPE OR\n");
        break;
    }
    case RSHIFT: {
        printf("OP TYPE RS\n");
        break;
    }
    case LSHIFT: {
        printf("OP TYPE LS\n");
        break;
    }
    case INIT: {
        printf("OP TYPE INIT\n");
        break;
    }
    case NOT: {
        printf("OP TYPE NOT\n");
        break;
    }
        printf("VAL = %d\n", current.val);

        return 0;
    }
}
int main() {
    build_dependencies();
    printf("\n\n\n");
    solve_dependencies('a', 0);
}
