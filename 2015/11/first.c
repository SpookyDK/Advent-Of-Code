#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

char original_passw[8] = "hepxxyzz";

int increment_passwd(char buffer[8]) {

    uint8_t index = 7;
    while (index != 8) {
        if (buffer[index] == 'z') {
            buffer[index] = 'a';
            index += -1;
        } else {
            buffer[index] += 1;
            index = 8;
        }
    }
    return 0;
}

bool check_passwd(char buffer[8]) {
    bool has_abc = false;
    for (int i = 0; i < 6; i++) {
        if (buffer[i] + 1 == (buffer[i + 1]) && buffer[i] + 2 == buffer[i + 2]) {
            has_abc = true;
            break;
        }
    }
    if (!has_abc) {
        return 0;
    }
    for (int i = 0; i < 8; i++) {
        switch (buffer[i]) {
        case 'i': {
            return 0;
            break;
        }
        case 'o': {
            return 0;
            break;
        }
        case 'l': {
            return 0;
        }
        }
    }

    uint8_t doubleLetters = 0;
    for (int i = 0; i < 7; i++) {
        if (buffer[i] == buffer[i + 1]) {
            doubleLetters++;
            i++;
            continue;
        }
    }
    if (doubleLetters >= 2) {
        return true;
    }
    return false;
}

int main() {
    char new_passwd[8];
    memcpy(new_passwd, original_passw, 8);
    int i = 0;
    while (1) {
        increment_passwd(new_passwd);
        bool check = check_passwd(new_passwd);
        printf("%d, %d,    %.8s\n", i, check, new_passwd);
        if (check) {
            break;
        }
        i++;
    }
}
