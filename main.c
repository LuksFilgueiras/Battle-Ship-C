#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// DEFINE COLORS
#define ANSI_COLOR_RED     "\e[0;31m"
#define ANSI_COLOR_GREEN   "\e[0;32m"
#define ANSI_COLOR_BLUE    "\e[0;34m"
#define ANSI_COLOR_RESET   "\e[0m"
// DEFINE COLORS

struct boat
{
    int size;
    bool isVertical;
    bool isDestroyed;
    char *name;
};

struct board_cell{
    char appearence;
    bool isOccupied;
    struct boat *boatOccupying;
    bool alreadyBombed;
};

bool isPlaying = 0;
static struct board_cell board_cells[10][10];
static struct boat boats[4];
static struct boat boatsPlaced[4];
static int totalPlays = 30;
static int totalBoatsDestroyed = 0;

void generateBoats(){
    int boats_sizes[] = {2,2,4,3};

    for(int i = 0; i < 4; i++){
        struct boat new_boat;
        if(boats_sizes[i] == 2){
            new_boat.name = "Small Boat";
        }
        if(boats_sizes[i] == 3){
            new_boat.name = "Medium Boat";
        }
        if(boats_sizes[i] >= 4){
            new_boat.name = "Large Boat";
        }
        new_boat.size = boats_sizes[i];
        new_boat.isVertical = rand() % 2;
        new_boat.isDestroyed = false;
        boats[i] = new_boat;
    }
}

bool canHaveBoat(struct boat boat, int x, int y){
    if(board_cells[y][x].isOccupied == true){
        return false;
    }

    if(boat.isVertical == false){
        if(x + boat.size > 10){
            return false;
        }
        for(int i = 0; i < boat.size; i++){
            if(board_cells[y][x + i].isOccupied == true){
                return false;
            }
        }
    }
    
    if(boat.isVertical == true){
        if(y + boat.size > 10){
            return false;
        }

        for(int i = 0; i < boat.size; i++){
            if(board_cells[y + i][x].isOccupied == true){
                return false;
            }
        }
    }

    return true;
}

void placeBoat(struct boat *boat, int x, int y){
    if(boat->isVertical == true){
        for(int i = 0; i < boat->size; i++){
            board_cells[y + i][x].isOccupied = true;
            board_cells[y + i][x].boatOccupying = boat;

            // UNCOMMENT LINE BELOW TO SHOW BOAT PLACED IN BOARD
            //board_cells[y + i][x].appearence = 'B';
        }
    }
    else{
         for(int i = 0; i < boat->size; i++){
            board_cells[y][x + i].isOccupied = true;
            board_cells[y][x + i].boatOccupying = boat;

            // UNCOMMENT LINE BELOW TO SHOW BOAT PLACED IN BOARD
            //board_cells[y][x + 1].appearence = 'B';
        }
    }
}

void populateBoard(){
    for (int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++){
            struct board_cell new_board_cell;
            new_board_cell.isOccupied = false;
            new_board_cell.appearence = 178;
            new_board_cell.alreadyBombed = false;
            board_cells[i][j] = new_board_cell;
        }
    }

    for(int i = 0; i < 4;){
        int pos_x = rand() % 11;
        int pos_y = rand() % 11;

        if(canHaveBoat(boats[i], pos_x, pos_y) == 0){
            continue;
        }else{
            placeBoat(&boats[i], pos_x, pos_y);
            i++;
        }
    }
}


void viewBoard(){ 
    printf("   ");
    for(int i = 0; i < 10; i++){
        printf(" %d", i);
    }
    printf("\n");

    for (int i = 0; i < 10; i++)
    {
        printf("%d - ", i); 
        for(int j = 0; j < 10; j++){
            if(board_cells[i][j].boatOccupying->isDestroyed == false){
                printf(ANSI_COLOR_RESET);
                if(board_cells[i][j].appearence == 'X'){
                    printf(ANSI_COLOR_RED "%c " ANSI_COLOR_RESET, board_cells[i][j].appearence);
                }else{
                    printf(ANSI_COLOR_BLUE "%c " ANSI_COLOR_RESET, board_cells[i][j].appearence);
                }
            }
            else{
                printf(ANSI_COLOR_GREEN "%c " ANSI_COLOR_RESET, board_cells[i][j].appearence);
            }
        }
        printf("\n");
    }

    printf("\n Rounds left: %d\n", totalPlays);
    printf("\n ------BOATS DESTROYED------ \n");
    for(int i = 0; i < 4; i++){
        if(boats[i].isDestroyed == true){
            printf(ANSI_COLOR_GREEN " %s\n" ANSI_COLOR_RESET, boats[i].name);
        }
    }
    printf("\n --------------------------- \n");
}

void dropBomb(int x, int y){
    if(board_cells[y][x].alreadyBombed == true){
        return;
    }

    if(board_cells[y][x].isOccupied == true){
        board_cells[y][x].appearence = 'X';
        board_cells[y][x].boatOccupying->size--;
        printf("boat size: %d", board_cells[y][x].boatOccupying->size);
        if(board_cells[y][x].boatOccupying->size == 0){
            board_cells[y][x].boatOccupying->isDestroyed = true;
            totalBoatsDestroyed++;
        }
    }
    else{
        board_cells[y][x].appearence = 'o';
        totalPlays--;
    }

    board_cells[y][x].alreadyBombed = true;
}

int readNumber(){
    int number = 100;
    while(1){
        if(scanf("%d", &number) == 0){
            printf("Invalid Input - please enter X Y as a number!\n");
            fseek(stdin, 0, SEEK_END);  
        }else{
            if(number < 10){
                return number;
            }
            else{
                printf("Out of range!\n");
            }
        }
    };
}

int main(){
    srand(time( NULL ));
    generateBoats();
    populateBoard();

    // Pre-Load Colors
    viewBoard();
    system("cls");

    unsigned int x_pos = 100;
    unsigned int y_pos = 100;

    isPlaying = 1;

    while(isPlaying){
        if(totalBoatsDestroyed >= 4 || totalPlays <= 0){
            isPlaying = 0;
            break;
        }
        viewBoard();
        printf("\nPlay X Y position: \n");
        x_pos = readNumber();
        y_pos = readNumber();
        fseek(stdin, 0, SEEK_END);
        dropBomb(x_pos, y_pos);
        system("cls");
    }

    system("cls");

    if(totalBoatsDestroyed >= 4){
        printf("Winner!");
    }else{
        printf("Loser!");
    }

    printf("\nPress any key to finish!\n");
    getchar();
    
    return 0;
}