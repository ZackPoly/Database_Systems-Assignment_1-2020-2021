#include "job_scheduler.h"

// A utility function to create a new linked list node.
struct QNode* newNode(Job* nodeJob)
{
    struct QNode* temp = (struct QNode*)malloc(sizeof(struct QNode));
    temp->job = nodeJob;
    temp->next = NULL;
    return temp;
}

// A utility function to create an empty queue
struct Queue* createQueue()
{
    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue));
    q->head = q->tail = NULL;
    return q;
}

// The function to add a job k to q
void enQueue(struct Queue* q, Job* nodeJob)
{
    // Create a new LL node
    struct QNode* temp = newNode(nodeJob);

    // If queue is empty, then new node is head and tail both
    if (q->tail == NULL) {
        q->head = q->tail = temp;
        return;
    }

    // Add the new node at the end of queue and change tail
    q->tail->next = temp;
    q->tail = temp;
}

// Function to remove a job from given queue q
Job* deQueue(struct Queue* q)
{
    // If queue is empty, return NULL.
    if (q->head == NULL)
        return NULL;

    // Store previous head and move head one node ahead
    struct QNode* temp = q->head;
    Job* j = temp->job;

    q->head = q->head->next;

    // If head becomes NULL, then change tail also as NULL
    if (q->head == NULL)
        q->tail = NULL;

    free(temp);
    return j;
}
