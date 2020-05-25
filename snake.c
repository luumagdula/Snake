#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <string.h>

typedef struct {
        int x;
        int y;
}point;


point food;
point snake[200]={};
int snakelen = 1;

void newfood()
{
        food.x = rand()%25;
        food.y = rand()%60;
}
void screen(int score)
{
        int i;
        for(i=0;i<=60;i++)
        {
                mvprintw(0,i,"-");
                mvprintw(25,i,"-");
        }
        for(i=0;i<=25;i++)
        {
                mvprintw(i,0,"|");
                mvprintw(i,60,"|");
        }
        mvprintw(1,61,"SNAKE GAME");
        mvprintw(2,61,"Your score: %d",score);
        mvprintw(3,61,"Press 'q' to quit");
}
bool wallcollision()
{
        if(snake[0].y == -1 ||snake[0].x == -1 || snake[0].x == 26 || snake[0].y==61){return true;}
        else{return false;}
}
bool bodycollision()
{
        for(int i=1;i<snakelen;i++){
                if(snake[i].x == snake[0].x && snake[i].y == snake[0].y){return true;}
        }
        return false;
}
void makesnake()
{//tworzenie weza
        point p;
        p.x = 10;
        p.y = 10;
        snake[0] = p;
}
int main(void) 
{
        WINDOW * mainwin;

  // Initializacja ncurses
        if ( (mainwin = initscr()) == NULL ) {
                fprintf(stderr, "Error initialising ncurses.\n");
                exit(EXIT_FAILURE);
        }
        noecho();         
        cbreak();         
        timeout(500);     
        keypad(stdscr,true);

        char direction[6]= "right"; 
        bool quit = false;
        int score = 0;
        makesnake();
        newfood();
do {
                mvprintw(food.x,food.y,"*");
                screen(score);
                int c = getch();
                switch (c)
                {
                        case 'q':
                        quit = true;
                        break;
                        default:
                        break;
                }
//kierunek weza
                int direct = getch();
                if(direct == KEY_UP  && strcmp(direction,"down") != 0){
                        strcpy(direction,"up");
                }
                else if(direct == KEY_LEFT && strcmp(direction,"right") != 0){
                        strcpy(direction,"left");
                }
                else if(direct == KEY_DOWN && strcmp(direction,"up") != 0){
                        strcpy(direction,"down");
                }
                else if(direct == KEY_RIGHT && strcmp(direction,"left") != 0){
                        strcpy(direction,"right");
                }
                clear();
                for(int i=0;i<=snakelen;i++){   //rysowanie weza
                        mvaddstr(snake[i].x,snake[i].y,"o");
                }
                refresh();
                int X = snake[0].x;
                int Y = snake[0].y;

                if(strcmp(direction,"left") == 0){Y--;}
                if(strcmp(direction,"up") == 0){X--;}
                if(strcmp(direction,"down") == 0){X++;}
                if(strcmp(direction,"right") == 0){Y++;}
                //zbieranie jedzenia
                if(food.x == snake[0].x && food.y == snake[0].y){
                        point tail;
                        tail.x = X;
                        tail.y = Y;
                        snakelen++;
                        snake[snakelen]=tail;
                        score++;
                        newfood();
                }
                snake[snakelen-1].x = X;
                snake[snakelen-1].y = Y;

                point po = snake[snakelen-1];
                for(int i=snakelen;i>0;i--){       //przesuniecie weza
                        snake[i]=snake[i-1];
                }
                snake[0]= po;

                if(wallcollision()||bodycollision()){  //kolizja weza
                        clear();
                        bool again = false;
                        while(!again){
                                mvprintw(9,10,"GAME OVER!");
                                mvprintw(10,10,"Your score: %d ",score);
                                mvprintw(11,10,"Press 'a' to try again");
                                mvprintw(12,10,"Press 'q' to quit.");
                                int a = getch();
                                switch (a)
                                {
                                        case 'q':
                                        quit = true;
                                        again = true;
                                        break;
                                        case 'a':
                                        clear();
                                        newfood();
                                        score = 0;
                                        snakelen = 1;
                                        makesnake();
                                        again = true;
                                        break;
                                }
                        }
                }

        } while( ! quit );
  // Czyszczenie
        nocbreak();
        echo();
        refresh();
        delwin(mainwin);
        endwin();
        return EXIT_SUCCESS;
}
