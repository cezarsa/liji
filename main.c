#include "liji.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ASSERT_STR_EQ(str1, len1, str2, len2, line) \
    if (len1 == len2 && memcmp(str1, str2, len1) == 0) { \
        printf("Yay!\n"); \
    } else { \
        printf("Nope: '%s' - Expected: '%s'. Line: %d\n", strndup(str1, len1), strndup(str2, len2), line); \
    }

#define ASSERT_INT_EQ(v1, v2, line) \
    if (v1 == v2) { \
        printf("Yay!\n"); \
    } else { \
        printf("Nope. Expected %d to equal %d. Line: %d\n", v1, v2, line); \
    }

void test_find_many(char *json, char *to_find[], int number_of_keys, char *expected[], int number_of_expected, int line) {
    int *sizes = (int*)malloc(sizeof(int) * number_of_keys);
    for (int i = 0; i < number_of_keys; ++i) {
        sizes[i] = strlen(to_find[i]);
    }

    liji_state state = liji_init(json, strlen(json), to_find, sizes, number_of_keys);

    int i = 0;
    while (liji_find_multi_state(&state)) {
        ASSERT_STR_EQ(state.response.result_start, state.response.len, expected[i], strlen(expected[i]), line);
        ++i;
    }
    ASSERT_INT_EQ(i, number_of_expected, line);
}

void test_simple_find(char *json, char *to_find, char *expected, int line) {
    test_find_many(json, &to_find, 1, &expected, 1, line);
}

void test_find_multi(char *json, char *to_find[], int number_of_keys, char *expected, int line) {
    test_find_many(json, to_find, number_of_keys, &expected, 1, line);
}

char *read_json(char *path) {
    char *buffer = 0;
    FILE *f = fopen(path, "r");
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    buffer = malloc(sz);
    fread(buffer, sz, 1, f);
    fclose(f);
    return buffer;
}

int main(int argc, char *argv[]) {
    char *json[] =  {
        read_json("fixtures/edge.json"),
        read_json("fixtures/repeated_keys.json"),
        read_json("fixtures/arrays.json"),
        read_json("fixtures/bigone.json"),
    };

    test_simple_find(json[0], "yet_another", "999", __LINE__);
    test_simple_find(json[0], "test", "\"value\"", __LINE__);
    test_simple_find(json[0], "s]omething", "\"aaa\"", __LINE__);
    test_simple_find(json[0], "other{", "{\"s]omething\": \"aaa\", \"bbbb\": 1}", __LINE__);
    test_simple_find(json[0], "deep", "{\"really\":{\"deep\":7}}", __LINE__);
    test_simple_find(json[0], "really", "{\"deep\":7}", __LINE__);
    test_simple_find(json[0], "with\\\"quoted", "66", __LINE__);
    test_simple_find(json[0], "after_quoted", "\"ok\"", __LINE__);
    test_simple_find(json[0], "value_quoted", "\"quo\\\"ted\"", __LINE__);
    test_find_multi(json[0], (char *[]){ "other{", "s]omething" }, 2, "\"aaa\"", __LINE__);
    test_find_multi(json[0], (char *[]){ "other{", "bbbb" }, 2, "1", __LINE__);
    test_find_multi(json[0], (char *[]){ "deep", "really", "deep" }, 3, "7", __LINE__);

    test_find_many(json[1], (char *[]){ "other{", "yet_another" }, 2, NULL, 0, __LINE__);
    test_find_many(json[1], (char *[]){ "a" }, 1, (char *[]){ "1", "2" }, 2, __LINE__);
    test_find_many(json[1], (char *[]){ "d", "x" }, 2, (char *[]){ "9", "8" }, 2, __LINE__);
    test_find_many(json[1], (char *[]){ "d", "e", "x" }, 3, (char *[]){ "9", "8" }, 2, __LINE__);

    // Maybe not very intuitive,fix?
    test_find_many(json[1], (char *[]){ "c" }, 1, (char *[]){ "{ \"c\": 3 }" }, 1, __LINE__);

    test_find_many(json[2], (char *[]){ "title" }, 1, (char *[]){ "1", "2", "3" }, 3, __LINE__);
    test_find_many(json[2], (char *[]){ "key", "title" }, 2, (char *[]){ "1", "2", "3" }, 3, __LINE__);

    return 0;
}
