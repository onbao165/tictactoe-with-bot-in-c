#include<stdio.h>
#include<stdlib.h>

//turn will be 1 for X and 0 for O to simplify
void switchTurn(int* turn){ //take pointer to turn
    (*turn)=(*turn)==1 ? 0:1;
}
char toSymbol(int turn){
    if(turn==1) return 'X';
    else if (turn==0) return 'O';
    else return '-';
}
int isEmpty(int** board,int row,int col){
    int val=*(*(board+row)+col);
    return !(val==1 || val==0);
}
void makeMove(int** board,int row,int col,int turn){
    if(isEmpty(board, row, col)){
        *(*(board+row)+col)=turn;
    }
    else{
        printf("Cell already occupied\n");
    }
}
int** createBoard(int size){
    int** board=(int**) malloc(size*sizeof(int*));
    int i,j;
    for(i=0;i<size;i++){
        *(board+i)=(int*) malloc(size*sizeof(int));
    }
    for(i=0;i<size;i++){
        for(j=0;j<size;j++){
            *(*(board+i)+j)=-1;
        }
    }
    return board;
}
int** copyBoard(int** board,int size){
    int** newboard=createBoard(size);
    int i,j;
    for(i=0;i<size;i++){
        for(j=0;j<size;j++){
            *(*(newboard+i)+j)=*(*(board+i)+j);
        }
    }
    return newboard;
}
void printBoard(int** board,int size){
    int i,j;
    // indexing the board for better visual (col index)
    printf("  ");
    for(j=0;j<size;j++){
        printf("  %d ",j);
    }
    printf(" \n");
    for(i=0;i<size;i++){
        printf("%d ",i); //row index
        for(j=0;j<size;j++){
            printf("| %c ", toSymbol(*(*(board+i)+j)));
        }
        printf("|\n");
        printf("  ");
        for(j=0;j<4*size+1;j++) printf("-");
        printf("\n");
    }
}
void printGameState(int gameState){
    switch (gameState){
    case 1:
        printf("Player X win\n");
        break;
    case 0:
        printf("Player O win\n");
        break;
    case 2:
        printf("Tie\n");
    default:
        break;
    }
}
int checkGameState(int** board,int size,int condition){
    //gameState= 1 for X win, 0 for O for win, 2 for tie, -1 for still playable
    int gameState=-1;
    //check rows
    int i,j,k; // i for rows, j for cols, k for checking diagonals
    for(i=0;i<size;i++){
        int current=*(*(board+i));
        int score=1;
        for(j=1;j<size;j++){
            if(current==*(*(board+i)+j) &&  current!=-1){
                score++;
                if((score>=condition) && (gameState=-1)){
                    gameState=current;
                }
            }
            else{
                if(j>size-condition) break;
                score=1;
                current=*(*(board+i)+j);
            }
        }
    }
    //check collumns
    for(j=0;j<size;j++){
        int current=*(*(board)+j);
        int score=1;
        for(i=1;i<size;i++){
            if(current==*(*(board+i)+j) &&  current!=-1){
                score++;
                if((score>=condition) && (gameState=-1)){
                    gameState=current;
                }
            }
             else{
                if(i>size-condition) break;
                score=1;
                current=*(*(board+i)+j);
            }
        }
    }
    //check diagonal from top-left to bottom-right
    for(i=0;i<size-condition+1;i++){ //draw it out to visualize for better understanding
        for(j=0;j<size-condition+1;j++){
            int current=*(*(board+i)+j);
            int score=1;
            for(k=1;k<condition;k++){
                if(current==*(*(board+i+k)+j+k) && current!=-1){
                    score++;
                    if((score>=condition) && (gameState=-1)){
                        gameState=current;
                    }
                }
                else break;
            }
        }
    }
    //checking diagonal from top-right to bottom-left
    for(i=0;i<size-condition+1;i++){ //draw it out to visualize for better understanding
        for(j=condition-1;j<size;j++){
            int current=*(*(board+i)+j);
            int score=1;
            for(k=1;k<condition;k++){
                if(current==*(*(board+i+k)+j-k) && current!=-1){
                    score++;
                    if((score>=condition) && (gameState=-1)){
                        gameState=current;
                    }
                }
                else break;
            }
        }
    }
    //check tie
    int tie=1;
    for(i=0;i<size;i++){
        for(j=0;j<size;j++){
            int current=*(*(board+i)+j);
            if(current==-1) tie=0; //still have empty space
        }
    }
    if(tie) return 2;
    return gameState;
}