/*
graph.c

Set of vertices and edges implementation.

Implementations for helper functions for graph construction and manipulation.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2024
*/
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include "graph.h"
#include "utils.h"
#include "pq.h"

#define INITIALEDGES 32

/* Definition of a graph. */
struct graph {
  int numVertices;
  int numEdges;
  int allocedEdges;
  struct edge **edgeList;
};

/* Definition of an edge. */
struct edge {
  int start;
  int end;
  int cost;
};

struct graph *newGraph(int numVertices){
  struct graph *g = (struct graph *) malloc(sizeof(struct graph));
  assert(g);
  /* Initialise edges. */
  g->numVertices = numVertices;
  g->numEdges = 0;
  g->allocedEdges = 0;
  g->edgeList = NULL;
  return g;
}

/* Adds an edge to the given graph. */
void addEdge(struct graph *g, int start, int end, int cost){
  assert(g);
  struct edge *newEdge = NULL;
  /* Check we have enough space for the new edge. */
  if((g->numEdges + 1) > g->allocedEdges){
    if(g->allocedEdges == 0){
      g->allocedEdges = INITIALEDGES;
    } else {
      (g->allocedEdges) *= 2;
    }
    g->edgeList = (struct edge **) realloc(g->edgeList,
      sizeof(struct edge *) * g->allocedEdges);
    assert(g->edgeList);
  }

  /* Create the edge */
  newEdge = (struct edge *) malloc(sizeof(struct edge));
  assert(newEdge);
  newEdge->start = start;
  newEdge->end = end;
  newEdge->cost = cost;

  /* Add the edge to the list of edges. */
  g->edgeList[g->numEdges] = newEdge;
  (g->numEdges)++;
}

/* Returns a new graph which is a deep copy of the given graph (which must be 
  freed with freeGraph when no longer used). */
struct graph *duplicateGraph(struct graph *g){
  struct graph *copyGraph = (struct graph *) malloc(sizeof(struct graph));
  assert(copyGraph);
  copyGraph->numVertices = g->numVertices;
  copyGraph->numEdges = g->numEdges;
  copyGraph->allocedEdges = g->allocedEdges;
  copyGraph->edgeList = (struct edge **) malloc(sizeof(struct edge *) * g->allocedEdges);
  assert(copyGraph->edgeList || copyGraph->numEdges == 0);
  int i;
  /* Copy edge list. */
  for(i = 0; i < g->numEdges; i++){
    struct edge *newEdge = (struct edge *) malloc(sizeof(struct edge));
    assert(newEdge);
    newEdge->start = (g->edgeList)[i]->start;
    newEdge->end = (g->edgeList)[i]->end;
    newEdge->cost = (g->edgeList)[i]->cost;
    (copyGraph->edgeList)[i] = newEdge;
  }
  return copyGraph;
}

/* Frees all memory used by graph. */
void freeGraph(struct graph *g){
  int i;
  for(i = 0; i < g->numEdges; i++){
    free((g->edgeList)[i]);
  }
  if(g->edgeList){
    free(g->edgeList);
  }
  free(g);
}

struct solution *graphSolve(struct graph *g, enum problemPart part,
  int numLocations, int startingLocation, int finalLocation){
  struct solution *solution = (struct solution *)
    malloc(sizeof(struct solution));
  assert(solution);

  /* Making new priority queue */
  struct pq *mainPQ = newPQ();

  if(part == PART_A || part == PART_B){
    int visited[numLocations], distance[numLocations];
    for(int i = 0; i < numLocations; i++) {
      distance[i] = INT_MAX;
      visited[i] = 0;
    }

    struct edge *startEdge = (struct edge*)malloc(sizeof(struct edge));
    startEdge->start = startingLocation;
    startEdge->end = startingLocation;
    startEdge->cost = 0;

    enqueue(mainPQ, startEdge, startEdge->cost);

    while(!empty(mainPQ)) {
      struct edge *currE = deletemin(mainPQ); 
      visited[currE->end] = 1;

      /* Check if we want to update the distance array */
      if(currE->cost < distance[currE->end]) {
        distance[currE->end] = currE->cost;
      }

      /* Finding its neighbours */
      for(int i = 0; i < (g->numEdges); i++) { 
        struct edge *curr = (g->edgeList)[i];
        int startV = curr->start;
        int endV = curr->end;

        if((startV == currE->end && !visited[endV]) || (endV == currE->end && !visited[startV])) {
          int current_distance;
          if(endV == currE->end) {
            /* Swapping start and end values */
            int temp  = startV;
            startV = endV;
            endV = temp;

            curr->start = startV;
            curr->end = endV;
          }
          current_distance = distance[startV] + curr->cost;
          if(current_distance < distance[endV]) {
            enqueue(mainPQ, curr, current_distance);
          }
        }
      }
    }
    freePQ(mainPQ);
    if(part == PART_A) {
      solution->damageTaken = distance[finalLocation];
    } else {
      solution->totalCost = distance[finalLocation];
    }
  } else if(part == PART_C) {
    /* Initialize visited array and edge(and its costs) array */
    int visited[numLocations], cost[numLocations];
    for(int i = 0; i < numLocations; i++) {
      visited[i] = 0;
      cost[i] = INT_MAX;
    }

    struct edge *startEdge = (struct edge*)malloc(sizeof(struct edge));
    startEdge->start = startingLocation;
    startEdge->end = startingLocation;
    startEdge->cost = 0;

    enqueue(mainPQ, startEdge, startEdge->cost);

    while(!empty(mainPQ)) {
      struct edge *currE = deletemin(mainPQ);

      if(visited[currE->end] == 1) {
        continue;
      }

      visited[currE->end] = 1;
      cost[currE->end] = currE->cost;

      /* Finding neighbours */
      for(int i = 0; i < (g->numEdges); i++) { 
        struct edge *curr = (g->edgeList)[i];
        int startV = curr->start;
        int endV = curr->end;

        if((startV == currE->end && !visited[endV]) || (endV == currE->end && !visited[startV])) {
          if(endV == currE->end) {
            /* Swapping start and end values */
            int temp  = startV;
            startV = endV;
            endV = temp;

            curr->start = startV;
            curr->end = endV;
          }
          if(curr->cost < cost[curr->end]) {
            enqueue(mainPQ, curr, curr->cost);
          }
        }
      }
    }
    freePQ(mainPQ);

    /* Calculating the sum of the costs */
    int sum = 0;
    for(int i = 0; i < numLocations; i++) {
      sum += cost[i];
    }
    solution->artisanCost = sum;
  } else {
    /* Initialize multiplier array and visited array */
    int visited[numLocations];
    float multiplier[numLocations];
    for(int i = 0; i < numLocations; i++) {
      visited[i] = 0;
      multiplier[i] = 10.0; 
    }

    struct edge *startEdge = (struct edge*)malloc(sizeof(struct edge));
    startEdge->start = startingLocation;
    startEdge->end = startingLocation;
    startEdge->cost = 0;

    enqueue(mainPQ, startEdge, startEdge->cost);
    multiplier[startEdge->end] = 0;

    while(!empty(mainPQ)) {
      struct edge *currE = deletemin(mainPQ);
      float currMultiplier;
      float currCost = currE->cost / 100.0;
      visited[currE->end] = 1;
      
      if(currE->end == 0) {
        currMultiplier = 0;
      } else {
        currMultiplier = multiplier[currE->start] + ((1 + multiplier[currE->start])*currCost);
      }

      if(currMultiplier < multiplier[currE->end]) {
        multiplier[currE->end] = currMultiplier;
      }

      /* Finding neighbours */
      for(int i = 0; i < (g->numEdges); i++) {
        struct edge *curr = (g->edgeList)[i];
        int startV = curr->start;
        int endV = curr->end;

        if((startV == currE->end && !visited[endV]) || (endV == currE->end && !visited[startV])) {
          if(endV == currE->end) {
            /* Swapping start and end values */
            int temp  = startV;
            startV = endV;
            endV = temp;

            curr->start = startV;
            curr->end = endV;
          }
          enqueue(mainPQ, curr, curr->cost);
        }
      }
    }
    freePQ(mainPQ);

    int perInc = multiplier[finalLocation] * 100;
    solution->totalPercentage = perInc;
  }
  return solution;
}
