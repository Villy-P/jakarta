#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "data_structures/hashmap.h"
#include "core.h"

#define HASH_BASE_PRIME 31
#define HASH_LARGE_PRIME 1000000009
#define HASH_INITIAL_VALUE 0
#define HASH_INITIAL_POWER 1

int hash(char* key) {
    unsigned long hash_value = 5381;
    int c;
    while ((c = *key++))
        hash_value = ((hash_value << 5) + hash_value) + c;
    return (int)(hash_value % HASHMAP_ARRAY_SIZE);
}

int insert(HashMap* hashmap, char* key, void* value) {
    int index = hash(key);
    HashNode* node = hashmap->array[index];

    if (node == NULL) {
        hashmap->array[index] = create_hashnode(key, value);
        return 0;
    }

    HashNode* prev = NULL;
    while (node != NULL) {
        if (strcmp(node->key, key) == 0) {
            node->value = value;
            return 0;
        }
        prev = node;
        node = node->next;
    }

    prev->next = create_hashnode(key, value);
    return 0;
}

void* get(HashMap* hashmap, char* key) {
    if (hashmap == NULL)
        jakarta_error(ERR_CUSTOM, NULL, "HashMap is not initialized");
    int index = hash(key);
    HashNode* node = hashmap->array[index];
    while (node != NULL) {
        if (strcmp(node->key, key) == 0)
            return node->value;
        node = node->next;
    }
    return NULL;
}

HashMap* create_hashmap() {
    HashMap* hashmap = (HashMap*)calloc(1, sizeof(HashMap));
    if (!hashmap)
        jakarta_error(ERR_MALLOC_FAIL, NULL, "HashMap");
    return hashmap;
}

HashNode* create_hashnode(char* key, void* value) {
    HashNode* hashnode = (HashNode*)malloc(sizeof(HashNode));
    if (!hashnode)
        jakarta_error(ERR_MALLOC_FAIL, NULL, "HashNode");
    hashnode->key = strdup(key);
    hashnode->value = value;
    hashnode->next = NULL;
    return hashnode;
}
