#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Optional: use these functions to add debug or error prints to your application
//#define DEBUG_LOG(msg,...)
#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)
#define delay_ms(time_ms)	{ usleep(time_ms * 1000); }

void* threadfunc(void* thread_param)
{

    // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
    // hint: use a cast like the one below to obtain thread arguments from your parameter
    //struct thread_data* thread_func_args = (struct thread_data *) thread_param;
	struct thread_data *thread_arg = (struct thread_data*)thread_param;
	//DEBUG_LOG("Entered thread");	
	delay_ms(thread_arg->wait_to_obtain);
	//DEBUG_LOG("Delayed to obtain");
	pthread_mutex_lock(thread_arg->mutex);
	//DEBUG_LOG("Acquired mutex");
	delay_ms(thread_arg->wait_to_release);
	pthread_mutex_unlock(thread_arg->mutex);
	thread_arg->thread_complete_success = true;
	return thread_param;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{
    /**
     * TODO: allocate memory for thread_data, setup mutex and wait arguments, pass thread_data to created thread
     * using threadfunc() as entry point.
     *
     * return true if successful.
     *
     * See implementation details in threading.h file comment block
     */
	struct thread_data *S_thread = (struct thread_data*)malloc(sizeof(struct thread_data));
	S_thread->thread_handle = thread;
	S_thread->wait_to_obtain = wait_to_obtain_ms;
	S_thread->wait_to_release = wait_to_release_ms;
	S_thread->mutex = mutex;
	int ret;
    ret = pthread_create(S_thread->thread_handle, NULL, threadfunc, (void*)S_thread);
	if(0 != ret){
		ERROR_LOG("Thread was not created!");
		return false;
	}
	return true;
}

