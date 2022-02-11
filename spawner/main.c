/**
 * @file main.c
 * @author Philip Meulengracht (philip.meulengracht@canonical.com)
 * @brief Spawner, small tool to spawn threads up to a certain point. If a thread fails to spawn
 * the program will retry again after a configurable delay.
 * @version 0.1
 * @date 2022-02-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

struct worker_context {
    pthread_t  id;
    atomic_int enabled;
};

void* worker_main(void* arg)
{
    struct worker_context* context = (struct worker_context*)arg;
    atomic_store(&context->enabled, 1);

    printf("worker_main: %li started\n", context->id);

    while(atomic_load(&context->enabled)) {
        sleep(1);
    }

    printf("worker_main: %li stopped\n", context->id);
}

int main(int argc, char** argv)
{
    struct worker_context* contexts;
    int                    i;
    int                    num_threads = 32;
    int                    delay = 2;

    // retrieve recreation delay and number of workers from args
    if (argc > 1) {
        for (i = 1; i < argc; i++) {
            if (!strcmp(argv[i], "-n") || !strcmp(argv[i], "--num-threads")) {
                num_threads = atoi(argv[i + 1]);
            }
            else if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--delay")) {
                delay = atoi(argv[i + 1]);
            }
        }
    }

    if (num_threads < 1) {
        fprintf(stderr, "spawner: invalid number of threads: %i\n", num_threads);
        return 1;
    }

    if (delay < 1) {
        fprintf(stderr, "spawner: invalid delay: %i\n", delay);
        return 1;
    }

    contexts = calloc(num_threads, sizeof(struct worker_context));
    if (!contexts) {
        fprintf(stderr, "spawner: failed to allocate memory for worker contexts\n");
        return 1;
    }

    printf("spawner: spawning %i workers\n", num_threads);
    while (1) {
        for(i = 0; i < num_threads; i++) {
            int status;

            // is thread running
            if (atomic_load(&contexts[i].enabled)) {
                continue;
            }
            
            status = pthread_create(&contexts[i].id, NULL, worker_main, &contexts[i]);
            if (status) {
                printf("spawner: reached worker thread limit %i\n", i);
                break;
            }
        }

        // sleep for delay
        sleep(delay);
    }

    return 0;
}
