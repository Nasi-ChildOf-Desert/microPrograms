#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
 
#define BUFFER_SIZE 256

struct Pos
{
    int x;
    int y;
};

//*****************************************************************//
struct world
{
    int levelNumber;
    int mapNumbers;
    int mapSize;
    int **userMap = nullptr;
    Pos p;
    Pos lastPos;
    Pos bulletPos;
    bool isFired = false;
};

//*****************************************************************//
void pause(int number_of_seconds)
{
    int milli_seconds = 1000 * number_of_seconds;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds);

}
//*****************************************************************//
void cleanWorld(world *world)
{

    if (world->userMap != NULL)
    {
        for (int i = 0; i < world->mapSize; i++)
        {
            if (world->userMap[i] != NULL)
                free(world->userMap[i]);
        }
        free(world->userMap);
    }
}

//*****************************************************************//
void getMapFile(world *world, const char *filepath)
{
    char buffer[BUFFER_SIZE];
    char *line;
    char *token;
    const char *delimiter = ",";
    const char *breaks = " ";

    FILE *fstream = fopen(filepath, "r");
    printf("\nfilepath = %s", filepath);
    if (fstream == NULL)
    {
        printf("\nFile opening failed");
        return;
    }
    line = fgets(buffer, sizeof(buffer), fstream);
    if (line != NULL)
    {
        char *mapDimension = strtok(line, breaks);
        world->mapSize = atoi(mapDimension);

        world->userMap = (int **)malloc(world->mapSize * sizeof(int *));
        for (int i = 0; i < world->mapSize; i++)
        {
            world->userMap[i] = (int *)malloc(world->mapSize * sizeof(int));
        }
        if (world->userMap != NULL)
        {
            int row_count = 0;
            while ((line = fgets(buffer, sizeof(buffer), fstream)) != NULL)
            {
                int col_count = 0;
                token = strtok(line, delimiter);
                while (token != NULL && col_count < world->mapSize)
                {
                    world->userMap[row_count][col_count] = atoi(token);
                    col_count++;
                    token = strtok(NULL, delimiter);
                }
                row_count++;
            }

            fclose(fstream);
        }
    }
}
//**************************************************************** */
void resetPosition(world *world)
{
    world->p.x = 1;
    world->p.y = 1;
}
//*****************************************************************//
void loadMap(world *world)
{
    char  temp[] ="test";  
    char path[20];
    sprintf(path, "%s%d.txt",temp, world->levelNumber);
    cleanWorld(world);
    getMapFile(world, path);
    if ((world->levelNumber) < (3))
    {
        world->levelNumber++;
    }
    else
    {
        world->levelNumber = 1;
    }
}
//*****************************************************************//
int translate_user_input_x(char input)
{
    if (input == 'd')
    {
        return 1;
    }
    else if (input == 'a')
    {
        return -1;
    }
    return 0;
}

//*****************************************************************//
bool translate_user_fire(char input)
{
    if (input == 'x')
    {
        return true;
    }
    
    return false;
}

//*****************************************************************//
void setbulletPosition(world *world)
{
    int diffX = 0;
    int diffY = 0;
    if(world->p.x - world->lastPos.x > 0)
    {
        diffX = 1; 
    }
    else if(world->p.x - world->lastPos.x < 0)
    {
      diffX = -1; 
    }
     if(world->p.y - world->lastPos.y > 0)
    {
        diffY = 1; 
    }
    else if(world->p.y - world->lastPos.y < 0)
    {
      diffY = -1; 
    }
    world->bulletPos.x = world->p.x + diffX;
    world->bulletPos.y = world->p.y + diffY;
 }   
//*****************************************************************//
int translate_user_input_y(char input)
{
    if (input == 'w')
    {
        return -1;
    }
    else if (input == 's')
    {
        return 1;
    }

    return 0;
}
//*****************************************************************//
void displayFiring(world *world, int mapsize)
{
     int dimention = mapsize;
     for (int i = 0; i < dimention; i++)
    {
        for (int j = 0; j < dimention; j++)
        {

             if (world->userMap[world->bulletPos.x][world->bulletPos.y] == 2)
            {
                 resetPosition(world);
                 loadMap(world);
                 return;
            }
             if (world->userMap[world->bulletPos.x][world->bulletPos.y]  == 0)
            {
                std::cout << " -";
                  pause(10);

            }
            else if (world->userMap[world->bulletPos.x][world->bulletPos.y]  == 1)
            {
                break;
            }
      
        }
    }    
}
//*****************************************************************//
void translate_user_input(world *world, char input)
{

    world->lastPos.x = world->p.x;
    world->lastPos.y = world->p.y;
    world->isFired = translate_user_fire(input);
    int new_col = world->p.x + translate_user_input_x(input);
    int new_row = world->p.y + translate_user_input_y(input);
       if(world->isFired)
    {    
       setbulletPosition(world);
       pause(1);
      // displayFiring(world, world->mapSize);
    }
    if ((world->userMap[new_row][new_col] == 2 )|| (world->userMap[world->bulletPos.x][world->bulletPos.y] == 2))
    {
        resetPosition(world);
        loadMap(world);
        return;
    }
    if (world->userMap[new_row][new_col] != 1)
    {
        world->p.x = new_col;
        world->p.y = new_row;
    }
 
    
}

//*****************************************************************//
char get_user_input()
{
    char a = getch();
    return a;
}


//*****************************************************************//
void display(world *world, int mapsize)
{
    int dimention = mapsize;
    system("cls");
    for (int i = 0; i < dimention; i++)
    {
        for (int j = 0; j < dimention; j++)
        {
            if (world->userMap[i][j] == 2)
            {
                std::cout << " $";
            }
            else if (world->userMap[i][j] == 1)
            {
                if (j == 0)
                    std::cout << "#";
                else
                    std::cout << " #";
            }
            else if (i == world->p.y && j == world->p.x)
            {
                std::cout << " *";
            }
            else
            {
                std::cout << "  ";
            }
              if (i == world->bulletPos.x && j== world->bulletPos.y && world->isFired)
            {
                world->bulletPos.y++;
                std::cout << "-";
               

            }
          
        }
        std::cout << std::endl;
    }
}

//*****************************************************************//
void render(world *world)
{
    display(world, world->mapSize);
    char input = get_user_input();
    translate_user_input(world, input);
}

//*****************************************************************//
int main()
{
    world w;
    w.p.x = 1;
    w.p.y = 1;
    w.bulletPos.x = 1;
    w.bulletPos.y = 2;
    w.levelNumber = 1;
    cleanWorld(&w);
    loadMap(&w);
    while (1)
    {
        render(&w);
 
    }
    cleanWorld(&w);
}
