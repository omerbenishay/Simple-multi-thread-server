#include "segel.h"
#include "request.h"
//#include <pthread.h>
#include "queue.h"
#include "stats.h"
// 
// server.c: A very, very simple web server
//
// To run:
//  ./server <portnum (above 2000)>
//
// Repeatedly handles HTTP requests sent to this port number.
// Most of the work is done within routines written in request.c
//

pthread_mutex_t lock_queue;
pthread_cond_t  c_busy;
pthread_cond_t  c_free;
Queue* requests_pending;
int requests_handled;

//pending + active <= queue_size
// HW3: Parse the new arguments too
void getargs(int *port, int argc, char *argv[], int *threads, int* queue_size, char * schedalg)
{
    if (argc < 2 || argc > 5) {
	fprintf(stderr, "Usage: %s <port>\n", argv[0]);
	exit(1);
    }
    *port = atoi(argv[1]);
    //argv[0] = ./server , argv[1] = portnum,  argv[2] = threads, argv[3] = queue size , argv[4] = schedalg
    *threads = atoi(argv[2]);
    *queue_size = atoi(argv[3]);
    strcpy(schedalg, argv[4]);
}

void overload_handler(char* schedalg, int queue_size)
{
	if (schedalg == NULL )
	{
		return;
	}
	if (strcmp(schedalg, "block") == 0)
	{
		while (queue_get_size(requests_pending) +requests_handled >= queue_size)
		{
			pthread_cond_wait(&c_busy, &lock_queue);
		}
	}
	else if (strcmp(schedalg, "dt") == 0)
	{
		queue_pop_back(requests_pending);
	}
	else if (strcmp(schedalg, "random") == 0)
	{
		queue_drop_random(requests_pending);
	}
	else if (strcmp(schedalg, "dh") == 0)
	{
		queue_pop(requests_pending);
	}
}
void* thread_request_handler(void* index)
{
	printf("hello i'm thread %d\n", *(int*)index);
	Stats* thread_stats = malloc(sizeof(*thread_stats));
	if (thread_stats == NULL)
	{ 

	}
	thread_stats->static_req = 0;
	thread_stats->dynamic_req = 0;
	thread_stats->total_req = 0;
	while (1)
	{
		pthread_mutex_lock(&lock_queue);
		while (queue_is_empty(requests_pending)) 
		{
			pthread_cond_wait(&c_free, &lock_queue);
		}
		int fd_to_handle;  
		queue_front(requests_pending, &fd_to_handle, &thread_stats->arrival_time);
		queue_pop(requests_pending);
		requests_handled++;
		
		pthread_mutex_unlock(&lock_queue);

		thread_stats->total_req++;
		struct timeval start_time;
		gettimeofday(&start_time, NULL);
		printf("time of day\n");
		timersub(&start_time, &(thread_stats->arrival_time), &(thread_stats->wait_time));
		printf("timersub\n");
		requestHandle(fd_to_handle, thread_stats);
		printf("after handling the request by thread number %d\n", *((int*)index));
		close(fd_to_handle);

		pthread_mutex_lock(&lock_queue);
		requests_handled--;
		pthread_cond_signal(&c_busy);
		pthread_mutex_unlock(&lock_queue);
	}
	return NULL;
}

int main(int argc, char *argv[])
{
    int listenfd, connfd, port, clientlen;
    struct sockaddr_in clientaddr;

	int thread_num, queue_size;
	char schedalg [7];
    getargs(&port, argc, argv, &thread_num, &queue_size, schedalg);

    // 
    // HW3: Create some threads...
    //
    if (pthread_mutex_init(&lock_queue, NULL) != 0)
    {
		//fail
	}
	pthread_cond_init(&c_busy, NULL);
	pthread_cond_init(&c_free, NULL);
	requests_pending = queue_create();
	requests_handled = 0;
	if ( requests_pending == NULL)
	{
		//fail
	}
	int* thread_idx = (int*)malloc(sizeof(int)*thread_num);
    pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t)*thread_num);
    if (threads == NULL || thread_idx == NULL)
    {
		//?
	}
	int i = 0;
    for (; i < thread_num; i++)
    {
		thread_idx[i] = i;
		pthread_create(&threads[i], NULL, thread_request_handler, (void*)(thread_idx + i));//check ret val?
	}

    listenfd = Open_listenfd(port);
    while (1) {
		clientlen = sizeof(clientaddr);
		/////////
		connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);
		//////////
		////// my addition:
		pthread_mutex_lock(&lock_queue);
		if (queue_get_size(requests_pending) + requests_handled >= queue_size)
		{
			overload_handler(schedalg, queue_size);
		}
		else
		{
			if (!queue_push_back(requests_pending, connfd))
			{
				//something went wrong
			}
			if (queue_get_size(requests_pending))
			{
				pthread_cond_signal(&c_free);
			}
			pthread_mutex_unlock(&lock_queue);
		}
	}
	//////////
	// 
	// HW3: In general, don't handle the request in the main thread.
	// Save the relevant info in a buffer and have one of the worker threads 
	// do the work. 
	// 
	//requestHandle(connfd);

	//Close(connfd);
}




    


 
