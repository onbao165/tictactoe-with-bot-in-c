#include<stdio.h>
#include "game.c"
// X 1 
// O 0
int main(){
    int size, condition;
    printf("//////TICTACTOEGAME//////\n");
    printf("Please input your desired board size and winning condition: \n");
    printf("Some rules: 3<= condition,size <=10\n");
    printf("The first player to get n (n is the inputted condition) symbols in a row in any row,col,diagonal order will win\n");
    printf("Input: ");
    do
    {
        scanf("%d%d",&size, &condition);
        fflush(stdin);
        if(size>10 || size<3 || condition>10 || condition<3) printf("Invalid input\n");
        else break;
    } while (1);
    int mode;
    int** board=createBoard(size);
    do{
        printf("Choose MODE:\n");
        printf("1. 2Player\n");
        printf("2. Computer: EASY\n");
        printf("3. Computer: MEDIUM\n");
        printf("4. Computer: IMPOSSIBLE\n");
        printf("Input: ");
        scanf("%d",&mode);
        fflush(stdin);
        if(mode<1 || mode>4) printf("Invalid value\n");
        else break;
    }while(1);
    PVP(board, size, condition, mode);
    free(board);
    return 0;
}
