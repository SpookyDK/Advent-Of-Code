#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef enum { AND, OR, RSHIFT, LSHIFT, NOT, INIT } STATE;
struct operation {
    struct operation *dep1;
    struct operation *dep2;
    STATE op_type;
    uint32_t shift_amount;
    uint16_t val;
    bool evaluated;
    struct operation *previous;
    uint32_t line;
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
    uint32_t currentline = 1;
    while (fgets(buffer, 256, fptr) != NULL) {
        char x, y;
        struct operation tempOP = {0};
        tempOP.line = currentline;
        tempOP.evaluated = false;
        currentline++;
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
                tempOP.dep1 = &opperations[buffer[4]][0];
                printf("OP=NOT, dep1=%c%c", buffer[4], '0');
                printf("ptr = %p\n", tempOP.dep1);
            } else {
                tempOP.dep1 = &opperations[buffer[4]][buffer[5]];
                printf("OP=NOT, dep11=%c%c", buffer[4], buffer[5]);
                printf("ptr = %p\n", tempOP.dep1);
            }
            opperations[x][y] = tempOP;
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
                printf("dep11 = %c%c ", buffer[0], '0');
                printf("ptr = %p", tempOP.dep1);
                offset = 0;
            } else {
                tempOP.dep1 = &opperations[buffer[0]][buffer[1]];
                offset = 1;
                printf("dep12 = %c%c ", buffer[0], buffer[1]);
                printf("ptr = %p", tempOP.dep1);
            }
        }
        switch (buffer[offset + 2]) {
        case 'R': {
            tempOP.op_type = RSHIFT;
            offset += 9;
            printf("OP=RSHIFT ");
            break;
        }
        case 'L': {
            tempOP.op_type = LSHIFT;
            offset += 9;
            printf("OP=LSHIFT  ");
            break;
        }
        case 'O': {
            tempOP.op_type = OR;
            offset += 5;
            printf("OP=OR  ");
            break;
        }
        case 'A': {
            tempOP.op_type = AND;
            offset += 6;
            printf("OP=AND  ");
            break;
        }
        case '-': {
            tempOP.op_type = INIT;
            printf("OP=INIT  ");
        }
        }
        // Now offset should point to dep 2. which is either a number or letters
        // handles the number case first
        if (tempOP.op_type == INIT) {
            tempOP.dep2 = NULL;
            printf("deb2 = NULL\n ");
        } else if (tempOP.op_type == LSHIFT || tempOP.op_type == RSHIFT) {
            // printf("string %s", &buffer[offset]);
            sscanf(&buffer[offset], " %d ", &tempOP.shift_amount);
            // printf("to shift %d\n", tempOP.shift_amount);
            tempOP.dep2 = NULL;
            printf("deb2 = NULL\n");
        } else if (buffer[offset + 1] == ' ') {
            tempOP.dep2 = &opperations[buffer[offset]][0];
            printf("deb21 = %c%c\n", buffer[offset], '0');
        } else {
            tempOP.dep2 = &opperations[buffer[offset]][buffer[offset + 1]];
            printf("deb2 = %c%c\n", buffer[offset], buffer[offset + 1]);
        }
        opperations[x][y] = tempOP;
    }
}
int solve_dependencies(char a, char b) {
    struct operation *goal = &opperations[a][b];
    struct operation *current = goal;
    while (!goal->evaluated) {
        printf("line %d prt = %p\n", opperations['a']['c'].line, &opperations['a']['c'].dep1);
        printf("line = %d\n", current->line);
        printf("Add = %p\n", current);

        if (!current->evaluated) {
            printf("%p,%p\n", current->dep1, current->dep2);
            if (current->dep1 != NULL && !current->dep1->evaluated) {
                printf("current=dep1 %d\n", current->dep1->line);
                current->dep1->previous = current;
                current = current->dep1;
                continue;
            }
            if (current->dep2 != NULL && !current->dep2->evaluated) {
                printf("current=dep2\n");
                current->dep2->previous = current;
                current = current->dep2;
                continue;
            }
            printf("Both deps evaluate\n");
            printf("test\n");
            uint32_t val1, val2;
            if (current->dep1 == NULL) {
                val1 = current->shift_amount;
                printf("applyting shift as val\n");
            } else if (current->dep1 != NULL && current->op_type == INIT) {
                printf("not applyting shift\n");
                val1 = current->dep1->val;

            } else if (current->dep1 != NULL) {
                printf("dep1case2\n");
                val1 = current->dep1->val;
            }

            if (current->dep2 == NULL) {
                val2 = current->shift_amount;
                printf("dep2case1\n");

            } else if (current->dep2->evaluated) {
                val2 = current->dep2->val;
                printf("dep2case2\n");
            }
            switch (current->op_type) {
            case AND: {
                current->val = val1 & val2;
                printf("OP TYPE AND %d, %d\n", val1, val2);
                break;
            }
            case OR: {
                current->val = val1 | val2;
                printf("OP TYPE OR %d, %d\n", val1, val2);
                break;
            }
            case RSHIFT: {
                current->val = val1 >> val2;
                printf("OP TYPE RS %d, %d\n", val1, val2);
                break;
            }
            case LSHIFT: {
                current->val = val1 << val2;
                printf("OP TYPE LS %d, %d\n", val1, val2);
                break;
            }
            case INIT: {
                current->val = val1;
                printf("OP TYPE INIT %d, %d\n", val1, val2);
                break;
            }
            case NOT: {
                current->val = ~current->dep1->val;
                printf("OP TYPE NOT %d\n", val1);
                break;
            }
            }
            current->evaluated = true;
            printf("VAL = %d\n", current->val);
        } else {
            printf("Already Evaluated\n");
            printf("Pptr %p\n", current->previous);
            current = current->previous;
        }
    }
    return goal->val;
}
int main() {
    build_dependencies();
    printf("\n\n\n");
    solve_dependencies('a', 0);
}
