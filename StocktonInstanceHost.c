/**
 * 
 *simple program to dynamically create server instances to deliver *resources (e.g., compute time) on demand. 
 * 
 * @author Brent Stockton
 * @version 1.1
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "InstanceHost.h"

struct host
{
    pthread_t *t;
    int number_of_instances;
};

//forward declarations for (public) functions
pthread_t *create_instance(host *h);

/**
* Initializes the host environment.
*/
//Without threads this will create a new host
host *host_create()
{
    host *host = (struct host *)malloc(sizeof(host));
    host->num_instances = 0;
    host->t = create_instance(host);
    return host;
}

/**
* Shuts down the host environment. Ensures any outstanding batches have
* completed.
*/
//deallocate memory
void host_destroy(host **)
{
    free(*h);

    *h = NULL;
}

/**
* Creates a new server instance (i.e., thread) to handle processing the items
* contained in a batch (i.e., a listed list of job_node). InstanceHost will
* maintain a list of active instances, and if the host is requested to
* shutdown, ensures that all jobs are completed.
*
* @param job_batch_list A list containing the jobs in a batch to process.
*/
void host_request_instance(host *h, struct job_node *batch)
{
    pthread_create(&h->t[h->num_instances], NULL, runner, (void *)batch);
}

//create Array of threads
pthread_t *create_instance(host *h)
{
    return (pthread_t *)malloc(sizeof(pthread_t));
}
