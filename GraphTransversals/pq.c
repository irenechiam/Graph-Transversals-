/*
pq.c

Unsorted Array Implementation

Implementations for helper functions for priority queue construction and
manipulation.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2022
*/
#include "graph.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define INITIALITEMS 32

struct edge;

struct edge {
  int start;
  int end;
  int cost;
};

struct pq {
  int count;
  int allocated;
  struct edge **queue; // use array containing edges 
  int *priorities; // distances from the starting location to the current location
};


struct pq *newPQ(){
  struct pq *pq = (struct pq *) malloc(sizeof(struct pq));
  assert(pq);
  pq->count = 0;
  pq->allocated = 0;
  pq->queue = NULL;
  pq->priorities = NULL;
  return pq;
}

void enqueue(struct pq *pq, struct edge *newEdge, int priority){
  assert(pq);
  if((pq->count + 1) > pq->allocated){
    if (pq->allocated == 0){
      pq->allocated = INITIALITEMS;
    } else {
      pq->allocated *= 2;
    }
    pq->queue = (struct edge**) realloc(pq->queue, pq->allocated * sizeof(struct edge*));
    assert(pq->queue);
    pq->priorities = (int *) realloc(pq->priorities, pq->allocated *
      sizeof(int));
    assert(pq->priorities);
  }
  //struct edge *newEdge = (struct edge*)malloc(sizeof(struct edge));
  (pq->queue)[pq->count] = newEdge;
  (pq->priorities)[pq->count] = priority;
  (pq->count)++;
}

/* Scan through all the priorities linearly and find lowest. */
// Returning the edge being removed and updating the distance array
// Something wrong here!
struct edge *deletemin(struct pq *pq){
  int i;
  int lowestElement = 0;
  struct edge *returnVal;
  if (pq->count <= 0){
    return NULL;
  }
  for(i = 0; i < pq->count; i++){
    if((pq->priorities)[i] < (pq->priorities)[lowestElement]){
      lowestElement = i;
    }
  }
  returnVal = (pq->queue)[lowestElement];

  /* Delete item from queue by swapping final item into place of deleted
    element. */
  if(pq->count > 0){
    returnVal->cost = (pq->priorities)[lowestElement];
    (pq->priorities)[lowestElement] = (pq->priorities)[pq->count - 1];
    (pq->queue)[lowestElement] = (pq->queue)[pq->count - 1];
    (pq->count)--;
  }
  return returnVal;
}

/* Checking if the priority queue is empty */
int empty(struct pq *pq){
  return pq->count == 0;
}

void freePQ(struct pq *pq){
  if(! pq) {
    return;
  }
  if(pq->allocated > 0){
    free(pq->queue);
    free(pq->priorities);
  }
  free(pq);
}
