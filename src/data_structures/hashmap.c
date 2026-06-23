#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "data_structures/hashmap.h"
#include "core.h"

#define HASHMAP_HASH_VALUE 5381
#define HASHMAP_HASH_VALUE_SHIFT 5

int hash(const char* key) {
    unsigned long hash_value = HASHMAP_HASH_VALUE;
    int hash = 0;
    while ((hash = *(unsigned char*)key++)) {
        hash_value = ((hash_value << HASHMAP_HASH_VALUE_SHIFT) + hash_value) + hash;
}
    return (int)(hash_value % HASHMAP_ARRAY_SIZE);
}

bool insert(HashMap* hashmap, const char* key, void* value) {
    int index = hash(key);
    HashNode* node = hashmap->array[index];

    if (node == NULL) {
        hashmap->array[index] = create_hashnode(key, value);
        return false;
    }

    HashNode* prev = NULL;
    while (node != NULL) {
        if (strcmp(node->key, key) == 0) {
            node->value = value;
            return false;
        }
        prev = node;
        node = node->next;
    }

    prev->next = create_hashnode(key, value);
    return false;
}

void* get(HashMap* hashmap, const char* key) {
    if (hashmap == NULL) {
        jakarta_error(ERR_CUSTOM, NULL, "HashMap is not initialized");
    }
    int index = hash(key);
    HashNode* node = hashmap->array[index];
    while (node != NULL) {
        if (strcmp(node->key, key) == 0) {
            return node->value;
        }
        node = node->next;
    }
    return NULL;
}

HashMap* create_hashmap() {
    HashMap* hashmap = (HashMap*)calloc(1, sizeof(HashMap));
    if (!hashmap) {
        jakarta_error(ERR_MALLOC_FAIL, NULL, "HashMap");
    }
    return hashmap;
}

HashNode* create_hashnode(const char* key, void* value) {
    HashNode* hashnode = (HashNode*)malloc(sizeof(HashNode));
    if (!hashnode) {
        jakarta_error(ERR_MALLOC_FAIL, NULL, "HashNode");
    }
    hashnode->key = strdup(key);
    hashnode->value = value;
    hashnode->next = NULL;
    return hashnode;
}
