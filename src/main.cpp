#include <raylib.h>
#include <cstdlib> //for srand() and rand()
#include <ctime> //for time()
//minesweeper
const Color darkGreen = {20, 160, 145, 255};
const Color endgame = {200, 200, 200, 150};
const int COLS = 10;
const int ROWS = 10;
const int NUM_MINE = 15;
constexpr int screenWidth = 600;
constexpr int screenHeight = 600;
const char* youlose = "Oh no!";
const char* restart = "PRESS 'R' TO PLAY AGAIN ";
const char* youwin = "Oh yeah!";
const char* flag = "FLAG";

int cellHeight = screenHeight / ROWS;
int cellWidth = screenWidth / COLS;


int revealed;
struct Cell{
    int i;
    int j;
    bool haveMine;
    bool reveal;
    bool flag;
    int neighbor;
};
enum GameState{
    PLAYING,
    WIN,
    LOSE
};
Cell grid[ROWS][COLS];
GameState state = PLAYING;
void CellDraw(Cell);
void GridInit();
bool IndexIsValid(int, int);
int CellCountMines(int, int);
void CellReveal(int, int);
void CellFlag(int, int);
void GameInit(void);

int main() 
{

    
    InitWindow(screenWidth, screenHeight, "MINESWEEPER");
    SetTargetFPS(60);
    GameInit();




    while (!WindowShouldClose())
    {
        
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) //left click --> reveal cell
        {
            Vector2 mpos = GetMousePosition();
            int indexI = mpos.x / cellWidth;   //position from pixel --> cell
            int indexJ = mpos.y / cellHeight; 
            if (state == PLAYING && IndexIsValid(indexI, indexJ))
            {
                CellReveal(indexI, indexJ);
            }
        }

        if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) //right click --> reveal/remove flag
        {
            Vector2 mpos = GetMousePosition();
            int indexI = mpos.x / cellWidth;   //position from pixel --> cell
            int indexJ = mpos.y / cellHeight; 
            if (state == PLAYING && IndexIsValid(indexI, indexJ))
            {
                CellFlag(indexI, indexJ);
            }
        }
        if (IsKeyPressed(KEY_R))
        {
            GameInit();
        }
        BeginDrawing();
            ClearBackground(darkGreen);

            for(int i = 0; i < COLS; i++) //draw cell
            {
                for (int j = 0; j < ROWS; j++)
                {
                    CellDraw(grid[i][j]);
                }
            }
            if (state == LOSE)
            {
                DrawRectangle(0,0,screenWidth,screenHeight,endgame);
                DrawText(youlose, (screenWidth - MeasureText(youlose, 40))/2, screenHeight/2 - 40,40,DARKGRAY);
                DrawText(restart, (screenWidth - MeasureText(restart, 40))/2, screenHeight/2 ,40,DARKGRAY);

            }
            if (state == WIN)
            {
                DrawRectangle(0,0,screenWidth,screenHeight,endgame);
                DrawText(youwin, (screenWidth - MeasureText(youwin, 40))/2, screenHeight/2 - 40,40,DARKGRAY);
                DrawText(restart, (screenWidth - MeasureText(restart, 40))/2, screenHeight/2 ,40,DARKGRAY);
            }

        EndDrawing();
        
    }
    
    CloseWindow();
return 0;
};

void CellDraw(Cell cell)
{

    if(cell.reveal)
    {
        if(cell.haveMine)
        {
            DrawRectangle(cell.i * cellWidth, cell.j * cellHeight, cellWidth, cellHeight, RED);
            DrawCircleGradient(cell.i * cellWidth + cellWidth/2, cell.j * cellHeight + cellHeight/2, 25 , RAYWHITE, BLACK); //add mine image later
        }
        else
        {
            DrawRectangle(cell.i * cellWidth, cell.j * cellHeight, cellWidth, cellHeight, LIGHTGRAY);
            if (cell.neighbor > 0)
            {
                DrawText(TextFormat("%d",cell.neighbor),cell.i * cellWidth + 12, cell.j * cellHeight, cellWidth, DARKBLUE);
            }
            
        }
    }
    else if (cell.flag)
    {
        DrawText(flag, cell.i * cellWidth + 9, cell.j * cellHeight + 25, 18, RAYWHITE);
    }
    DrawRectangleLines(cell.i * cellWidth, cell.j * cellHeight, cellWidth, cellHeight, WHITE);
    
}
void GridInit()
{
    for(int i = 0; i < COLS; i++) //set grid
    {
        for (int j = 0; j < ROWS; j++)
        {
            grid[i][j] = (Cell) 
            {
                .i = i,                   /*   position  */     
                .j = j,                       
                .haveMine = false,
                .reveal = false,
                .flag = false,
                .neighbor = -1
            };
        }
    }

    int minePlaced = 0;
    while(minePlaced < NUM_MINE)
    {
        int i = rand() % COLS;
        int j = rand() % ROWS;
        if (grid[i][j].haveMine == false)
        {
            grid[i][j].haveMine = true;
            minePlaced++;
        }

    }

    for(int i = 0; i < COLS; i++) // count neighbor
    {
        for (int j = 0; j < ROWS; j++)
        {
            if(!grid[i][j].haveMine)
                grid[i][j].neighbor = CellCountMines(i , j);
        }
    }
    

}
bool IndexIsValid(int i, int j)
{
    if ((i >= 0 && i < COLS) && (j >= 0 && j < ROWS))
    {
        return true;
    }
    else return false;
}
int CellCountMines(int i, int j)
{
    int count = 0;
    for (int t = -1; t <= 1; t++)
    {
        for (int k = -1; k <= 1; k++)
        {
            if( t == 0 && k == 0) continue;
            if(!IndexIsValid(i + t, j + k))
            {
                continue;
            }
            if(grid[i + t][j + k].haveMine)
            {
                count ++;
            }
            
        }
        
    }
    return count;
    
}



void CellReveal(int i, int j)
{
    if (grid[i][j].flag || grid[i][j].reveal)
    {
        return;
    }
    
    grid[i][j].reveal = true;
    
    if (grid[i][j].haveMine == true)
    {
        state = LOSE;
    }
    else
    {
        revealed++;
        if(revealed == ROWS*COLS - NUM_MINE)
        {
            state = WIN;
        }
    }

}

void CellFlag(int i, int j)
{
    //init flag
    if (grid[i][j].reveal)
    {
        return;
    }
    grid[i][j].flag = !grid[i][j].flag;


};

void GameInit()
{
    srand(time(0));
    GridInit();
    state = PLAYING;
    revealed =0;
}
