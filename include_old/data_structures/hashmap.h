#pragma once

#define HASHMAP_ARRAY_SIZE 100

typedef struct HashNodeDef {
    char* key;
    void* value;
    struct HashNodeDef* next;
} HashNode;

typedef struct HashMapDef {
    HashNode* array[HASHMAP_ARRAY_SIZE];
} HashMap;

int hash(const char* key);
bool insert(HashMap* hashmap, const char* key, void* value);
void* get(HashMap* hashmap, const char* key);
HashMap* create_hashmap();
HashNode* create_hashnode(const char* key, void* value);