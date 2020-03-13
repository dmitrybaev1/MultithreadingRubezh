#include <stdlib.h>
#include "thinlist.h"
#include <pthread.h>
void lock(pthread_mutex_t *mutex){
    pthread_mutex_lock(mutex);
}
void unlock(pthread_mutex_t *mutex){
    pthread_mutex_unlock(mutex);
}

FindResult find(ThinList* list, int key)
{
    FindResult result={0,0};
    if (list->root != NULL){
        ThinNode* node = list->root;
        while (node != NULL){
            pthread_mutex_init(&node->mutex, NULL);
            lock(&node->mutex);
            if (node->key == key)
            {
                result.exists = 1;
                result.value = node->value;
                return result;
            }
            node = node->next;
            unlock(&node->mutex);
        }
    }
    return result;
}



char insert(ThinList* list, int key, int value)
{
    if (!find(list, key).exists){
        ThinNode *tmp = (ThinNode*) malloc(sizeof(ThinNode));
        tmp->key = key;
        tmp->value = value;
        tmp->next = NULL;
        pthread_mutex_init(&tmp->mutex, NULL);
        if (list->root == NULL){
            list->root = tmp;
        } else {
            lock(&list->root->mutex);
            tmp->next = list->root;
            list->root = tmp;
            unlock(&list->root->next->mutex);
        }
        return 1;
    } else{
        return 0;
    }
}

char remove(ThinList* list, int key)
{
    if(!find(list,key).exists) {
        return 1;
    }

    if(list->root->key==key){
        lock(&list->root->mutex);
        ThinNode* toRemove = list->root;
        list->root=list->root->next;
        unlock(&toRemove->mutex);
        return 1;
    }

    ThinNode* localNode = list->root;

    while (localNode){
        if(localNode->next->key==key)
        {
            lock(&localNode->mutex);
            lock(&localNode->next->mutex);
            ThinNode* toRemove = localNode->next;
            localNode->next = localNode->next->next;
            free(toRemove);
            unlock(&localNode->mutex);
            unlock(&toRemove->mutex);
            return 1;
        }

        localNode=localNode->next;
    }

    return 0;
}
char areSame(ThinNode *node1, ThinNode *node2) {
    if (node1->key == node2->key && node1->value == node2->value)
        return 1;
    else
        return 0;
}

ThinList *initialize_list(void) {
    ThinList *list;

    if ((list = (ThinList *) calloc(1, sizeof(ThinList))) == NULL) {
        return NULL;
    }

    if ((list->root = (ThinNode *) calloc(1, sizeof(ThinNode))) == NULL) {
        free(list->root);
        free(list);
        return NULL;
    }

    list->root= NULL;

    return list;
}

ThinList *copy_list(ThinList *list, ThinList *list_copy) {
    ThinNode *curr = list->root;

    list_copy = initialize_list();

    while (curr != NULL) {
        if (list_copy->root == NULL){
            ThinNode *tmp = (ThinNode*) malloc(sizeof(ThinNode));
            tmp->key = curr->key;
            tmp->value = curr->value;
            tmp->next = NULL;
            pthread_mutex_init(&tmp->mutex, NULL);

            list_copy->root = tmp;
        } else {
            ThinNode* localNode = list_copy->root;

            while (localNode->next != NULL) {
                localNode = localNode->next;
            }

            ThinNode *tmp = (ThinNode*) malloc(sizeof(ThinNode));
            tmp->key = curr->key;
            tmp->value = curr->value;
            tmp->next = NULL;
            pthread_mutex_init(&tmp->mutex, NULL);

            localNode->next = tmp;
        }

        curr = curr->next;
    }

    return list_copy;
}

ThinList *get_snapshot(ThinList *list) {
    ThinList *list_copy = initialize_list();
    ThinNode *curr, *curr_temp_copy;
    list_copy = copy_list(list, list_copy);
    curr = list->root;
    curr_temp_copy = list_copy->root;
    while (curr != NULL) {
        if (areSame(curr, curr_temp_copy) == 0) {
            list_copy = copy_list(list, list_copy);
            curr = list->root;
            curr_temp_copy = list_copy->root;
        } else {
            curr = curr->next;
            curr_temp_copy = curr_temp_copy->next;
        }
    }
    return list_copy;
}



