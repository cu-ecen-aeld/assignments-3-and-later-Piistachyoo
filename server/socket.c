#include <fcntl.h>
#include <netdb.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>

#define LOG_PRIO(_LEVEL) (LOG_USER | _LEVEL)
#define BUFF_MAX_LEN 5000000

#define USE_AESD_CHAR_DEVICE 1

#ifndef USE_AESD_CHAR_DEVICE
#define PATH "/var/tmp/aesdsocketdata"
#else
#define PATH "/dev/aesdchar"
#endif

struct node {
    pthread_t thread;
    struct node *next;
} thread_list;
#ifndef USE_AESD_CHAR_DEVICE
pthread_t timer_thread_handle;
#endif
int mySocketFD, peerSocketFD;
FILE *file;
pthread_mutex_t file_mutex;
struct sockaddr peeradd;
struct node *HEAD = NULL;

void signal_handler(int signal) {
    printf("Caught signal SIGINT!\n");
    printf("Terminating...\n");
//	struct node *iterator = HEAD;
//	struct node *temp = iterator;
#ifndef USE_AESD_CHAR_DEVICE
    pthread_cancel(timer_thread_handle);
    pthread_join(timer_thread_handle, NULL);
#endif
    //	while(iterator){
    // pthread_cancel(iterator->thread);
    // iterator = iterator->next;
    /*
    free(iterator);
    iterator = temp->next;
    temp = temp->next;
    */
    //	}
    close(mySocketFD);
    close(peerSocketFD);
#ifndef USE_AESD_CHAR_DEVICE
    remove(PATH);
#endif
    shutdown(mySocketFD, SHUT_RDWR);
    exit(0);
}

int initiate_connection(void) {
    int yes = 1;
    struct addrinfo ad_info;
    struct addrinfo *servinfo;
    mySocketFD = socket(PF_INET, SOCK_STREAM, 0);
    memset(&ad_info, 0, sizeof(ad_info));
    ad_info.ai_flags = AI_PASSIVE;
    ad_info.ai_family = AF_INET;
    ad_info.ai_socktype = SOCK_STREAM;
    ad_info.ai_protocol = 0;
    if (getaddrinfo(NULL, "9000", &ad_info, &servinfo) != 0) {
        perror("getaddrinfo");
        return -1;
    } else { /* Do Nothing */
    }

    if (setsockopt(mySocketFD, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) ==
        -1) {
        perror("setsockopt");
        return -1;
    } else { /* Do Nothing */
    }

    if (bind(mySocketFD, servinfo->ai_addr, sizeof(struct addrinfo)) != 0) {
        perror("bind");
        freeaddrinfo(servinfo);
        return -1;
    } else { /* Do Nothing */
    }

    freeaddrinfo(servinfo);

    if (listen(mySocketFD, 1) != 0) {
        perror("listen");
        return -1;
    } else { /* Do Nothing */
    }

    return 0;
}

void receive_data(void) {
    char my_buffer[BUFF_MAX_LEN];

    int index = 0;
    int buffer_len = 0;

    while (1) {
        recv(peerSocketFD, &(my_buffer[index]), 1, MSG_WAITALL);
        buffer_len++;
        if (my_buffer[index] == '\n') {
            break;
        }
        index++;
    }
    //	pthread_mutex_lock(&file_mutex);
    file = fopen(PATH, "a");
    fwrite(my_buffer, sizeof(char), buffer_len, file);
    fclose(file);
    //	pthread_mutex_unlock(&file_mutex);
}

void send_data(void) {
    char *str_buffer = (char *)malloc(BUFF_MAX_LEN);
    //	pthread_mutex_lock(&file_mutex);
    file = fopen(PATH, "r");
    while (fgets(str_buffer, BUFF_MAX_LEN, file)) {
        send(peerSocketFD, str_buffer, strlen(str_buffer), MSG_WAITALL);
    }
    fclose(file);
    //	pthread_mutex_unlock(&file_mutex);
    printf("Closed connection from %u:%u:%u:%u\n", peeradd.sa_data[2],
           peeradd.sa_data[3], peeradd.sa_data[4], peeradd.sa_data[5]);
    syslog(LOG_PRIO(LOG_DEBUG), "Closed connection from %u:%u:%u:%u\n",
           peeradd.sa_data[2], peeradd.sa_data[3], peeradd.sa_data[4],
           peeradd.sa_data[5]);
    free(str_buffer);
}

void *start_thread(void *arg) {
    pthread_mutex_lock(&file_mutex);
    receive_data();
    send_data();
    pthread_mutex_unlock(&file_mutex);
    pthread_exit(arg);
}
#ifndef USE_AESD_CHAR_DEVICE
void *timer_thread(void *arg) {
    int old;
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &old);
    char timestr[200];
    time_t t;
    struct tm *tmp;
    t = time(NULL);
    while (1) {
        sleep(10);
        tmp = localtime(&t);
        if (tmp == NULL) {
            perror("localtime");
            exit(EXIT_FAILURE);
        }
        if (strftime(timestr, sizeof(timestr), "%a, %d %b %Y %T %z", tmp) ==
            0) {
            fprintf(stderr, "strftime returned 0");
            exit(EXIT_FAILURE);
        }
        pthread_mutex_lock(&file_mutex);
        file = fopen(PATH, "a");
        fwrite("timestamp:", sizeof(char), strlen("timestamp:"), file);
        fwrite(timestr, sizeof(char), strlen(timestr), file);
        fwrite("\n", sizeof(char), strlen("\n"), file);
        fclose(file);
        pthread_mutex_unlock(&file_mutex);
    }
}
#endif
int main(int argc, char *argv[]) {
    int ret;

    if (argc > 1) {
        if (strcmp(argv[1], "-d") == 0) {
            pid_t pid = fork();
            if (pid == -1) {
                perror("fork");
                return -1;
            } else if (pid != 0) {
                exit(0);
            } else { /* Do Nothing */
            }
            if (setsid() == -1) {
                perror("setsid");
                return -1;
            } else { /* Do Nothing */
            }
            freopen("/dev/null", "r", stdin);
            freopen("/dev/null", "w", stdout);
            freopen("/dev/null", "w", stderr);
        }
    }
    /* Set signal handler */
    signal(SIGINT, signal_handler);

    if (initiate_connection() != 0) {
        return -1;
    }

/* Truncate file */
#ifndef USE_AESD_CHAR_DEVICE
    file = fopen(PATH, "w");
    fclose(file);
#endif
#ifndef USE_AESD_CHAR_DEVICE
    pthread_create(&timer_thread_handle, NULL, timer_thread, (void *)NULL);
#endif
    while (1) {
        socklen_t len = sizeof(struct sockaddr);
        peerSocketFD = accept(mySocketFD, &peeradd, &len);
        if (peerSocketFD == -1) {
            perror("accept");
            continue;
        }
        printf("Accepted connection from %u:%u:%u:%u\n", peeradd.sa_data[2],
               peeradd.sa_data[3], peeradd.sa_data[4], peeradd.sa_data[5]);
        syslog(LOG_PRIO(LOG_DEBUG), "Accepted connection from %u:%u:%u:%u\n",
               peeradd.sa_data[2], peeradd.sa_data[3], peeradd.sa_data[4],
               peeradd.sa_data[5]);
        struct node *new = (struct node *)malloc(sizeof(struct node));
        ret = pthread_create(&(new->thread), NULL, start_thread, (void *)NULL);
        if (0 != ret) {
            syslog(LOG_PRIO(LOG_ERR), "Error creating thread!");
        }
        if (HEAD == NULL) {
            HEAD = new;
            new->next = NULL;
        } else {
            new->next = NULL;
            HEAD = new;
        }
        pthread_join(new->thread, NULL);
        free(new);
    }
    return 0;
}
