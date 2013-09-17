#include "liji.h"
#include <stdio.h>
#include <string.h>

#define JSON "{\"test\":\"value\", \"other{\": {\"s]omething\": \"aaa\", \"bbbb\": 1}, \"yet_another\": 999 }"

void test_simple_find(char *json, char *to_find, char *expected) {
    char *result_start = 0;
    int len = 0, expected_len = strlen(expected);

    liji_find(json, to_find, strlen(to_find), &result_start, &len);

    if (expected_len == len && memcmp(result_start, expected, len) == 0) {
        printf("Yay!\n");
    } else {
        printf("Nope: `%s`\n", strndup(result_start, len));
    }
}

int main(int argc, char *argv[]) {
    test_simple_find(JSON, "test", "\"value\"");
    test_simple_find(JSON, "other{", "{\"s]omething\": \"aaa\", \"bbbb\": 1}");
    test_simple_find(JSON, "yet_another", "999");

    return 0;
}
