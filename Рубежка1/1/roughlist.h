#include <pthread.h>
typedef struct RoughNode
{
    int key;
    int value;
    struct RoughNode* next;
}RoughNode;

typedef struct RoughList
{
    pthread_mutex_t mutex;
    RoughNode* root;
}RoughList;

typedef struct FindResult
{
    int value;
    char exists;
}FindResult;
FindResult find(RoughList* list, int key);
char insert(RoughList* list, int key, int value);
char remove(RoughList* list, int key);
