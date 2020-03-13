#include <stdlib.h>
#include <assert.h>
#include "roughlist.h"
#include <pthread.h>
int main() {
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex,NULL);
    RoughList list = {mutex,NULL};
    insert(&list,1,2);
    {
        FindResult result = find(&list,1);
        assert(result.exists);
        assert(result.value == 2);
    }

    {
        FindResult result = find(&list, 0);
        assert(!result.exists);
    }

    insert(&list, 0,1);

    {
        FindResult result = find(&list,0);
        assert(result.exists);
        assert(result.value == 1);
    }

    insert(&list, 2,3);
    {
        FindResult result = find(&list,2);
        assert(result.exists);
        assert(result.value == 3);
    }

    assert(remove(&list,1));

    {
        FindResult result = find(&list, 1);
        assert(!result.exists);
    }

    assert(remove(&list,0));
    assert(remove(&list,2));

    {
        FindResult result = find(&list, 0);
        assert(!result.exists);
    }

    {
        FindResult result = find(&list, 1);
        assert(!result.exists);
    }

    {
        FindResult result = find(&list, 2);
        assert(!result.exists);
    }
    return 0;
}

