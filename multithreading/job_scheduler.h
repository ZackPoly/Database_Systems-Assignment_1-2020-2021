#include "../logistic_regression/model.h"
#include <pthread.h>

// L031203576008256613
#define BATCH_SIZE 1024
#define perror2(s,e) fprintf(stderr, "%s: %s\n", s, strerror(e))
int err;

typedef struct Job {
    void (*ftpr)(void **);     // the function pointer
    void **arg_ptr;             // the argument pointer
}Job;

struct QNode {
    Job* job ;
    struct QNode* next;
};

// The queue, head stores the head node of LL and tail stores the
// last node of LL
struct Queue {
    struct QNode *head, *tail;
};

struct QNode* newNode(Job*);

struct Queue* createQueue();

void enQueue(struct Queue*, Job*);

Job* deQueue(struct Queue*);


typedef struct JobScheduler{
    int execution_threads; // number of execution threads
    struct Queue* q; // a queue that holds submitted jobs
    pthread_t* tids; // execution threads...

    int pending_jobs;

    // mutex, condition variable, ...
    pthread_mutex_t mtx; // cond var mutex
    pthread_cond_t cv; // cond var to block threads until queue is ready

    pthread_mutex_t mtxQ; // mutex for taking jobs from queue

    pthread_mutex_t mtxPJ; // mutex for pending_jobs
    pthread_cond_t cvPJ; // condition variable for pending_jobs

    pthread_mutex_t mtxDer; // mutex for derivatives array
}JobScheduler;


struct arg_struct {
    Model model;
    BoW bow;
    double** derivatives;
    int batches;

    Hash_For_Site SiteTable;
    int siteBucketsNum;
    int idBucketsNum;

    JobScheduler* sch;
};


void *master_thread(void *);


JobScheduler* initialize_scheduler(int);

int submit_job(JobScheduler*, Job*);

int execute_all_jobs(JobScheduler*);

int wait_all_tasks_finish(JobScheduler*);

int destroy_scheduler(JobScheduler*);



int get_batches_num(char*);

Job* make_job(Model,BoW,FILE*,Hash_For_Site,int,int,double*,void (*ftpr)(void **),JobScheduler*);

void train_job(void **args);

void test_job(void **args);

void terminate(void **args);
