#ifndef HASH_H
#define HASH_H

#define HASH_MAX_LENGTH 100
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

uint8_t hash_search(char *start, char *end, uint32_t *result);
void hash_add(char *str, uint32_t value);
void upper(char *str);

#endif //HASH_H
