#ifndef __JAKARTA_HASHMAP_H__
#define __JAKARTA_HASHMAP_H__

#define HASHMAP_ARRAY_SIZE 100

typedef struct HashNodeDef {
    char* key;
    void* value;
    struct HashNodeDef* next;
} HashNode;

typedef struct HashMapDef {
    HashNode* array[HASHMAP_ARRAY_SIZE];
} HashMap;

int hash(char* key);
int insert(HashMap* hashmap, char* key, void* value);
void* get(HashMap* hashmap, char* key);
HashMap* create_hashmap();

#endif