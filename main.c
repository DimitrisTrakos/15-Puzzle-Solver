#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <stdbool.h>

#include "state.h"
#include "list.h"
#include "node.h"
#include "io.h"

unsigned int nodesExpanded;  //number of expanded nodes
unsigned int nodesGenerated; //number of generated nodes
unsigned int solutionLength; //number of moves in solution
double runtime;              //elapsed time (in milliseconds)


SolutionPath* AStar_search(State *, State *);
SolutionPath* IDDF_search(State *, State *,int depth_limit);
int main(void) {


    State initial;           //initial board state
    State goalState;         //goal board configuration

    //solution path of each search method

    SolutionPath *aStar;
    SolutionPath *idds;


    inputInitialState(&initial);



    inputgoalState(&goalState );

    printf("INITIAL BOARD STATE:\n");
    printBoard(initial.board);

    printf("GOAL BOARD STATE:\n");
    printBoard(goalState.board);

    //perform A* search
    aStar = AStar_search(&initial, &goalState);
    printf("\n-------------------------- USING A* ALGORITHM --------------------------\n");
    printSolution(aStar);


    for(int depth_limit=1; depth_limit<1000000; depth_limit++){
    nodesExpanded = 0;
    nodesGenerated = 0;
    solutionLength = 0;
    runtime = 0;


    //perform IDDS
    idds = IDDF_search(&initial, &goalState,depth_limit);
    printf("\n------------------------- USING IDS ALGORITHM  FOR DEPTH LIMIT = %d--------------\n",depth_limit);

    printSolution(idds);

    if(solutionLength){
        depth_limit=1000000;
        }

    destroySolution(&idds);
    }

    destroySolution(&aStar);

    return 0;
}



/**
 * DESCRIPTION:
 *    Our A* implemetation.
 * PARAMETERS:destroySolution(&idds);
 *    initial - address to the initial state
 *    goal    - address to the goal state
 * RETURN:
 *    Returns the solution in a linked list; NULL if the solution is not found.
**/
SolutionPath* AStar_search(State *initial, State *goal) {
    NodeList *queue = NULL;
    NodeList *children = NULL;
    Node *node = NULL;

    //start timer
    clock_t start = clock();

    //initialize the queue with the root node of the search tree
    pushNode(createNode(0, manhattanDist(initial, goal), initial, NULL), &queue);
    Node *root = queue->head->currNode; //for deallocating generated tree

    //while there is a node in the queue to expand
    while(queue->nodeCount > 0) {
        //pop the last node (tail) of the queue
        node = popNode(&queue);

        //if the state of the node is the goal state
        if(statesMatch(node->state, goal))
            break;

        //else, expand the node and update the expanded-nodes counter
        children = getChildren(node, goal);
        ++nodesExpanded;

        //add the node's children to the queue
        pushListInOrder(&children, queue);
    }

    //determine the time elapsed
    runtime = (double)(clock() - start) / CLOCKS_PER_SEC;

    //get solution path in order from the root, if it exists
    SolutionPath *pathHead = NULL;
    SolutionPath *newPathNode = NULL;

    while(node) {
        newPathNode = malloc(sizeof(SolutionPath));
        newPathNode->action = node->state->action;
        newPathNode->next = pathHead;
        pathHead = newPathNode;

        //update the solution length and move on the next node
        ++solutionLength;
        node = node->parent;
    }

    --solutionLength; //uncount the root node

    //deallocate the generated tree
    destroyTree(root);

    return pathHead;
}

SolutionPath* IDDF_search(State *initial, State *goal,int depth_limit) {
    NodeList *queue = NULL;
    NodeList *children = NULL;
    Node *node = NULL;

    //start timer
    clock_t start = clock();

    //initialize the queue with the root node of the search tree
    pushNode(createNode(0, manhattanDist(initial, goal), initial, NULL), &queue);
    Node *root = queue->head->currNode; //for deallocating the generated tree


    while(queue->nodeCount > 0) {
        //pop the last node (tail) of the queue
        node = popNode_head(&queue);

        //if the state of the node is the goal state
        if(statesMatch(node->state, goal))
            break;

        if(node->depth< depth_limit){
        children = getChildren(node, goal);
        ++nodesExpanded;


        pushList(&children, queue);
        }
    }




    //determine the time elapsed
    runtime = (double)(clock() - start) / CLOCKS_PER_SEC;


    SolutionPath *pathHead = NULL;
    SolutionPath *newPathNode = NULL;
if(statesMatch(node->state, goal)){

    while(node) {
        newPathNode = malloc(sizeof(SolutionPath));
        newPathNode->action = node->state->action;
        newPathNode->next = pathHead;
        pathHead = newPathNode;

        //update the solution length and move on the next node
        ++solutionLength;
        node = node->parent;
    }

    --solutionLength; //uncount the root node
    }
    //deallocate the generated tree
    destroyTree(root);

    return pathHead;
}
