#include "data_structures/hashmap.h"

#include <stddef.h>

int hash(char* key) {
    const int p = 31;
    const int m = 1e9 + 9;
    int hash_value = 0;
    int p_pow = 1;
    for (int i = 0; key[i]; ++i) {
        hash_value = (hash_value + (key[i] - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return hash_value % HASHMAP_ARRAY_SIZE;
}

int insert(HashMap* hashmap, char* key, void* value) {
    int index = hash(key);
    HashNode* node = hashmap->array[index];
    while (node != NULL) {
        if (strcmp(node->key, key) == 0) {
            node->value = value;
            return 0;
        }
        node = node->next;
    }
    HashNode* new_node = (HashNode*)malloc(sizeof(HashNode));
    new_node->key = key;
    new_node->value = value;
    new_node->next = hashmap->array[index];
    hashmap->array[index] = new_node;
    return 0;
}

void* get(HashMap* hashmap, char* key) {
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
    HashMap* hashmap = (HashMap*)malloc(sizeof(HashMap));
    for (int i = 0; i < HASHMAP_ARRAY_SIZE; ++i)
        hashmap->array[i] = NULL;
    return hashmap;
}
