typedef struct Node Node;
typedef struct NodeList NodeList;

typedef struct ListNode {
    Node *currNode;
    struct ListNode *prevNode; //the node before `this` instance
    struct ListNode *nextNode; //the next node in the linked list
} ListNode;


/**
 * DESCRIPTION:
 *    Contains the linked list of nodes, as well as other list information.
**/
struct NodeList {
    unsigned int nodeCount;    //the number of nodes in the list
    ListNode *head;            //pointer to the first node in the list
    ListNode *tail;            //pointer to the last node in the list
};


/**
 * DESCRIPTION:
 *    A structure for holding the solution.
**/
typedef struct SolutionPath {
    Move action;
    struct SolutionPath *next;
} SolutionPath;

/**
 * DESCRIPTION:
 *    This function is used to deallocate a list of type `SolutionPath`.
**/
void destroySolution(SolutionPath **list) {
    SolutionPath *next;
    while(*list) {
        next = (*list)->next;
        free(*list);
        *list = next;
    }
    *list = NULL;
}

/**
 * DESCRIPTION:
 *    This function pushes a node to the list of nodes.
 * PARAMETER:
 *    node - the node to add to the list
 *    list - a pointer to the list pointer to add the node into
 * RETURN:
 *    Returns 1 on success, 0 on failure.
**/
char pushNode(Node *node, NodeList** const list) {
    if(!node)
        return 0;

    ListNode *doublyNode = malloc(sizeof(ListNode));
    if(!doublyNode)
        return 0;

    doublyNode->currNode = node;

    if(*list && !(*list)->nodeCount) {
        (*list)->head = doublyNode;
        (*list)->tail = doublyNode;
        doublyNode->nextNode = NULL;
        doublyNode->prevNode = NULL;
        ++(*list)->nodeCount;
        return 1;
    }

    if(*list == NULL) {
        *list = malloc(sizeof(NodeList));
        if(*list == NULL)
            return 0;

        (*list)->nodeCount = 0;
        (*list)->head = NULL;
        (*list)->tail = doublyNode;
    }
    else {
        (*list)->head->prevNode = doublyNode;
    }

    doublyNode->nextNode = (*list)->head;
    doublyNode->prevNode = NULL;
    (*list)->head = doublyNode;

    ++(*list)->nodeCount;

    return 1;
}


/**
 * DESCRIPTION:
 *    This detatchs the node at the tail of the list. The previous
 *    node of the next node to detatch will be set to NULL.
 * PARAMETER:
 *    list - the list to pop
 * RETURN:
 *    Returns the address of the dettatched node; NULL if the list
 *    is empty.
**/
Node* popNode(NodeList** const list) {
    if(!*list || (*list)->nodeCount == 0)
        return NULL;

    Node *popped = (*list)->tail->currNode;
    ListNode *prevNode = (*list)->tail->prevNode;

    //free the list node pointing to node to be popped
    free((*list)->tail);

    if((*list)->nodeCount == 1) {
        (*list)->head = NULL;
    }
	else {
		prevNode->nextNode = NULL;
	}

    (*list)->tail = prevNode;
    --(*list)->nodeCount;
    return popped;
}

/**
 * DESCRIPTION:
 *    This detatchs the node at the tail of the list.
 *    Returns the address of the dettatched node; NULL if the list
 *    is empty.
 **/

Node* popNode_head(NodeList** const list){
    if(!*list || (*list)->nodeCount == 0)
        return NULL;

    Node *popped = (*list)->head->currNode;
    ListNode *nextNode = (*list)->head->nextNode;

    free((*list)->head);

    if((*list)->nodeCount == 1) {
        (*list)->tail = NULL;
    }
	else {
		nextNode->prevNode = NULL;
	}

    (*list)->head = nextNode;
    --(*list)->nodeCount;
    return popped;

}




void pushList(NodeList **toAppend, NodeList *list) {
    //if either of the list is NULL, the head of the list to be appended is NULL,
    //or the list points to the same starting node
    if(!*toAppend || !list || !(*toAppend)->head || (*toAppend)->head == list->head) {
        return;
    }

    //if the list to append to has currently no element
    if(!list->nodeCount) {
        list->head = (*toAppend)->head;
        list->tail = (*toAppend)->tail;
    }
    else {
        //connect the lists
        (*toAppend)->tail->nextNode = list->head;
        list->head->prevNode = (*toAppend)->tail;
		list->head = (*toAppend)->head;
    }

    //update list information
    list->nodeCount += (*toAppend)->nodeCount;

    free(*toAppend);
    *toAppend = NULL;
}

int totalCost(Node *); //forward declaration for the next function

/**
 * DESCRIPTION:
 *    This is a special node insertion function used for A*. Unlike `insertNode()`,
 *    this inserts the node in order based on the sum of the node's heuristic and
 *    path cost values. Node with the lowest value is sitted to the tail of the queue.
 * PARAMETERS:
 *    toAppend - the list to be appended
 *    list     - the list to append `toAppend` into
**/
void pushListInOrder(NodeList **toAppend, NodeList *list) {
    //if either of the list is NULL, the head of the list to be appended is NULL,
    //or the list points to the same starting node
    if(!*toAppend || !list || !(*toAppend)->head || (*toAppend)->head == list->head) {
        return;
    }

    //if the list to append to has currently no element
    if(!list->nodeCount) {
        pushNode(popNode(toAppend), &list);
    }

    ListNode *toAppendNode; //list node to place in `list`
    ListNode *listNode;     //for traversing each node in `list`
    Node *node;

     while((toAppendNode = (*toAppend)->head)) {
        listNode = list->head;

        while(listNode && totalCost(toAppendNode->currNode) < totalCost(listNode->currNode)) {
            listNode = listNode->nextNode;
        }

        ListNode *temp = toAppendNode->nextNode;

        //if the head node of `toAppend` is to be inserted after the current tail of `list`
        if(!listNode) {
            list->tail->nextNode = toAppendNode;
            toAppendNode->prevNode = list->tail;
            toAppendNode->nextNode = NULL;
            list->tail = toAppendNode;
        }
        else {
            //if if the head node of `toAppend` is to be inserted somewhere in `list` except before its head
            if(listNode->prevNode) {
                toAppendNode->prevNode = listNode->prevNode;
                toAppendNode->nextNode = listNode;
                listNode->prevNode->nextNode = toAppendNode;
                listNode->prevNode = toAppendNode;
            }
            //if the head node of `toAppend` is to be inserted before the head of `list`
            else {
                toAppendNode->nextNode = list->head;
                toAppendNode->prevNode = NULL;
                list->head->prevNode = toAppendNode;
                list->head = toAppendNode;
            }
        }

        (*toAppend)->head = temp;
        --(*toAppend)->nodeCount;
        ++list->nodeCount;
    }

    free(*toAppend);
    *toAppend = NULL;
}
