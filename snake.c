#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

void drawBoard(int xmax, int ymax);
void insertStruct(void);
void print(void);
int getInput(int prevmtn);
int moveSnake(int currmtn, int prevmtn, int r[2]);
bool checkCollision(int xmax, int ymax);
void generatexy(int xmax, int ymax, int r[2]);

//cell for the snake, the snake is made up from a linked list
typedef struct snake{
  int x;
  int y;
  struct snake* next;
}snake;

//creating a pointer to point to the back of the snake
snake* top = NULL;

//this function creates the boundary of the snake game
void drawBoard(int xmax, int ymax){
  
    for(int i = 0;i < ymax-1;++i){
     printw("#");
     for(int j = 0;j < xmax-3;++j){
      if (i == 0 || i == ymax - 2){
        printw("#");
      } else{
         printw(" ");
      } 
     }
      printw("#\n");
    }
}

//this function creates the snake cells, the function creates a linked list
snake *temp;
void insertStruct(void){

    temp = malloc(sizeof(snake));
    //create the first cell of the snake
    if (top == NULL){
     temp->x = 45;
     temp->y = 15;
     temp->next = top;
     top = temp;
    } else if (top->next != NULL){ //create a new cell of the snake if the snake is 2 cells or bigger
        if (top->next->x - top->x == 1){
         temp->x = top->x - 1;
        } else if (top->next->x - top->x == -1){
           temp->x = top->x + 1;
        } else if (top->x == top->next->x){
           temp->x = top->x;
        }

        if (top->next->y - top->y == 1){
         temp->y = top->y -1;
        } else if (top->next->y - top->y == -1){
           temp->y = top->y + 1;
        } else if (top->y == top->next->y){
           temp->y = top->y;
        }

        temp->next = top;
        top = temp;
   
    } else if (top->next == NULL){ //create a new cell of the snake if the snake is 1 cell big
       temp->x = top->x -1;
       temp->y = top->y;
       temp->next = top;
       top = temp;
    }
  
}

//this function prints the snake
void print(void){
    snake* t = top;
    const char i = '+';
  
    while (t != NULL){
     mvaddch(t->y,t->x,i);
     t = t->next;
    }
  
}
//this function gets input from the user
int getInput(int prevmtn){

    keypad(stdscr,TRUE);

    enum arrows {up,down,left,right};
    int i = 0;
    int curmtn = right;
    

    i = getch();

    if (i == KEY_UP){
       curmtn = up;
       return curmtn;
    } else if (i == KEY_DOWN){
      curmtn = down;
      return curmtn;
    } else if (i == KEY_LEFT){
      curmtn = left;
      return curmtn;
    } else if (i == KEY_RIGHT){
      curmtn = right;
      return curmtn;
    } else
      return prevmtn;

}

//this function moves the snake to its new position
int moveSnake(int curmtn, int prevmtn, int r[2]){
 
    enum movement {up,down,left,right};

    if (prevmtn == left && curmtn == right){//prevent snake from reversing its direction
      curmtn = prevmtn;
    } else if (prevmtn == right && curmtn == left){//prevent snake from reversing its direction
       curmtn = prevmtn;
    } else if (prevmtn == up && curmtn == down){//prevent snake from reversing its direction
       curmtn = prevmtn;
    } else if (prevmtn == down && curmtn == up){//prevent snake from reversing its direction
       curmtn = prevmtn;
    }

//creating pointer to point to the back of the snake
    snake* t = top;
    while (t != NULL){//iterate through the linked list
     if (t->next != NULL){//if the cell is not the head of the snake
      if (t->y == t->next->y){
        t->x += (t->next->x - t->x); 
      } else if (t->x == t->next->x){
         t->y += (t->next->y - t->y);
      }
     } else if (t->next == NULL){//if the cell is the head of the snake
        if (curmtn == up){
           --t->y;
        } else if (curmtn == down){
           ++t->y;
        } else if (curmtn == left){
           --t->x;
        } else if (curmtn == right){
            ++t->x;
        }
      
        if (t->x == r[0] && t->y == r[1]){//checking if the snake ate the food
           mvdelch(r[1],r[0]);
           insertStruct();//insert a new cell
           r[0] = 0;
           r[1] = 0;
         }
      }
     t = t->next;
    }

  return curmtn;
  
}
//this function checks for collisions
bool checkCollision(int xmax, int ymax){

    snake* t = top;
    while(t->next != NULL){//iterate to the head of the snake
     t = t->next;
    }
  
    if (t->x == xmax-2 || t->y == ymax-2){
     return false;
    } else if (t->x == 0 || t->y == 0){
       return false;
    }
    snake* i = top;
    while (i->next != NULL){
     if (i->x == t->x && i->y == t->y){
      return false;
     }
     i = i->next;
    }
    return true;
}
//this function generates food for the board
void generatexy(int xmax, int ymax, int r[2]){

    // int lowerX = 1;
    int upperX = xmax - 3;
    // int lowerY = 1;
    int upperY = ymax - 3;
    int rx = 0;
    int ry = 0;
    bool randState = true;
    const char i = '+';
    if (r[0] == 0 && r[1] == 0){//creating new food
      
      while (randState){
        srand(time(0));
        rx = (rand() % upperX) + 1; 
        ry = (rand() % upperY) + 1;
       
        snake* t = top;
        while (t != NULL){//checking the new food is not colliding with the snake
         if (t->x == rx && t->y == ry){
             break;
         } else if (t->next == NULL){
            randState = false;
         }
          t = t->next;
        }
        if (randState == false){//printing new food
         mvaddch(ry,rx,i);
         r[0] = rx;
         r[1] = ry;
        }

      }
    } else{
        mvaddch(r[1],r[0],i);//print the old food
    }

}

int main(void){
  
    initscr();
    raw();
    timeout(100);
    curs_set(0);

    bool w = true;
    int curmtn = 0;//current motion
    int prevmtn = 3;//previous motion, 3 corresponds to the right direction
    int ymax = 0;
    int xmax = 0;
    int r[2] = {0};

    getmaxyx(stdscr,ymax,xmax);

    //create the initial snake
    insertStruct();
    insertStruct();
    insertStruct();
    insertStruct();

    //start of the game
    while(w){
     generatexy(xmax,ymax,r);
     curmtn = getInput(prevmtn);
     prevmtn = moveSnake(curmtn,prevmtn,r);
     clear();
     drawBoard(xmax,ymax);
     print();
     refresh();
     w = checkCollision(xmax,ymax);
    }
    endwin();

    while (top != NULL){
     snake* t = top;
     top = top->next;
     free(t);
    }
  
    return EXIT_SUCCESS;
}
