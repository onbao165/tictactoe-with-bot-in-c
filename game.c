#include"board.c"
#include<time.h>
//MAX MIN FUNCTION
int max(int a,int b){
    return (a>=b) ? a:b;
}
int min(int a,int b){
    return (a<=b) ? a:b;
}
//MAKING PLAYER MOVE
void getPlayerMove(int** board,int size,int turn){
    int row,col;
    do{
        printf("Player %c move: ", toSymbol(turn));
        scanf("%d%d",&row,&col);
        fflush(stdin);
        if(row>=size || row<0 || col>=size || col<0) printf("Invalid position\n");
        else break;
    }while(1);
    makeMove(board, row, col, turn);
}
//FOR TRANSFORMING 2D TO 1D AND VICE VERSA
int translateTo1D(int size,int row,int col){
    return row*size+col; //convert to 1D index
}
int* translateTo2D(int index, int size){
    int* arr=(int*) malloc(2*sizeof(int));
    *(arr)=index/size;
    *(arr+1)=index%size;
    return arr; //get row index and collumn index
}
//GETTING LEGAL MOVES AS A 1D ARRAY
int countLegalMove(int** board,int size){
   int i,j;
    int count=0;
    for(i=0;i<size;i++){
        for(j=0;j<size;j++){
            if(*(*(board+i)+j)==-1) count++;  
        }
    }
    return count; //array as parameter cant be measured in length so we need to get the length using another function
}
int* getLegalMove(int** board,int size){
    int i,j;
    int len=countLegalMove(board, size);
    int k=0;
    int* arr=(int*) malloc(len*sizeof(int));
    for(i=0;i<size;i++){
        for(j=0;j<size;j++){
            if(*(*(board+i)+j)==-1) {
                arr[k]=translateTo1D(size, i, j);
                k++;
            }
        }
    }
    return arr;
}
//EASY BOT
int* getEasyBotMove(int* legalMoves,int len,int size){
    //len is legalMoves length 
    //creating seed for different randomization
    time_t t1;
    srand((unsigned) time (&t1));
    int randomMove=rand()%len;
    return translateTo2D(legalMoves[randomMove], size);
}
//MEDIUM BOT
int* getMediumBotMove(int* legalMoves,int len,int ** board,int size,int condition){
    int i;
    //Checking if computer can win
    for(i=0;i<len;i++){
        int* expectMove=translateTo2D(*(legalMoves+i), size);
        int** newboard=copyBoard(board, size);
        makeMove(newboard, *expectMove, *(expectMove+1), 0);
        int result=checkGameState(newboard, size, condition);
        if(result==0) return expectMove;
        else free(expectMove);
        free(newboard);
    } 
    //Checking if player win, then return that position
    for(i=0;i<len;i++){
        int* expectMove=translateTo2D(*(legalMoves+i), size);
        int** newboard=copyBoard(board, size);
        makeMove(newboard, *expectMove, *(expectMove+1), 1);
        int result=checkGameState(newboard, size, condition);
        if(result==1) return expectMove;
        else free(expectMove);
        free(newboard);
    }
    //return random move if there is no possible computer win or player win
    return getEasyBotMove(legalMoves, len, size);
}
//HARD BOT
int alphaBeta(int** board,int size,int condition,int depth,int alpha,int beta,int isMaximizing){
    int gameState=checkGameState(board, size, condition);
    if(gameState==1) return 10;
    else if(gameState==0) return -10;
    else if(gameState==2) return 0;
    else if(depth==0) return 0;
    //check possible move and get value for each move
    int* legalMoves=getLegalMove(board, size);
    int len=countLegalMove(board, size);
    int i;
    int maxChoice=-100;
    int minChoice=100;
    for(i=0;i<len;i++){
        int* move=translateTo2D(*(legalMoves+i), size);
        int** newboard=copyBoard(board, size); //creating a new board for every move
        makeMove(newboard, *move, *(move+1), isMaximizing);
        int result=alphaBeta(newboard, size, condition, depth-1, alpha, beta, !isMaximizing);
        if(isMaximizing){
            alpha=max(result, alpha); 
            if(alpha>=beta) return result; //alpha beta prune
            maxChoice=max(result, maxChoice);
        }
        else{
            beta=min(result, beta);
            if(alpha>=beta) return result;
            minChoice=min(result, minChoice);
        }
        free(move);
        free(newboard);
    }
    if(isMaximizing){
            return maxChoice;
        }
        else return minChoice;
    free(legalMoves);
}
int* getHardBotMove(int* legalMoves,int len,int** board,int size,int condition,int depth,int alpha,int beta,int turn){ //Using minimax algorithm and alpha beta pruning
    //As this is bot turn we input argument turn as 0 also 0 is for minimizing the result in alphaBeta function
    int choice=100;
    int bestMove=0;
    int i;
    //Checking if computer can win
    for(i=0;i<len;i++){
        int* expectMove=translateTo2D(*(legalMoves+i), size);
        int** newboard=copyBoard(board, size);
        makeMove(newboard, *expectMove, *(expectMove+1), 0);
        int result=checkGameState(newboard, size, condition);
        if(result==0) return expectMove;
        else free(expectMove);
        free(newboard);
    } 
    //Checking if player win, then return that position
    for(i=0;i<len;i++){
        int* expectMove=translateTo2D(*(legalMoves+i), size);
        int** newboard=copyBoard(board, size);
        makeMove(newboard, *expectMove, *(expectMove+1), 1);
        int result=checkGameState(newboard, size, condition);
        if(result==1) return expectMove;
        else free(expectMove);
        free(newboard);
    }
    //Find best move using alphaBeta
    for(i=0;i<len;i++){
        int** newboard=copyBoard(board, size);
        int* move=translateTo2D(*(legalMoves+i), size);
        makeMove(newboard, *move, *(move+1), 0);
        int val=alphaBeta(newboard, size, condition, depth, -100, 100, 1);
        if(choice>=val){
            choice=val;
            bestMove=i;
        }
        free(move);
        free(newboard);
    }
    return translateTo2D(*(legalMoves+bestMove), size);
}
void PVP(int** board, int size, int condition,int mode){
    printBoard(board, size);
    printf("Please input position by row-index and col-index (Ex. 0 1)\n");
    int result;
    int turn=1;
    int depth=size*size-1;
    do{
        getPlayerMove(board, size, turn);
        switchTurn(&turn);
        result=checkGameState(board, size, condition);
        if(result!=-1){
            printBoard(board, size);
            printGameState(result);
            break;
        }
        int len=countLegalMove(board, size);
        int* legalMoves=getLegalMove(board, size);
        switch(mode){
            case 1:
                printBoard(board, size);
                getPlayerMove(board, size, turn);
                break;
            case 2:
                int* eMove=getEasyBotMove(legalMoves, len, size);
                makeMove(board, *eMove, *(eMove+1), turn);
                free(eMove);
                break;
            case 3:
                int* mMove=getMediumBotMove(legalMoves, len, board, size, condition);
                makeMove(board, *mMove, *(mMove+1), turn);
                free(mMove);
                break;
            case 4:
                int* hMove=getHardBotMove(legalMoves, len, board, size, condition, depth-1, -100, 100, turn);
                makeMove(board, *hMove, *(hMove+1), turn);
                free(hMove);
                break;
        }
        depth--;
        switchTurn(&turn);
        free(legalMoves);
        printBoard(board, size);
        result=checkGameState(board, size, condition);
        printGameState(result);
    }while(result==-1);
}