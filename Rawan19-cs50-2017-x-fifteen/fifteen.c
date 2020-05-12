/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX]; //board is a local variable, so all the functions can see it

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);



int tile;
int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE* file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();
    
    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file); 
         
         // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }
        
        
         // prompt for move
        printf("tile To Move?");
        tile=get_int();
        
         // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }
        
    
        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);
        
        
        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }
        
            
        draw();
        // sleep thread for animation's sake
        usleep(500000);
    
    }
    // close log
    fclose(file);

    // success
    
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{

    int i,j,start=0,temp=0;
    if(d%2!=0){
    start=d*d-1;
    for (i=0; i<d;i++){    
        for (j=0; j<d;j++){
            board[i][j]= start;
            //the descending order of the elements
            start-=1;
        }

    }
    int a=d-1;
    for(i=(d-1);i<=(d-1);i++){
        for(j=0;j<d;j++){
            board[i][j]=a;
            a-=1;
        }
    }
}
    else if(d%2==0){
    start=d*d-1;
        for (i=0; i<d;i++){
           for (j=0; j<d;j++){
                board[i][j]= start;
                start-=1;
        }
    }
    int b=d-1;
    for(i=(d-1);i<=(d-1);i++){
        for(j=0;j<d;j++){
            board[i][j]=b;
            b-=1;
        }
    }
    //switch 1 & 2, if d is even
    temp=board[d-1][d-3] ;
    board[d-1][d-3]=board[d-1][d-2];
    board[d-1][d-2]=temp;
}
/**
 * Prints the board in its current state.
 */}
void draw(void)
{
    
    int i,j;

    for (i=0; i<d;i++){    
        for (j=0; j<d;j++){
            
            //checking if value is 0 to print a blank tile
            if (board[i][j] == 0) 
                   printf(" _");
            else
                printf("%2i\t",board[i][j]);
        }
       printf("\n");
    }
}


/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
   
 //variables to store the wanted tile number to move  
int tile_i=0,tile_j=0;  
//to keep track of the blank location for i&j
int blank_i,blank_j,tempp; 

    
       int i,j;
    do{
        for (i=0; i<d;i++){
        for (j=0; j<d;j++){
                //keep track of the tile location
                if(board[i][j]==tile){  
                    tile_i=i;
                    tile_j=j;
                }
                //keep track of the blank location
                if(board[i][j]==0){    
                    blank_i=i;
                    blank_j=j;
                }
    }}      //check for legal move,, if blank is next to the tile
            if((blank_i==tile_i && blank_j==tile_j+1) || (blank_i==tile_i && blank_j==tile_j-1)){ 
                
                //legal move
                tempp=board[tile_i][tile_j];         
                board[tile_i][tile_j]=board[blank_i][blank_j];
                board[blank_i][blank_j]=tempp;
                return true;
            }
            //or if blank is above/below the tile
            else if((blank_j==tile_j && blank_i==tile_i+1) || (blank_j==tile_j && blank_i==tile_i-1) ){ 
                //legal move
                tempp=board[tile_i][tile_j];         
                board[tile_i][tile_j]=board[blank_i][blank_j];
                board[blank_i][blank_j]=tempp;
                return true;
            }
            

   else
   return false;
}while(tile>0);
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    int i,j,count=1;
    for (i=0; i<d;i++){    
        for (j=0; j<d;j++){
            count+=1;       
        }
       printf("\n");
    }
    //if the last element in the array is d*d, you won!
    if(count==d*d-1)      
       return true;
    else
        return false;
}


