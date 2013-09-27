#include "liji.h"
#include <stdio.h>
#include <string.h>

#define FINAL_CHECKER \
    if (level == result_level && tmp_result) { \
        if (!non_ws) { \
            non_ws = current_pos - 1; \
        } \
        tmp_len = non_ws - tmp_result; \
        *result_start = tmp_result; \
        *len = tmp_len; \
        return; \
    } \
    non_ws = NULL; \

void liji_find_multi(
    const char *json_str,
    const char *wanted_keys[],
    int wanted_lens[],
    int number_of_keys,
    char **result_start,
    int *len)
{
    char *current_pos = (char *)json_str;

    char val, *key_start = 0, *tmp_result = 0, *non_ws = 0;

    int level = 0,
        result_level = 0,
        key_len = 0,
        tmp_len = 0,
        current_key = 0;

    char in_dict = 0,
         in_string = 0,
         in_key = 0,
         skipping = 1;

    while ((val = *(current_pos++)) != 0) {
        if (val != '"' && in_string) {
            continue;
        }
        if (tmp_result) {
            if (val == ' ') {
                if (skipping) {
                    tmp_result++;
                    continue;
                }
                if (!non_ws) {
                    non_ws = current_pos - 1;
                }
            } else {
                skipping = 0;
            }
        }
        switch (val) {
            case '{':
                key_start = 0;
                key_len = 0;
                in_dict = 1;
            case '[':
                ++level;
                break;

            case '}':
                in_dict = 0;
            case ']':
                FINAL_CHECKER;
                --level;
                if (current_key > 0) {
                    --current_key;
                }
                break;
            case ',':
                key_start = 0;
                key_len = 0;
                FINAL_CHECKER;
                break;
            case ':':
                if (in_key && !tmp_result) {
                    tmp_result = current_pos;
                    result_level = level;
                }
                break;
            case '"':
                in_string = !in_string;
                if (!key_start) {
                    key_start = current_pos;
                } else if (!key_len) {
                    key_len = current_pos - 1 - key_start;
                    if (key_len == wanted_lens[current_key] && memcmp(key_start, wanted_keys[current_key], key_len) == 0) {
                        ++current_key;
                        if (current_key == number_of_keys) {
                            in_key = 1;
                        }
                    }
                }
                break;
        }
    }
}

void liji_find(const char *json_str, const char *wanted_key, int wanted_len, char **result_start, int *len) {
    liji_find_multi(json_str, (const char**) &wanted_key, (int*) &wanted_len, 1, result_start, len);
}
