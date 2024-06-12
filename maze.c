/* 
Patrik Mokrusa 
xmokrup00 
28.11.2023
*/

/* prosim prosim hodne bodu :-) */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct{
    int r;
    int c;
    unsigned char *cells;
} Map;

typedef struct{
    int rows;
    int colls;
} position;

enum { Left, Up, Right, Down, DIRS};

position delta[DIRS] = { // change direction
    {0, -1},
    {-1, 0},
    {0, +1},
    {+1, 0},
};

bool isborder(Map *map, int r, int c, int border){  // structure map, rows and collums of cell in question, wich border you want to check
    int num = map->cells[(map->c*r)+c];  

    if (border == Left){
        if (num == 1){
            return true;
        }
        else if (num == 3)
        {
            return true;
        }
        else if (num == 5)
        {
            return true;
        }
        else if (num == 7)
        {
            return true;
        }
    }
    else if ((border == Down) || (border == Up))
    {
        if (num == 4){
            return true;
        }
        else if (num == 5)
        {
            return true;
        }
        else if (num == 6)
        {
            return true;
        }
        else if (num == 7)
        {
            return true;
        }
    }
    else if (border == Right)
    {
        if (num == 2){
            return true;
        }
        else if (num == 3)
        {
            return true;
        }
        else if (num == 6)
        {
            return true;
        }
        else if (num == 7)
        {
            return true;
        }
    }
    else {
        return false;
    }
    
    return NULL;
}

int start_border(Map *map, int r, int c, int leftright){ // decides the starting border

    int oddeven = (r + c)%2; 

    if (r > map->r -1)
        return -1;   

    if (leftright == Right){
        if ((c == 0) && ((r%2) == 1)){
            return Right;
        }
        if ((c == 0) && ((r%2) == 0)){
            return Down;
        }
        if ((c == map->c -1) && (oddeven == 0)){
            return Up;
        }
        if ((c == map->c -1) && (oddeven == 1)){
            return Left;
        }
        if (r == 0){
            return Left;
        }
        if (r == map->r -1){
            return Right;
        }
    }
    else if (leftright == Left)
    {
        if ((c == 0) && ((r%2) == 1)){
            return Up;
        }
        if ((c == 0) && ((r%2) == 0)){
            return Right;
        }
        if ((c == map->c -1) && (oddeven == 0)){
            return Left;
        }
        if ((c == map->c -1) && (oddeven == 1)){
            return Down;
        }
        if (r == 0){
            return Right;
        }
        if (r == map->r -1){
            return Left;
        }
    }
    return -1;
}

void maze_init(Map maze, FILE* file, unsigned char cells[maze.r * maze.c]){ // scans txt file
    for (int i = 0; i < maze.r; i++){
        for (int j = 0; j < maze.c; j++){
            fscanf(file, "%hhd", &cells[(maze.c*i)+j]);            
        }
    }
    fclose(file);
}

void print_cell(Map maze, int r, int c){ // print cell - used for testing
    printf("%u",maze.cells[(maze.c*r)+c]);
}


int main(int argc,char *argv[]){
    if (argc == 1){
        printf("No arguments\n");
        return 1;
    }
    
    if (strcmp(argv[1], "--help") == 0){ // print guide
        printf("\nHELP - how to use\n\nParameters you can use:\n --help : prints this guide\n --test : used to validate your maze (needs .txt file)\n");
        printf(" --lpath : prints path through your maze using LEFT-HAND RULE (needs starting position and .txt file)\n");
        printf(" --rpath : prints path through your maze using RIGHT-HAND RULE (needs starting position and .txt file)\n");
        return 0;
    }

    char* filename;

    filename = argv[argc -1];

    FILE* file;

    file = fopen(filename, "r");
    if(file==NULL){
        printf("File '%s' not found\n",filename);
        return 1;
    }

    Map maze;
    maze.r = 0;
    maze.c = 0;

    fscanf(file, "%d", &maze.r);
    fscanf(file, "%d", &maze.c);
    

    if (strcmp(argv[1], "--test") == 0) //test .txt file
    {
        if (maze.r <= 0 || maze.c <= 0){
            printf("Invalid\n");
            return 1;
        }

        if(argc != 3){ // check args
            printf("Spatny argument\n");
            return 1;
        }

        unsigned char cells[maze.c * maze.r];
        maze.cells = cells;
        
        int counter = 0;

        while (fscanf(file, "%hhd", &cells[counter]) != EOF){

            int temp = maze.cells[counter];
            if ((temp < 0) || (temp > 7)){
                printf("Invalid\n");
                fclose(file);
                return 1;
            }

            counter ++;
        }

        fclose(file);

        if (counter > ((maze.c) * (maze.r)) || counter < ((maze.c) * (maze.r))){ //check maze tile count
            printf("Invalid\n");
            return 1;
        }

        for (int i = 0; i < maze.r; i++) // check if left right borders are correct
        {
            for (int j = 0; j < maze.c -1; j++)
            {
                // int temp = maze.cells[(maze.c*i)+j];
                if ((isborder(&maze, i, j, Right)) != (isborder(&maze, i, j+1, Left))){
                    // printf("Bad border contact: %d,%d\n",i,j);
                    printf("Invalid\n");
                    return 1;
                }
            }
        }

        for (int j = 0; j < maze.c; j++) // check if vertical borders are correct
        {
            for (int i = 0; i < maze.r -1; i++)
            {
                if ((j % 2) == 0 && i == 0){
                    i++;
                }
                if ((isborder(&maze, i, j, Down)) != (isborder(&maze, i+1, j, Up))){
                    // printf("Bad border contact: %d,%d\n",i,j);
                    printf("Invalid\n");
                    return 1;
                }
                i++;
            }
        }
        
                
        
        printf("Valid\n");
        return 0;
    }
    
    if (argc != 5){ // Check parameters
        printf("Wrong parameters\n");
        return 1;
    }

    position current = {
        .rows = (atoi(argv[2]) -1),
        .colls = (atoi(argv[3]) -1),
    };


    if (strcmp(argv[1], "--lpath") == 0){


        unsigned char cells[maze.c * maze.r];
        maze_init(maze, file, cells);
        int dir = start_border(&maze, current.rows, current.colls, Left);
        
        if (dir == -1){
            printf("Wrong starting position!\n");
            return 1;
        }

        maze.cells = cells;

        while (1){ //lpath algorithm
            if ((isborder(&maze, current.rows, current.colls, dir)) == false){
                printf("%d,%d\n", current.rows +1, current.colls +1);

                current.rows = current.rows + delta[dir].rows; //do a step
                current.colls = current.colls + delta[dir].colls;

                if (current.rows < 0 || current.rows >= maze.r || current.colls < 0 || current.colls >= maze.c){ // if out of bounds return 0;
                    return 0;
                }
                dir = (dir -1) % DIRS; // change dir
                if (dir == -1){
                    dir = 3;
                }
            }
            else{
                dir = (dir +1) % DIRS; 
            }
            if (((current.rows + current.colls )%2 == 1) && (dir == Up)){ // if doesnt have upper border skip direction
                dir = (dir +1) % DIRS;
            }
            else if (((current.rows + current.colls)%2 == 0) && (dir == Down)){
                dir = (dir +1) % DIRS;
            }
        }
    }
    else if (strcmp(argv[1], "--rpath") == 0){

        unsigned char cells[maze.c * maze.r];
        maze_init(maze, file, cells);

        int dir = start_border(&maze, current.rows, current.colls, Right);

        if (dir == -1){
            printf("Wrong starting position!\n");
            return 1;
        }

        maze.cells = cells;

        while (1){ // rpath algorithm
            if ((isborder(&maze, current.rows, current.colls, dir)) == false){
                printf("%d,%d\n", current.rows +1, current.colls +1);

                current.rows = current.rows + delta[dir].rows; // do the step
                current.colls = current.colls + delta[dir].colls;

                if (current.rows < 0 || current.rows >= maze.r || current.colls < 0 || current.colls >= maze.c){
                    return 0;
                }
                dir = (dir +1) % DIRS; //change dir
            }
            else{
                dir = (dir -1) % DIRS;
                if (dir == -1){
                    dir = 3;
                } 
            }
            if (((current.rows + current.colls )%2 == 1) && (dir == Up)){ 
                dir = (dir -1) % DIRS;
                if (dir == -1){
                    dir = 3;
                }
            }
            else if (((current.rows + current.colls)%2 == 0) && (dir == Down)){
                dir = (dir -1) % DIRS;
                if (dir == -1){
                    dir = 3;
                }
            }
        }
    }

    printf("Error\nOops, something went wrong\n\ntry './maze --help' for a guide :)\n");
    return 1;
}