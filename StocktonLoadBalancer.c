/**
 * Simple program acts as a gateway that is a middle layer between server instances and the users, 
 it is used to distribute incoming traffic to different servers capable of fulflling those requests 
 in a manner that maximizes throughput, and ensures that no one server is overworked.
 * 
 * @author Brent Stockton
 * @version 1.1
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "LoadBalancer.h"
#include "InstanceHost.h"

struct balancer
{
  pthread_mutex_t *lock; //mutex lock
  int batch_size;
  int num_requests;
  struct host *host;     //keeps track of host info
  struct job_node *head; //keeps track of job info
};

//forward declarations for (public) functions

/**
 * Initializes the load balancer. Takes batch size as parameter.
 */
balancer *balancer_create(int batch_size)
{
  balancer *balance = (balancer *)malloc(sizeof(balancer));

  //array with num jobs
  balance->head = (struct job_node *)malloc(sizeof(struct job_node) * batch_size);

  //mutex lock initalized
  pthread_mutex_init(balance->lock, NULL);

  //num requests should start at 0
  balance->num_requests = batch_size;
  balance->num_requests = 0;

  balance->host = host_create();
  return balance;
}

/**
 * Shuts down the load balancer. Ensures any outstanding batches have
 * completed.
 */
//deallocate memory
void balancer_destroy(balancer **lb)
{
  free(*lb);
  *lb = NULL;
}

/**
 * Adds a job to the load balancer. If enough jobs have been added to fill a
 * batch, will request a new instance from InstanceHost. When job is complete,
 * *data_return will be updated with the result.
 * 
 * @param user_id the id of the user making the request.
 * @param data the data the user wants to process.
 * @param data_return a pointer to a location to store the result of processing.
 */
void balancer_add_job(balancer *lb, int user_id, int data, int *data_return)
{
  pthread_mutex_lock(lb->lock);

  //from user.c
  printf("User #%d: Wants to process data=%d and store it at %p.\n", (int)user_id, data, data_return);

  //increment num_requests
  lb->num_requests++;

  //create new node
  struct job_node *job = (struct job_node *)malloc(sizeof(struct job_node));
  job->data = data;
  job->data_result = data_return;
  job->next = lb->head;

  //move to head of balancer
  lb->head = job;

  //start pocessing if we reach num_requests==batchsize
  if (lb->num_requests == lb->batch_size)
  {
    host_request_instance(lb->host, lb->head);
  }
  //unlock
  pthread_mutex_unlock(lb->lock);
}
