#include <openssl/md5.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OPENSSL_API_COMPAT
char secret[] = "yzbqklnj";
char secretAppended[100];

int main() {
    char *fullkey = secret;
    uint32_t secretlen = strlen(secret);
    uint64_t number = 123;
    u_char digest[MD5_DIGEST_LENGTH];
    char output[MD5_DIGEST_LENGTH * 2 + 1];
    while (1) {
        sprintf(secretAppended, "%s%ld", secret, number);
        printf("secret is %s\n", secretAppended);
        MD5((u_char *)secretAppended, strlen(secretAppended), digest);
        if (digest[0] != 0) {
            number++;
            continue;
        }
        if (digest[1] != 0) {
            number++;
            continue;
        }
        if (digest[2] != 0) {
            number++;
            continue;
        }
        break;
    }
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        sprintf(&output[i * 2], "%02x", digest[i]);
    }
    output[MD5_DIGEST_LENGTH * 2] = '\0';

    printf("Input: %s\n", secretAppended);
    printf("MD5: %s\n", output);
    return 0;
}
