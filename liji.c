#include "liji.h"
#include <stdio.h>
#include <string.h>

void liji_find(const char *json_str, const char *wanted_key, int wanted_len, char **result_start, int *len) {
    char *current_pos = (char *)json_str;

    char val, *key_start = 0, *tmp_result = 0, *non_ws = 0;

    int level = 0,
        result_level = 0,
        key_len = 0,
        tmp_len = 0;

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
            } else {
                non_ws = current_pos - 1;
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
                if (level == result_level && tmp_result) {
                   tmp_len = non_ws - tmp_result;
                   *result_start = tmp_result;
                   *len = tmp_len;
                   return;
                }
            case ']':
                --level;
                break;
            case ',':
                key_start = 0;
                key_len = 0;
                if (level == result_level && tmp_result) {
                   tmp_len = non_ws - tmp_result;
                   *result_start = tmp_result;
                   *len = tmp_len;
                   return;
                }
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
                    // printf("key: ");
                    // fwrite(key_start, key_len, 1, stdout);
                    // printf("\n");
                    if (key_len == wanted_len && memcmp(key_start, wanted_key, key_len) == 0) {
                        in_key = 1;
                    }
                }
                break;
        }
        // printf("level: %d\n", level);
    }
}
