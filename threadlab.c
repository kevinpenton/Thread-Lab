/*================================================================================
 * Project Info Header
 *================================================================================
 *================================================================================
 *  Author: Kevin Penton 
 *  Panther ID: 6173069
 *
 *  Certification: I understand FIU's academic policies, and I certify
 *                 that this work is my own and that none of it is the work of any 
 *                 other person.
 *
 *  Program Description: The purpose of this program is to use 8 threads where each 
 *                       thread loops 10 times to increment a shared variable by
 *                       its Thread ID (tid) in every iteration of the loop.  
 *================================================================================
*/

/* Same as multthread, but with mutexes */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 8

/* create thread argument struct for thr_func() */
typedef struct _thread_data_t {
  int tid;
  double stuff;
} thread_data_t;

/* shared data between threads */
double shared_x;
pthread_mutex_t lock_x;

void *thr_func(void *arg) {
  thread_data_t *data = (thread_data_t *)arg;

  printf("Thread %d has finished.\n", data->tid);
  
  /* get mutex before modifying and printing shared_x */
  pthread_mutex_lock(&lock_x);

  /*increments shared variable by its Thread ID 10 times*/
  for (int i = 0; i < 10; ++i) {
    shared_x += data->tid;
  } 

  pthread_mutex_unlock(&lock_x);
  pthread_exit(NULL);
  
}

int main(int argc, char **argv) {
  pthread_t thr[NUM_THREADS];
  int i, rc;
  
/* create a thread_data_t argument array */
  thread_data_t thr_data[NUM_THREADS];

  /* initialize shared data */
  shared_x = 0;

  /* initialize pthread mutex protecting "shared_x" */
  pthread_mutex_init(&lock_x, NULL);

  /* create threads */
  for (i = 0; i < NUM_THREADS; ++i) {
    thr_data[i].tid = i;
    thr_data[i].stuff = (i + 1) * NUM_THREADS;
    if ((rc = pthread_create(&thr[i], NULL, thr_func, &thr_data[i]))) {
      fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
      return EXIT_FAILURE;
    }
  }
  /* block until all threads complete */
  for (i = 0; i < NUM_THREADS; ++i) {
    pthread_join(thr[i], NULL);
  }
  printf("x = %f\n", shared_x);
  return EXIT_SUCCESS;
}

