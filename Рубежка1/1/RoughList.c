#include <stdlib.h>
#include <pthread.h>
#include "roughlist.h"

void lock(pthread_mutex_t* mutex){
    pthread_mutex_lock(mutex);
}
void unlock(pthread_mutex_t* mutex){
    pthread_mutex_unlock(mutex);
}


FindResult find(RoughList* list, int key)
{
    lock(&list->mutex);
    FindResult result={0,0};
    if (list->root != NULL){
        for (RoughNode* node=list->root; node != NULL; node = node->next){
            if (node->key == key)
            {
                result.exists = 1;
                result.value = node->value;
                unlock(&list->mutex);
                return result;
            }
        }
    }
    unlock(&list->mutex);
    return result;
}

char insert(RoughList* list, int key, int value)
{
    if (!find(list, key).exists){
        lock(&list->mutex);
        RoughNode *tmp = (RoughNode*) malloc(sizeof(RoughNode));
        tmp->key = key;
        tmp->value = value;
        tmp->next = list->root;
        list->root = tmp;
        unlock(&list->mutex);
        return 1;
    } else{
        unlock(&list->mutex);
        return 0;
    }
}


char remove(RoughList* list, int key)
{
    //lock(&list->mutex);
    if(!find(list,key).exists) {
        //unlock(&list->mutex);
        return 1;
    }
    lock(&list->mutex);
    if(list->root->key==key){
        list->root=list->root->next;
        unlock(&list->mutex);
        return 1;
    }
    RoughNode* node = list->root;
    while (node){
        if(node->next->key==key)
        {
            RoughNode* nextnode = node->next;
            node->next = nextnode->next;
            free(nextnode);
            unlock(&list->mutex);
            return 1;
        }
        node=node->next;
    }
    unlock(&list->mutex);
    return 0;
}

