#include "hash.h"
#include <stdio.h>

uint32_t hash_func(char *start, char *end);

typedef struct {
	uint32_t key;
	uint32_t value;
} hash_item;

static hash_item hash_map[HASH_MAX_LENGTH];
static size_t hash_map_length = 0;

uint32_t hash_func(char *start, char *end) { //Why? Because it looked fun, no good reason. Makes it slightly faster with less modularity
	int hash = 5381;
	while (start <= end) {
		hash = ((hash << 5) + hash) + *start;
		start++;
	}
	return hash;
}

uint8_t hash_search(char *start, char *end, uint32_t *result) {
	uint32_t key = hash_func(start, end);
	for (size_t i = 0; i < hash_map_length; i++) {
		if (hash_map[i].key == key) {
			*result = hash_map[i].value;
			return 1;
		}
	}
	return 0;
}

void hash_add(char *str, uint32_t value) {
	hash_map[hash_map_length].key = hash_func(str, str + strlen(str) - 1);
	hash_map[hash_map_length].value = value;
	hash_map_length++;
}
void upper(char *str) {
	for (int i = 0; i < strlen(str); i++) {
		if (str[i] <= 'z' && str[i] >= 'a') {
			str[i] -= 32;
		}
	}
}
