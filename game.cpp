#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
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
    char **mapList = nullptr;
    Pos p;
};

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
void cleanMapList(world *world)
{
    if (world->mapList != NULL)
    {
        for (int j = 0; j < world->mapNumbers; j++)
        {
            if (world->mapList[j] != NULL)
                free(world->mapList[j]);
        }
        free(world->mapList);
    }
}
//*****************************************************************//
void initMapList(world *world, const char *filepath)
{
    char buffer[BUFFER_SIZE];
    char *line;
    char *token;
    const char *delimiter = ",";
    const char *breaks = " ";

    FILE *fstream = fopen(filepath, "r");
    if (fstream == NULL)
    {
        printf("\nFile opening failed");
        return;
    }
    line = fgets(buffer, sizeof(buffer), fstream);
    if (line != NULL)
    {
        char *mapNumbers = strtok(line, breaks);
        world->mapNumbers = atoi(mapNumbers);

        world->mapList = (char **)malloc(world->mapNumbers * sizeof(char *));
        for (int i = 0; i < world->mapNumbers; i++)
        {
            world->mapList[i] = (char *)malloc(256 * sizeof(char));
        }
        if (world->mapList != NULL)
        {
            int row_count = 0;
            while ((line = fgets(buffer, sizeof(buffer), fstream)) != NULL)
            {
                line[strcspn(line, "\n")] = 0;
                char *address = strtok(line, breaks);
                world->mapList[row_count] = strdup(line);
                row_count++;
            }

            fclose(fstream);
        }
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

//*****************************************************************//
void loadMap(world *world)
{
    char *path = world->mapList[world->levelNumber];
    cleanWorld(world);
    getMapFile(world, path);
    printf("\n loadMap %s", world->mapList[world->levelNumber]);
    if ((world->levelNumber + 1) <= (world->mapNumbers - 1))
    {
        world->levelNumber++;
    }
    else
    {
        world->levelNumber = 0;
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
void translate_user_input(world *world, char input)
{
    int new_col = world->p.x + translate_user_input_x(input);
    int new_row = world->p.y + translate_user_input_y(input);

    if (world->userMap[new_row][new_col] == 2)
    {
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
    w.levelNumber = 0;
    const char *path = "listOfMaps.txt";
    cleanWorld(&w);
    initMapList(&w, path);
    loadMap(&w);
    while (1)
    {
        render(&w);
    }
    cleanWorld(&w);
    cleanMapList(&w);
}
