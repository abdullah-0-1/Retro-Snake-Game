#include<iostream>
#include<windows.h>
#include<conio.h>
#include<math.h>
#include<fstream>
#include<time.h>
#include<string>
using namespace std;

#define ROWS 100
#define COLS 200

void getRowColbyLeftClick(int& rpos, int& cpos)
{
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD Events;
    INPUT_RECORD InputRecord;
    SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT |
        ENABLE_EXTENDED_FLAGS);
    do
    {
        ReadConsoleInput(hInput, &InputRecord, 1, &Events);
        if (InputRecord.Event.MouseEvent.dwButtonState ==

            FROM_LEFT_1ST_BUTTON_PRESSED)

        {
            cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
            rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
            break;
        }
    } while (true);
}

void gotoRowCol(int rpos, int cpos)
{
    COORD scrn;
    HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
    scrn.X = cpos;
    scrn.Y = rpos;
    SetConsoleCursorPosition(hOuput, scrn);
}

void color(int k)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, k);
}

void hideConsoleCursor()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

// ******************************************************************

struct Pos
{
    int r, c;
};

enum Dir
{
    Up, Down, Left, Right
};

struct Snake
{
    Pos* Ps;
    Dir D;
    int size;
    int Color;
    int Uk, Dk, Lk, Rk;
    bool IsAlive;
    int score;
};

void Initialization(Snake*& S, int rows, int cols,int& NOP)
{
    gotoRowCol(rows / 2, cols / 2);
    cout << "Enter Number of players: ";
    cin >> NOP;
    S = new Snake[NOP]{};
    for (int i = 0; i < NOP; i++)
    {
        S[i].size = 3;
        S[i].Ps = new Pos[S[i].size]{ {rows / 2+(i*3),cols / 2 + (i * 2)}, {rows / 2 + (i * 3),cols / 2 + 1 + (i * 2)}, {rows / 2 + (i * 3),cols / 2 + 2 + (i * 2)}};
        S[i].D = Left;
        S[i].Color = 4 + i;
        gotoRowCol(rows / 2 + (i * 5), cols / 2 - 40 - i);
        cout << "Enter up key of snake " << i << ": ";
        S[i].Uk = _getch();
        if (S[i].Uk == 224)
        {
            S[i].Uk = _getch();
        }

        gotoRowCol(rows / 2 + (i * 5), cols / 2 - 40 - i);
        cout << "Enter Down key of snake " << i << ":";
        S[i].Dk = _getch();
        if (S[i].Dk == 224)
        {
            S[i].Dk = _getch();
        }

        gotoRowCol(rows / 2 + (i * 5), cols / 2 - 40 - i);
        cout << "Enter Right key of snake: " << i;
        S[i].Rk = _getch();
        if (S[i].Rk == 224)
        {
            S[i].Rk = _getch();
        }

        gotoRowCol(rows / 2 + (i * 5), cols / 2 - 40 - i);
        cout << "Enter Left key of snake: " << i;
        S[i].Lk = _getch();
        if (S[i].Lk == 224)
        {
            S[i].Lk = _getch();
        }
        S[i].IsAlive = true;
        S[i].score = 0;
    }
        
}

void Initialization_2(fstream& rdr, Snake*& S, int& rows, int& cols, int& NOP)
{
    rdr >> rows >> cols;
    gotoRowCol(rows / 2, cols / 2);
    cout << "Enter Number of players: ";
    cin >> NOP;
    S = new Snake[NOP]{};
    for (int i = 0; i < NOP; i++)
    {
        S[i].size = 3;
        S[i].Ps = new Pos[S[i].size]{ {rows / 2 + (i * 3),cols / 2 + (i * 2)}, {rows / 2 + (i * 3),cols / 2 + 1 + (i * 2)}, {rows / 2 + (i * 3),cols / 2 + 2 + (i * 2)} };
        S[i].D = Left;
        S[i].Color = 4 + i;
        gotoRowCol(rows / 2 + (i * 5), cols / 2 - 40 - i);
        cout << "Enter up key of snake " << i << ": ";
        S[i].Uk = _getch();
        if (S[i].Uk == 224)
        {
            S[i].Uk = _getch();
        }

        gotoRowCol(rows / 2 + (i * 5), cols / 2 - 40 - i);
        cout << "Enter Down key of snake " << i << ":";
        S[i].Dk = _getch();
        if (S[i].Dk == 224)
        {
            S[i].Dk = _getch();
        }

        gotoRowCol(rows / 2 + (i * 5), cols / 2 - 40 - i);
        cout << "Enter Right key of snake: " << i;
        S[i].Rk = _getch();
        if (S[i].Rk == 224)
        {
            S[i].Rk = _getch();
        }

        gotoRowCol(rows / 2 + (i * 5), cols / 2 - 40 - i);
        cout << "Enter Left key of snake: " << i;
        S[i].Lk = _getch();
        if (S[i].Lk == 224)
        {
            S[i].Lk = _getch();
        }
        S[i].IsAlive = true;
        S[i].score = 0;
    }

}

void PrintSquare(int rdim, int cdim)
{
    for (int r = 0; r < rdim; r++)
    {
        for (int c = 0; c < cdim; c++)
        {
            if (r == 0 || c == 0 || r == rdim - 1 || c == cdim - 1)
            {
                gotoRowCol(r, c);
                cout << char(-37);
            }
        }
    }
}

void PrintWalls(int p1r, int p1c, int p2r, int p2c, char sym = -37)
{
    for (float a = 0.0011; a < 1; a += 0.01)
    {
        int rows = p1r * (1 - a) + p2r * (a);
        int cols = p1c * (1 - a) + p2c * (a);
        if (p1r == p2r)
        {
            rows = p1r;
        }
        if (p1c == p2c)
        {
            cols = p1c;
        }
        if (rows >= 80 || cols >= 120 || rows < 0 || cols < 0)
        {
            continue;
        }

        gotoRowCol(rows, cols);
        cout << sym;
    }
}

void PrintSnake(Snake*& S, int rows, int cols,int NOP, char sym = -37)
{
    for (int n = 0; n < NOP; n++)
    {
        color(S[n].Color);
        if (S[n].IsAlive==false)
        {
            continue;
        }
        for (int i = 0; i < S[n].size; i++)
        {
            if(i==0)
            {
                color(7);
                gotoRowCol(S[n].Ps[i].r, S[n].Ps[i].c);
                cout << (char)2;
            }
            else
            {
                color(S[n].Color);
                gotoRowCol(S[n].Ps[i].r, S[n].Ps[i].c);
                cout << sym;
            }
        }
    }
}

void EraseSnake(Snake*& S, int rows, int cols,int NOP, char sym = ' ')
{
    for (int n = 0; n < NOP; n++)
    {
        if (S[n].IsAlive == false)
        {
            continue;
        }
        for (int i = 0; i < S[n].size; i++)
        {
            gotoRowCol(S[n].Ps[i].r, S[n].Ps[i].c);
            cout << sym;
        }
    }
}

void DirectionChange(Snake& S, int key)
{
        if (S.Uk==key && S.D != Down)
        {
            S.D = Up;
        }
        else if (S.Dk == key && S.D != Up)
        {
            S.D = Down;
        }
        else if (S.Lk == key && S.D != Right)
        {
            S.D = Left;
        }
        else if (S.Rk == key && S.D != Left)
        {
            S.D = Right;
        }
    
}

void MoveSnake(Snake& S, int rows, int cols)
{
    if (S.IsAlive == false)
    {
        return;
    }
    for (int si = S.size - 1; si - 1 >= 0; si--)
    {
        S.Ps[si] = S.Ps[si - 1];
    }

    switch (S.D)
    {
    case Up:
    {
        S.Ps[0].r--;
        if (S.Ps[0].r == 0)
        {
            S.Ps[0].r = rows - 2;
        }
    }
    break;
    case Down:
    {
        S.Ps[0].r++;
        if (S.Ps[0].r == rows - 1)
        {
            S.Ps[0].r = 1;
        }
    }
    break;
    case Left:
    {
        S.Ps[0].c--;
        if (S.Ps[0].c == 0)
        {
            S.Ps[0].c = cols - 2;
        }
    }
    break;
    case Right:
    {
        S.Ps[0].c++;
        if (S.Ps[0].c == cols - 1)
        {
            S.Ps[0].c = 1;
        }
    }
    break;
    }
}

void MoveSnake_2(Snake& S, int rows, int cols,int& kills)
{
    if (S.IsAlive == false)
    {
        return;
    }
    for (int si = S.size - 1; si - 1 >= 0; si--)
    {
        S.Ps[si] = S.Ps[si - 1];
    }

    switch (S.D)
    {
    case Up:
    {
        S.Ps[0].r--;
        if (S.Ps[0].r == 0)
        {
            S.IsAlive=false;
            kills++;
            return;
        }
    }
    break;
    case Down:
    {
        S.Ps[0].r++;
        if (S.Ps[0].r == rows - 1)
        {
            S.IsAlive = false;
            kills++;
            return;
        }
    }
    break;
    case Left:
    {
        S.Ps[0].c--;
        if (S.Ps[0].c == 0)
        {
            S.IsAlive = false;
            kills++;
            return;
        }
    }
    break;
    case Right:
    {
        S.Ps[0].c++;
        if (S.Ps[0].c == cols - 1)
        {
            S.IsAlive = false;
            kills++;
            return;
        }
    }
    break;
    }
}

bool IsValidFP(Pos& FP, Snake*& S,int NOP)
{
    for(int j=0;j<NOP;j++)
    {
        for (int i = 0; i < S[j].size; i++)
        {
            if (FP.r == S[j].Ps[i].r && FP.c == S[j].Ps[i].c)
            {
                return false;
            }
        }
    }
    return true;
}

void GenerateFood(Pos& FP, Snake*& S, int rows, int cols,int NOP)
{
    srand(time(0));
    do
    {
        FP.r = rand() % (rows - 12) + 2;
        FP.c = rand() % (cols - 12) + 2;
    } while (!IsValidFP(FP, S,NOP));
}

void PrintFood(Pos& FP)
{
    color(7);
    gotoRowCol(FP.r, FP.c);
    cout << char(4);
}


bool FoodEaten(Snake& S, Pos FP)
{
    return(S.Ps[0].r == FP.r && S.Ps[0].c == FP.c);
}

void SnakeGrow(Snake& S)
{
    Pos* P = new Pos[S.size + 1];
    for (int i = 0; i < S.size; i++)
    {
        P[i] = S.Ps[i];
    }
    P[S.size] = P[S.size - 1];
    delete[] S.Ps;
    S.size++;
    S.Ps = P;
}

void PrintScore(Snake& S,int& score, int rows, int cols,int i)
{
    color(S.Color);
    gotoRowCol(rows / 8 +(i*5), (4 * cols) / 4 + 10);
    cout << "SCORE: " << score;
}

bool isKill(Snake*& S,int NOP,int si)
{
        for (int i = 0; i + 1 < S[si].size; i++)
        {
            if (S[si].Ps[0].r == S[si].Ps[i + 1].r && S[si].Ps[0].c == S[si].Ps[i + 1].c)
            {
                return true;
            }
        }

        for (int n = 0; n < NOP; n++)
        {
            if (n == si)
            {
                continue;
            }

            for (int i = 0; i < S[n].size; i++)
            {
                if (S[si].Ps[0].r == S[n].Ps[i].r && S[si].Ps[0].c == S[n].Ps[i].c)
                {
                    return true;
                }
            }
        }

    return false;
}

bool WallHit(Snake& S, int rows, int cols)
{
    //wall 1
    if (S.Ps[0].r >= ROWS / 4 && S.Ps[0].r <= ROWS / 4 + 10 &&
        S.Ps[0].c == COLS / 4)
    {
        return true;
    }

    //wall 2
    if (S.Ps[0].r >= ROWS / 4 && S.Ps[0].r <= ROWS / 4 + 10 &&
        S.Ps[0].c == COLS / 4+25)
    {
        return true;
    }

    //wall 3
    if (S.Ps[0].r >= ROWS / 4+20 && S.Ps[0].r <= ROWS / 4 + 35 &&
        S.Ps[0].c >= COLS / 4+10 && S.Ps[0].c < COLS / 4 + 13)
    {
        return true;
    }

    //wall 4
    if (S.Ps[0].r >= ROWS / 4 + 15 && S.Ps[0].r <= ROWS / 4 + 25 &&
        S.Ps[0].c == COLS / 4 + 65)
    {
        return true;
    }

    //wall 5
    if (S.Ps[0].r >= ROWS / 4 + 35 && S.Ps[0].r <= ROWS / 4 + 45 &&
        S.Ps[0].c >= COLS / 4 + 45 && S.Ps[0].c < COLS / 4 + 48)
    {
        return true;
    }

    return false;
}

// ***************************MAINS****************************
int Stage_1()
{
    system("cls");
    int Kills = 0;
    int NOP;
    int k;
    Snake* S;
    Pos FP;
    //_getch();
    Initialization(S, ROWS, COLS,NOP);
    system("cls");
    PrintSquare(ROWS, COLS);
    for(int i=0;i<NOP;i++)
    {
        PrintScore(S[i], S[i].score, ROWS, COLS, i);
    }
    GenerateFood(FP, S, ROWS, COLS,NOP);
    PrintFood(FP);
    hideConsoleCursor();
    while (true)
    {
            PrintSnake(S, ROWS, COLS,NOP, -37);
        if (_kbhit())
        {
            k = _getch();
            if (k == 224)
            {
                k = _getch();
            }
            for (int i = 0; i < NOP; i++)
            {
                if ((S[i].Uk == k) || (S[i].Dk == k) || (S[i].Lk == k) || (S[i].Rk == k))
                {
                    DirectionChange(S[i], k);
                    break;
                }
            }
        }
            PrintSnake(S, ROWS, COLS, NOP, -37);
            Sleep(25);
            EraseSnake(S, ROWS, COLS,NOP, ' ');
            for (int i = 0; i < NOP; i++)
            {
                MoveSnake(S[i], ROWS, COLS);
            }
            EraseSnake(S, ROWS, COLS, NOP, ' ');
            PrintSnake(S, ROWS, COLS, NOP, -37);

            for(int i=0;i<NOP;i++)
            {
                if (FoodEaten(S[i], FP))
                {
                    cout << "\a";
                    SnakeGrow(S[i]);
                    GenerateFood(FP, S, ROWS, COLS, NOP);
                    PrintFood(FP);
                    PrintScore(S[i], ++S[i].score, ROWS, COLS, i);
                }
            }
            color(7);
            for(int i=0;i<NOP;i++)
            {
                if (isKill(S, NOP,i))
                {
                    gotoRowCol(ROWS / 2+50+(i*5), COLS / 2);
                    cout << "Snake of Player : " << i + 1 << " Died" << endl;
                    Kills++;
                    EraseSnake(S, ROWS, COLS, NOP, ' ');
                    S[i].IsAlive = false;
                    continue;
                }
            }
            if (Kills == NOP)
            {
                break;
            }
    }
    system("cls");
    gotoRowCol(ROWS / 2, COLS / 2);
    Beep(500, 1000);
    cout << "GAME OVER...";
    for(int i=0;i<NOP;i++)
    {
        gotoRowCol(ROWS / 2+10+(i*2), COLS / 2);
        cout << "SCORE OF PLAYER: " << i + 1 <<" = " << S[i].score;
    }
    gotoRowCol(ROWS, COLS);
    return 0;
}

int Stage_2()
{
    system("cls");
    fstream rdr("input.txt");
    int Rows, Cols;
    int Kills = 0;
    int NOP;
    int k;
    Snake* S;
    Pos FP;
    //_getch();
    Initialization_2(rdr,S, Rows, Cols, NOP);
    system("cls");
    PrintSquare(Rows, Cols);
    for (int i = 0; i < NOP; i++)
    {
        PrintScore(S[i], S[i].score, Rows, Cols, i);
    }
    GenerateFood(FP, S, Rows, Cols, NOP);
    PrintFood(FP);
    hideConsoleCursor();
    while (true)
    {
        PrintSnake(S, Rows, Cols, NOP, -37);
        if (_kbhit())
        {
            k = _getch();
            if (k == 224)
            {
                k = _getch();
            }
            for (int i = 0; i < NOP; i++)
            {
                if ((S[i].Uk == k) || (S[i].Dk == k) || (S[i].Lk == k) || (S[i].Rk == k))
                {
                    DirectionChange(S[i], k);
                    break;
                }
            }
        }
        PrintSnake(S, Rows, Cols, NOP, -37);
        Sleep(25);
        EraseSnake(S, Rows, Cols, NOP, ' ');
        for (int i = 0; i < NOP; i++)
        {
            MoveSnake_2(S[i], Rows, Cols,Kills);
        }
        EraseSnake(S, Rows, Cols, NOP, ' ');
        PrintSnake(S, Rows, Cols, NOP, -37);

        for (int i = 0; i < NOP; i++)
        {
            if (S[i].IsAlive == false)
            {
                continue;
            }
            if (FoodEaten(S[i], FP))
            {
                cout << "\a";
                SnakeGrow(S[i]);
                GenerateFood(FP, S, Rows, Cols, NOP);
                PrintFood(FP);
                PrintScore(S[i], ++S[i].score, Rows, Cols, i);
            }
        }
        color(7);
        for (int i = 0; i < NOP; i++)
        {
            if (S[i].IsAlive == false)
            {
                gotoRowCol(Rows / 2 + 50 + (i * 5), Cols / 2);
                cout << "Snake of Player : " << i + 1 << " Died" << endl;
                continue;
            }
            if (isKill(S, NOP, i))
            {
                gotoRowCol(Rows / 2 + 50 + (i * 5), Cols / 2);
                cout << "Snake of Player : " << i + 1 << " Died" << endl;
                Kills++;
                EraseSnake(S, Rows, Cols, NOP, ' ');
                S[i].IsAlive = false;
                continue;
            }
        }
        if (Kills == NOP)
        {
            break;
        }
    }
    system("cls");
    gotoRowCol(ROWS / 2, COLS / 2);
    Beep(500, 1000);
    cout << "GAME OVER...";
    for (int i = 0; i < NOP; i++)
    {
        gotoRowCol(ROWS / 2 + 10 + (i * 2), COLS / 2);
        cout << "SCORE OF PLAYER: " << i + 1 << " = " << S[i].score;
    }
    gotoRowCol(ROWS, COLS);
    return 0;
}

int Stage_3()
{
    system("cls");
    int Kills = 0;
    int NOP;
    int k;
    Snake* S;
    Pos FP;
    //_getch();
    Initialization(S, ROWS, COLS, NOP);
    system("cls");
    PrintSquare(ROWS, COLS);


    PrintWalls(ROWS / 4, COLS / 4, ROWS / 4 + 10, COLS / 4);
    PrintWalls(ROWS / 4, COLS / 4+25, ROWS / 4 + 10, COLS / 4+25);
    PrintWalls(ROWS / 4 + 20, COLS / 4 + 15, ROWS / 4 + 35, COLS / 4 + 10);

    PrintWalls(ROWS / 4+15, COLS / 4+65, ROWS / 4 + 25, COLS / 4+65);
    PrintWalls(ROWS / 4+35, COLS / 4 + 50, ROWS / 4 + 45, COLS / 4 + 45);


    for (int i = 0; i < NOP; i++)
    {
        PrintScore(S[i], S[i].score, ROWS, COLS, i);
    }
    GenerateFood(FP, S, ROWS, COLS, NOP);
    PrintFood(FP);
    hideConsoleCursor();
    while (true)
    {
        PrintSnake(S, ROWS, COLS, NOP, -37);
        if (_kbhit())
        {
            k = _getch();
            if (k == 224)
            {
                k = _getch();
            }
            for (int i = 0; i < NOP; i++)
            {
                if ((S[i].Uk == k) || (S[i].Dk == k) || (S[i].Lk == k) || (S[i].Rk == k))
                {
                    DirectionChange(S[i], k);
                    break;
                }
            }
        }
        PrintSnake(S, ROWS, COLS, NOP, -37);
        Sleep(25);
        EraseSnake(S, ROWS, COLS, NOP, ' ');
        for (int i = 0; i < NOP; i++)
        {
            MoveSnake(S[i], ROWS, COLS);
        }
        EraseSnake(S, ROWS, COLS, NOP, ' ');
        PrintSnake(S, ROWS, COLS, NOP, -37);

        for (int i = 0; i < NOP; i++)
        {
            if (FoodEaten(S[i], FP))
            {
                cout << "\a";
                SnakeGrow(S[i]);
                GenerateFood(FP, S, ROWS, COLS, NOP);
                PrintFood(FP);
                PrintScore(S[i], ++S[i].score, ROWS, COLS, i);
            }
        }
        color(7);
        for (int i = 0; i < NOP; i++)
        {
            if (isKill(S, NOP, i) || WallHit(S[i],ROWS,COLS))
            {
                gotoRowCol(ROWS / 2 + 50 + (i * 5), COLS / 2);
                cout << "Snake of Player : " << i + 1 << " Died" << endl;
                Kills++;
                EraseSnake(S, ROWS, COLS, NOP, ' ');
                S[i].IsAlive = false;
                continue;
            }
        }
        if (Kills == NOP)
        {
            break;
        }
    }
    system("cls");
    gotoRowCol(ROWS / 2, COLS / 2);
    Beep(500, 1000);
    cout << "GAME OVER...";
    for (int i = 0; i < NOP; i++)
    {
        gotoRowCol(ROWS / 2 + 10 + (i * 2), COLS / 2);
        cout << "SCORE OF PLAYER: " << i + 1 << " = " << S[i].score;
    }
    gotoRowCol(ROWS, COLS);
    return 0;
}

int Stage_4()
{
    int count = 0;
    int foodDur = 100;
    bool FoodPresent = false;
    system("cls");
    int Kills = 0;
    int NOP;
    int k;
    Snake* S;
    Pos FP;
    //_getch();
    Initialization(S, ROWS, COLS, NOP);
    system("cls");
    PrintSquare(ROWS, COLS);
    for (int i = 0; i < NOP; i++)
    {
        PrintScore(S[i], S[i].score, ROWS, COLS, i);
    }
    hideConsoleCursor();
    while (true)
    {
        count++;
        PrintSnake(S, ROWS, COLS, NOP, -37);
        if (_kbhit())
        {
            k = _getch();
            if (k == 224)
            {
                k = _getch();
            }
            for (int i = 0; i < NOP; i++)
            {
                if ((S[i].Uk == k) || (S[i].Dk == k) || (S[i].Lk == k) || (S[i].Rk == k))
                {
                    DirectionChange(S[i], k);
                    break;
                }
            }
        }
        PrintSnake(S, ROWS, COLS, NOP, -37);
        Sleep(25);
        EraseSnake(S, ROWS, COLS, NOP, ' ');
        for (int i = 0; i < NOP; i++)
        {
            MoveSnake(S[i], ROWS, COLS);
        }
        EraseSnake(S, ROWS, COLS, NOP, ' ');
        PrintSnake(S, ROWS, COLS, NOP, -37);

        if (FoodPresent)
        {
            if (count % foodDur == 0)
            {
                gotoRowCol(FP.r, FP.c);
                    cout << " ";
                    FoodPresent = false;
            }
        }
        else
        {
            GenerateFood(FP, S, ROWS, COLS, NOP);
            PrintFood(FP);
            FoodPresent = true;
        }


        for (int i = 0; i < NOP; i++)
        {
            if (FoodEaten(S[i], FP))
            {
                cout << "\a";
                SnakeGrow(S[i]);
                PrintScore(S[i], ++S[i].score, ROWS, COLS, i);
                FoodPresent = false;
            }
        }
        color(7);
        for (int i = 0; i < NOP; i++)
        {
            if (isKill(S, NOP, i))
            {
                gotoRowCol(ROWS / 2 + 50 + (i * 5), COLS / 2);
                cout << "Snake of Player : " << i + 1 << " Died" << endl;
                Kills++;
                EraseSnake(S, ROWS, COLS, NOP, ' ');
                S[i].IsAlive = false;
                continue;
            }
        }
        if (Kills == NOP)
        {
            break;
        }
    }
    system("cls");
    gotoRowCol(ROWS / 2, COLS / 2);
    Beep(500, 1000);
    cout << "GAME OVER...";
    for (int i = 0; i < NOP; i++)
    {
        gotoRowCol(ROWS / 2 + 10 + (i * 2), COLS / 2);
        cout << "SCORE OF PLAYER: " << i + 1 << " = " << S[i].score;
    }
    gotoRowCol(ROWS, COLS);
    return 0;
}


void menu()
{
    cout << "\n\n1. Stage 1 Wrap-Around Mode";
    cout << "\n2. Stage 2 Bounded Box Mode\n";
    cout << "\n3. Stage 3 Wall Mode";
    cout << "\n4. Stage 4 Timed Food Mode";
    cout << "\n0. quit\n";

}

int main()
{
    int choice = 1;
    _getch();
    cout << "\nWhich Stage?  ";
    while (choice != 0)
    {
        menu();
        cin >> choice;
        cout << "\n";

        switch (choice)
        {
        case 1:
            Stage_1();
            break;
        case 2:
            Stage_2();
            break;
        case 3:
            Stage_3();
            break;
        case 4:
            Stage_4();
            break;
        case 0:
            cout << "Thank You";
            break;
        default:
            cout << "Wrong Input\n";
            break;

        }
        cout << endl;
        system("pause");
        system("cls");
    }
    return 0;
}