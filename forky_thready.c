#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void* pattern_one_thread(void* arg) {
    int p1 = *((int*)arg);
    printf("Thread %d TID: %lu created by main thread\n", p1, pthread_self());
    return NULL;
}

void pattern_one(int num_of_threads) {
    printf("Pattern 1\n");
    pthread_t threads[num_of_threads];
    int ids[num_of_threads];

    for (int i = 0; i < num_of_threads; i++) {
        ids[i] = i + 1;
        if (pthread_create(&threads[i], NULL, pattern_one_thread, &ids[i]) != 0) {
            perror("pthread_create failed\n");
            exit(1);
        }
    }
    
    for (int i = 0; i < num_of_threads; i++) {
        pthread_join(threads[i], NULL);
    }
}

void* pattern_two_thread(void* arg) {
    int p2 = *((int*)arg);
    printf("Thread %d TID: %lu created by main thread\n", p2, pthread_self());
    sleep(rand() % 8 + 1);
    if (p2 == 1) { // Simulating behavior of the last process in original code
        printf("Thread %d TID: %lu exiting\n", p2, pthread_self());
    }
    return NULL;
}

void pattern_two(int num_of_threads) {
    printf("Pattern 2\n");
    pthread_t threads[num_of_threads];
    int ids[num_of_threads];

    for (int i = 0; i < num_of_threads; i++) {
        ids[i] = i + 1;
        if (pthread_create(&threads[i], NULL, pattern_two_thread, &ids[i]) != 0) {
            perror("pthread_create failed\n");
            exit(1);
        }
    }

    for (int i = 0; i < num_of_threads; i++) {
        pthread_join(threads[i], NULL);
    }
}

void* pattern_three_thread(void* arg);

void pattern_three_recursive(int current_thread, int num_of_threads) {
    if (current_thread > num_of_threads) {
        return;
    }

    pthread_t left, right;
    int left_id = current_thread;
    int right_id = current_thread + 1;

    if (pthread_create(&left, NULL, pattern_three_thread, &left_id) != 0) {
        perror("pthread_create failed");
        exit(1);
    }
    pthread_join(left, NULL);

    if (current_thread * 2 + 1 <= num_of_threads) {
        if (pthread_create(&right, NULL, pattern_three_thread, &right_id) != 0) {
            perror("pthread_create failed");
            exit(1);
        }
        pthread_join(right, NULL);
    }
}

void* pattern_three_thread(void* arg) {
    int current_thread = *((int*)arg);
    printf("Thread %d (TID: %lu) created\n", current_thread, pthread_self());
    sleep(rand() % 8 + 1);
    pattern_three_recursive(current_thread * 2, 256); // Adjust for recursion
    printf("Thread %d (TID: %lu) exiting\n", current_thread, pthread_self());
    return NULL;
}

void pattern_three(int num_of_threads) {
    printf("Pattern 3\n");
    pattern_three_recursive(1, num_of_threads);
}

int main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("Insufficient arguments. Usage: %s <num_threads> <pattern_num>\n", argv[0]);
        return 1;
    }

    int num_of_threads = atoi(argv[1]);
    int pattern_num = atoi(argv[2]);

    if (num_of_threads < 1 || num_of_threads > 256) {
        printf("Number of threads value must be between 1 - 256.\n");
        return 1;
    }

    switch (pattern_num) {
        case 1:
            pattern_one(num_of_threads);
            break;
        case 2:
            pattern_two(num_of_threads);
            break;
        // case 3:
        //    pattern_three(num_of_threads);
        //    break;
        default:
            printf("You didn't choose a valid pattern. Please choose 1, 2, or 3 for Y.\n./thready 'X' 'Y'\n");
            return 1;
    }

    return 0;
}
