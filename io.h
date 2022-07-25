extern unsigned int nodesExpanded;
extern unsigned int nodesGenerated;
extern unsigned int solutionLength;
extern double runtime;






void inputInitialState(State * const state){
state->action=NOT_APPLICABLE;


int row,col;
int symbol=0;
for(row = 0; row < 4; ++row) {
        for(col = 0; col < 4; ++col) {
                 state->board[row][col] =symbol + '0';
                  symbol++;

        }
}

for(row = 0; row < 4; ++row) {
        for(col = 0; col < 4; ++col) {    // shuffle array
   char temp =  state->board[row][col];
    int randomIndexrow = rand() % 4;
     int randomIndexcol = rand() % 4;
    state->board[row][col] = state->board[randomIndexrow][randomIndexcol];
    state->board[randomIndexrow][randomIndexcol] = temp;
}

}

//begins the with a standard initial state to check if the algorithms work at least to a small amaount of nodes

/*state->board[0][0] =1 +'0';
state->board[0][1] =2 +'0';
state->board[0][2] =3 +'0';
state->board[0][3] =4 +'0';
state->board[1][0] =5 +'0';
state->board[1][1] =6 +'0';
state->board[1][2] =7 +'0';
state->board[1][3] =8 +'0';
state->board[2][0] =9 +'0';
state->board[2][1] =10 +'0';
state->board[2][2] =11 +'0';
state->board[2][3] =12+'0';
state->board[3][0] =0+'0';
state->board[3][1] =13+'0';
state->board[3][2] =14 +'0';
state->board[3][3] =15 +'0';
*/
}
void inputgoalState(State * const state){
state->action=NOT_APPLICABLE;
int row,col;

int symbol=1;
for(row = 0; row < 4; ++row) {
        for(col = 0; col < 4; ++col) {
                 state->board[row][col] = symbol + '0';
                 symbol++;
                 if(row==3 && col==3){
                    symbol=0;

                    state->board[row][col]= symbol + '0';
                 }
        }

}
}
/**
 * DESCRIPTION: This displays contents of `board` to the standard output
**/
void printBoard(char const board[][4]) {
    int row, col;
    char a=0;
    for(row = 0; row < 4; ++row) {
        printf("+---+---+---+---+---+\n");
        for(col = 0; col < 4; ++col) {
                a=board[row][col]-'0';
            printf("| %d ", a);
        }
        printf("|\n");
    }
    printf("+---+---+---+---+---+\n");
}

/**
 * DESCRIPTION:
 *    This function interprets numerical instructions of the move to make,
 *    to it's verbal counterpart to be displayed to the screen.
 * PARAMETER:
 *    solution - the solution path consisting a list of nodes from the root
 *               to the goal
**/
void printSolution(struct SolutionPath *path) {
	//check if solution exists
    if(!path) {
        printf("No solution found.\n");
        return;
    }

	//if the initial state is already the goal state
	if(!path->next) {
		printf("No moves needed. The initial state is already the goal state.\n");
		return;
	}

    printf("SOLUTION: (Relative to the space character)\n");

    //will use hash map to speed up the proccess a bit
    char *move[4] = { "UP", "DOWN", "LEFT", "RIGHT" };
    int counter = 1;

    //will be skipping the first node since it represents the initial state with no action
    for(path = path->next; path; path = path->next, ++counter) {
        printf("%i. Move %s\n", counter, move[path->action]);
    }

    printf(
        "DETAILS:\n"
        " - Solution length : %i\n"
        " - Nodes expanded  : %i\n"
        " - Nodes generated : %i\n"
        " - Runtime         : %g milliseconds\n"
        " - Memory used     : %i bytes\n", //only counting allocated `Node`s
        solutionLength, nodesExpanded, nodesGenerated, runtime, nodesGenerated * sizeof(Node));
}
