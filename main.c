#include "liji.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define JSON "{ \"test\":\"value\", \"other{\": {\"s]omething\": \"aaa\", \"bbbb\": 1}, \"yet_another\":    999   , \"deep\":{\"really\":{\"deep\":7}}, \"with\\\"quoted\": 66, \"after_quoted\": \"ok\", \"value_quoted\": \"quo\\\"ted\"}"

#define ASSERT_EQ(str1, len1, str2, len2) \
    if (len1 == len2 && memcmp(str1, str2, len1) == 0) { \
        printf("Yay!\n"); \
    } else { \
        printf("Nope: '%s' - Expected: '%s'\n", strndup(str1, len1), strndup(str2, len2)); \
    }

void test_simple_find(char *json, char *to_find, char *expected) {
    char *result_start = 0;
    int len = 0, expected_len = strlen(expected);

    liji_find(json, to_find, strlen(to_find), &result_start, &len);

    ASSERT_EQ(result_start, len, expected, expected_len)
}

void test_find_multi(char *json, char *to_find[], int number_of_keys, char *expected) {
    char *result_start = 0;
    int len = 0, expected_len = strlen(expected);

    int *sizes = (int*)malloc(sizeof(int) * number_of_keys);
    for (int i = 0; i < number_of_keys; ++i) {
        sizes[i] = strlen(to_find[i]);
    }
    liji_find_multi(json, (const char**)to_find, sizes, number_of_keys, &result_start, &len);

    ASSERT_EQ(result_start, len, expected, expected_len)
}

int main(int argc, char *argv[]) {
    test_simple_find(JSON, "yet_another", "999");
    test_simple_find(JSON, "test", "\"value\"");
    test_simple_find(JSON, "s]omething", "\"aaa\"");
    test_simple_find(JSON, "other{", "{\"s]omething\": \"aaa\", \"bbbb\": 1}");
    test_simple_find(JSON, "deep", "{\"really\":{\"deep\":7}}");
    test_simple_find(JSON, "with\\\"quoted", "66");
    test_simple_find(JSON, "after_quoted", "\"ok\"");
    test_simple_find(JSON, "value_quoted", "\"quo\\\"ted\"");
    test_find_multi(JSON, (char *[]){ "other{", "s]omething" }, 2, "\"aaa\"");
    test_find_multi(JSON, (char *[]){ "other{", "bbbb" }, 2, "1");
    test_find_multi(JSON, (char *[]){ "other{", "yet_another" }, 2, "");
    test_find_multi(JSON, (char *[]){ "deep", "really", "deep" }, 3, "7");

    return 0;
}
