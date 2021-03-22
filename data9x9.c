/*
George Maddux
March 22 2021
This program takes command line input of a directory in the format of "exampleDirectory/" and processes all files in that directory whose name doesn't begin with a '.'
It is designed to take .sgf format 9x9 go games and collate them into dimensionally expanded game states for use with a neural net. It places each game state sequentially into a text file.
*/
#include<stdio.h>
#include<dirent.h>
#include<string.h>
#include<stdlib.h>

void processMove(char *, FILE *, int gameState[10][9]);

int main(int argc, const char **argv) {
    FILE *stream, *outputStream;
    char *line = NULL, *moveToken = NULL;
    char fileName[1023];
    size_t len = 0;
    ssize_t nread;
    struct dirent *entry = NULL;
    DIR *dp = NULL;
    int i = 0, j = 0;
    int gameState[10][9];
    
    fileName[0] = '\0';
    dp = opendir(argv[1]);
    if(dp != NULL) {
        outputStream = fopen("nineFullData.txt", "w");
        while ((entry = readdir(dp)))
            if(strncmp(entry->d_name, ".", 1) != 0) {
                printf("%s\n", entry->d_name);
                strcat(fileName, argv[1]);
                strcat(fileName, entry->d_name);
                stream = fopen(fileName, "r");
                if(stream == NULL) {
                    printf("error with opening %s\n", fileName);
                    exit(-1);
                }
                for(i = 0; i < 9; i++) {
                    for(j = 0; j < 9; j++) {
                        gameState[i][j] = 0; //0 for empty, -1 for black piece present, 1 for white piece present
                    }
                }
                gameState[10][0] = 0; //row where piece was last placed
                gameState[10][1] = 0; //column where piece was last placed
                gameState[10][2] = -1; //-1 for black to move, 1 for white to move
                gameState[10][3] = 0; //-1 black won, 1 white won, 0 play continues
                for(i = 0; i < 4; i++)
                    nread = getline(&line, &len, stream);
                moveToken = strtok(line, ";");
                moveToken = strtok(NULL, ";");
                while(moveToken != NULL) {
                    processMove(moveToken, outputStream, gameState);
                    moveToken = strtok(NULL, ";");
                }
                fclose(stream);
                fileName[0] = '\0';
            }
            fclose(outputStream);
    }
    closedir(dp);
    return 0;
}

void processMove(char *moveToken, FILE *outputStream, int gameState[10][9]) {
/*    printf("%s is played\n", moveToken);
    fputs(moveToken, outputStream);
    fputs(" is played\n", outputStream); */
    int pieceColor, moveRow, moveCol, i, j;
    //write game state to file
    for(i = 0; i < 9; i++) {
        for(j = 0; j < 9; j++) {
            if(gameState[i][j] == -1) {
                fputs("1 -1 -1\n", outputStream);
            }
            else if(gameState[i][j] == 0) {
                fputs("-1 -1 1\n", outputStream);
            }
            else if(gameState[i][j] == 1) {
                fputs("-1 1 -1\n", outputStream);
            }
        }
    }
    if(gameState[10][0] == 0)
        fputs("0 0 1\n", outputStream); //first move of the game
    else {
        fprintf(outputStream, "%d %d 1\n", gameState[10][0] + 1, gameState[10][1] + 1); //print the coordinate of the last piece placed
    }
    fprintf(outputStream, "%d\n%d\n", gameState[10][2], gameState[10][3]);
    
    //update game state
    if(moveToken[0] == 'B')
        pieceColor = -1;
    else
        pieceColor = 1;
    moveRow = moveToken[2] - 97; //ascii conversion of a to i to 0 to 8
    moveCol = moveToken[3] - 97;
    gameState[moveRow][moveCol] = pieceColor;
    gameState[10][0] = moveRow;
    gameState[10][1] = moveCol;
    gameState[10][2] = pieceColor * -1;
    fprintf(outputStream, "%d\n", gameState[10][0] * 9 + gameState[10][1] + 1);
    if(strlen(moveToken) == 6) {
        gameState[10][3] = pieceColor;
        for(i = 0; i < 9; i++) {
            for(j = 0; j < 9; j++) {
                if(gameState[i][j] == -1) {
                    fputs("1 -1 -1\n", outputStream);
                }
                else if(gameState[i][j] == 0) {
                    fputs("-1 -1 1\n", outputStream);
                }
                else if(gameState[i][j] == 1) {
                    fputs("-1 1 -1\n", outputStream);
                }
            }
        }
        fprintf(outputStream, "%d %d 1\n", gameState[10][0] + 1, gameState[10][1] + 1); //print the coordinate of the last piece placed
        fprintf(outputStream, "%d\n%d\n", gameState[10][2], gameState[10][3]);
        fputs("0\n", outputStream);
    }
}









