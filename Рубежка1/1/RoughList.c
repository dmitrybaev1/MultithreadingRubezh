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

RoughList *initialize_list() {
    RoughList *list;
    if ((list = (RoughList *) calloc(1, sizeof(RoughList))) == NULL) {
        return NULL;
    }
    if ((list->root = (RoughNode *) calloc(1, sizeof(RoughNode))) == NULL) {
        free(list);
        return NULL;
    }
    list->root = NULL;
    pthread_mutex_init(&list->mutex, NULL);
    return list;
}

RoughList *copyList(RoughList *list, RoughList *list_copy) {
    RoughNode *curr = list->root;
    list_copy = initialize_list();
    while (curr != NULL) {
        if (list_copy->root == NULL){
            RoughNode *tmp = (RoughNode*) malloc(sizeof(RoughNode));
            tmp->key = curr->key;
            tmp->value = curr->value;
            tmp->next = NULL;
            list_copy->root = tmp;
        } else {
            RoughNode* localNode = list_copy->root;
            while (localNode->next != NULL) {
                localNode = localNode->next;
            }
            RoughNode *tmp = (RoughNode*) malloc(sizeof(RoughNode));
            tmp->key = curr->key;
            tmp->value = curr->value;
            tmp->next = NULL;
            localNode->next = tmp;
        }
        curr = curr->next;
    }
    return list_copy;
}

RoughList *get_snapshot(RoughList *list) {
    pthread_mutex_lock( &(list->mutex) );
    RoughList *roughList = initialize_list();
    roughList = copyList(list, roughList);
    pthread_mutex_unlock( &(list->mutex) );
    return roughList;
}

