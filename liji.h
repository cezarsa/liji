#ifndef __LIJI__INCLUDE__H__
#define __LIJI__INCLUDE__H__

void liji_find(const char *json_str, const char *wanted_key, int wanted_len, char **result_start, int *len);
void liji_find_multi(
    const char *json_str,
    const char *wanted_keys[],
    int wanted_lens[],
    int number_of_keys,
    char **result_start,
    int *len);

#endif