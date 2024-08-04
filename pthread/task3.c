#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_STORES 5
#define NUM_CUSTOMERS 3
#define STORE_CAPACITY 10000
#define LOADER_INCREMENT 5000

typedef struct {
    int id;
    int need;
} customer_t;

pthread_mutex_t store_mutexes[NUM_STORES];
int stores[NUM_STORES];
int satisfied_customers = 0;
pthread_mutex_t satisfied_customers_mutex = PTHREAD_MUTEX_INITIALIZER;

void *customer_thread(void *arg) {
    customer_t *customer = (customer_t *)arg;
    while (customer->need > 0) {
        for (int i = 0; i < NUM_STORES; i++) {
            if (pthread_mutex_trylock(&store_mutexes[i]) == 0) {
                printf("Customer %d entered store %d, store value: %d, need: %d\n", customer->id, i, stores[i], customer->need);
                customer->need -= stores[i];
                stores[i] = 0;
                printf("Customer %d sleeping...\n", customer->id);
                pthread_mutex_unlock(&store_mutexes[i]);
                sleep(2);
                break;
            }
        }
    }
    printf("Customer %d satisfied and exiting\n", customer->id);
    pthread_mutex_lock(&satisfied_customers_mutex);
    satisfied_customers++;
    pthread_mutex_unlock(&satisfied_customers_mutex);
    return NULL;
}

void *loader_thread() {
    while (1) {
        for (int i = 0; i < NUM_STORES; i++) {
            if (pthread_mutex_trylock(&store_mutexes[i]) == 0) {
                stores[i] += LOADER_INCREMENT;
                printf("Loader entered store %d, added %d, new value: %d\n", i, LOADER_INCREMENT, stores[i]);
                pthread_mutex_unlock(&store_mutexes[i]);
                sleep(1);
                break;
            }
        }
        pthread_mutex_lock(&satisfied_customers_mutex);
        if (satisfied_customers >= NUM_CUSTOMERS) {
            pthread_mutex_unlock(&satisfied_customers_mutex);
            break;
        }
        pthread_mutex_unlock(&satisfied_customers_mutex);
    }
    return NULL;
}

int main() {
    pthread_t customers[NUM_CUSTOMERS];
    pthread_t loader;

    srand(time(NULL));
    for (int i = 0; i < NUM_STORES; i++) {
        stores[i] = rand() % STORE_CAPACITY;
        pthread_mutex_init(&store_mutexes[i], NULL);
    }

    customer_t customer_data[NUM_CUSTOMERS];
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        customer_data[i].id = i;
        customer_data[i].need = rand() % 100000;
        pthread_create(&customers[i], NULL, customer_thread, &customer_data[i]);
    }

    pthread_create(&loader, NULL, loader_thread, NULL);

    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        pthread_join(customers[i], NULL);
    }

    pthread_join(loader, NULL);

    for (int i = 0; i < NUM_STORES; i++) {
        pthread_mutex_destroy(&store_mutexes[i]);
    }

    return 0;
}
