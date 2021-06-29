#include "TXLib.h"

struct Dot
    {
    int x, y;
    int r;
    bool visible;

    COLORREF color;
    COLORREF fillcolor;

    void Draw ();
    };

struct PacMan
    {
    int x, y;
    int r;

    COLORREF color;
    COLORREF fillcolor;

    int lives;
    int direction;

    void Draw (int time);
    void Move (HDC fon);
    void LivesDraw ();
    };

struct Enemy
    {
    int x, y;
    int r;
    int direction;

    void Draw (HDC enemy);
    void Move (HDC fon, int time);
    };

struct ControlType
    {
    int key_left, key_right, key_up, key_down;
    };

void StartGame ();
void Game ();
void GameOver ();
void Win ();

void Collision     (struct PacMan* pacman, struct Enemy* enemy);
void EatDots       (struct PacMan pacman, struct Dot* dot, int* score);
void ControlPacMan (struct PacMan* pacman, struct ControlType player);
void ScoreDraw     (int score);
void LivesDraw     ();

double DotDistance   (struct PacMan pacman, struct Dot dot);
double EnemyDistance (struct PacMan pacman, struct Dot dot);

int main()
    {
    txCreateWindow (960, 600);

    StartGame ();
    Game ();

    return 0;
    }

//-------------------------------------------------------------

void StartGame ()
    {
    HDC start_screen  = txLoadImage ("images\\start.bmp");

    txPlaySound ("sounds/start.wav");

    for (int x = -700; x <= 0; x += 2)
        {
        txBitBlt  (txDC(), x, 0, 960, 600, start_screen, 0, 0);
        txSleep();
        }
    txDeleteDC (start_screen);
    }

//-------------------------------------------------------------

void Game ()
    {
    HDC fon  = txLoadImage ("images\\map.bmp");
    HDC enemy1_look = txLoadImage ("images\\enemy1.bmp");
    HDC enemy2_look = txLoadImage ("images\\enemy2.bmp");
    HDC enemy3_look = txLoadImage ("images\\enemy3.bmp");
    HDC enemy4_look = txLoadImage ("images\\enemy4.bmp");

    PacMan pacman = { 250, 300, 20, TX_YELLOW, TX_YELLOW, 3, 1 };
    Enemy  enemy1 = { 480, 300, 20, 1 };
    Enemy  enemy2 = { 480, 300, 20, 3 };
    Enemy  enemy3 = { 480, 300, 20, 3 };
    Enemy  enemy4 = { 480, 300, 20, 3 };

    // array of dots
    Dot Dot1 = {  50, 100, 10, true, TX_LIGHTRED, TX_RED };
    Dot Dot2 = { 100, 150, 10, true, TX_LIGHTRED, TX_RED };
    Dot Dot3 = { 200, 300, 10, true, TX_LIGHTRED, TX_RED };

    ControlType player = {VK_RIGHT, VK_LEFT, VK_UP, VK_DOWN};

    int score = 0;
    int time  = 0;
    bool win = false;

    while (!txGetAsyncKeyState (VK_ESCAPE) && pacman.lives && !win)
        {
        if (pacman.direction)
            {
            time++;

            txBegin ();
            txBitBlt  (txDC(), 0, 0, 960, 600, fon, 0, 0);

            ScoreDraw (score);
            pacman.LivesDraw ();

            Dot1.Draw ();//loop for all dots
            Dot2.Draw ();
            Dot3.Draw ();

            pacman.Draw (time);
            enemy1.Draw (enemy1_look);//loop for all enemies
            enemy2.Draw (enemy2_look);
            enemy3.Draw (enemy3_look);
            enemy4.Draw (enemy4_look);

            Collision (&pacman, &enemy1);//loop for all enemies
            Collision (&pacman, &enemy2);
            Collision (&pacman, &enemy3);
            Collision (&pacman, &enemy4);

            ControlPacMan (&pacman, player);

            pacman.Move (fon);
            enemy1.Move (fon, time);
            enemy2.Move (fon, time);
            enemy3.Move (fon, time);
            enemy4.Move (fon, time);

            //loop for all dots EatDots (struct PacMan pacman, struct Dot* dot)
            //need array of dots

            EatDots (pacman, &Dot1, &score);
            EatDots (pacman, &Dot2, &score);
            EatDots (pacman, &Dot3, &score);

            txEnd ();

            if (time > 500) time = 0;
            if (score == 30) win = true;
            }
        ControlPacMan (&pacman, player);
        }
    txDeleteDC (fon);
    txDeleteDC (enemy1_look);
    txDeleteDC (enemy2_look);
    txDeleteDC (enemy3_look);
    txDeleteDC (enemy4_look);
    if (win) Win();
    else     GameOver();
    }

//------------------------------------------------- pacman -> direction = 4------------

double DotDistance (struct PacMan pacman, struct Dot dot)
    {
    return sqrt ((pacman.x - dot.x)*(pacman.x - dot.x) + (pacman.y - dot.y)*(pacman.y - dot.y));
    }

//-------------------------------------------------------------

void PacMan::Draw (int time)
    {
    txSetColor (color);
    txSetFillColor (fillcolor);

    txCircle (x, y, r);
    time ++;
    if (time%10 < 5)
        {
        if (direction == 1)
            {
            txSetFillColor (TX_BLACK);
            txSetColor (TX_BLACK);
            POINT mouth[] = {{x, y}, {x - r,  y - r}, {x - r,  y + r},{x, y}};
            txPolygon (mouth, 4);
            }
        if (direction == 2)
            {
            txSetFillColor (TX_BLACK);
            txSetColor (TX_BLACK);
            POINT mouth[] = {{x, y}, {x + r,  y + r}, {x + r,  y - r},{x, y}};
            txPolygon (mouth, 4);
            }
        if (direction == 3)
            {
            txSetFillColor (TX_BLACK);
            txSetColor (TX_BLACK);
            POINT mouth[] = {{x, y}, {x - r,  y - r}, {x + r,  y - r},{x, y}};
            txPolygon (mouth, 4);
            }
        if (direction == 4)
            {
            txSetFillColor (TX_BLACK);
            txSetColor (TX_BLACK);
            POINT mouth[] = {{x, y}, {x - r,  y + r}, {x + r,  y + r},{x, y}};
            txPolygon (mouth, 4);
            }
        }
    }

//-------------------------------------------------------------

void PacMan::Move (HDC fon)
    {
    int oldx = x;
    int oldy = y;

    if (direction == 1) x -= 1;
    if (direction == 2) x += 1;
    if (direction == 3) y -= 1;
    if (direction == 4) y += 1;

    bool wall = false; //find a wall

    for (int i = x - r; i <= x + r; i += 5)
        {
        for (int j = y - r; j <= y + r; j += 5)
            {
            if (txGetPixel (i, j, fon) != RGB (0, 0, 0))
                {
                wall = true;
                break;
                }
            }
        }

    if (wall)
        {
        x = oldx;
        y = oldy;
        }
    if (x - r <= 2) x = 958 - r;
    if (x + r >= 959) x = 3 + r;
    }

//-------------------------------------------------------------

void Enemy::Draw (HDC enemy)
    {
    txBitBlt  (txDC(), x - r, y - r, 2*r, 2*r, enemy, 0, 0);
    }

//-------------------------------------------------------------

void Enemy::Move (HDC fon, int time)
    {
    int oldx = x;
    int oldy = y;

    if (direction == 1) x -= 1;
    if (direction == 2) x += 1;
    if (direction == 3) y -= 1;
    if (direction == 4) y += 1;

    bool wall = false; //find a wall

    for (int dx = x - r; dx <= x + r; dx += 5)
        {
        for (int dy = y - r; dy <= y + r; dy += 5)
            {
            if (txGetPixel (dx, dy, fon) != RGB (0, 0, 0))
                {
                wall = true;
                break;
                }
            }
        }

    if (wall)
        {
        x = oldx;
        y = oldy;
        direction = rand() % 4 + 1;
        }

    if (x <= 2) x = 958;
    if (x >= 959) x = 3;

    if (time == 500) direction = rand() % 4 + 1;
    }

//-------------------------------------------------------------

void ControlPacMan (struct PacMan* pacman, struct ControlType player)
    {
    if (txGetAsyncKeyState (player.key_right)) pacman -> direction = 1;
    if (txGetAsyncKeyState (player.key_left))  pacman -> direction = 2;
    if (txGetAsyncKeyState (player.key_up))    pacman -> direction = 3;
    if (txGetAsyncKeyState (player.key_down))  pacman -> direction = 4;

    if (txGetAsyncKeyState (VK_SPACE)) pacman -> direction = 0;
    }

//-------------------------------------------------------------

void Dot::Draw ()
    {
    txBegin ();
    txSetColor (color, 2);
    txSetFillColor (fillcolor);

    if (visible) txCircle (x, y, r);
    txEnd ();
    }

//-------------------------------------------------------------

void ScoreDraw (int score)
    {
    txBegin ();
    txSetColor (TX_GREEN);
    txSelectFont ("TimesNewRoman", 40);
    char str[15] = "";
    sprintf (str, "SCORE: %d", score);

    int centerX = txGetExtentX()/2;

    int textSizeX = txGetTextExtentX ("---------------"),
        textSizeY = txGetTextExtentY ("---------------");

    txSetFillColor (TX_BLACK);
    txRectangle (centerX - textSizeX, 0,
                 centerX + textSizeX, textSizeY);
    txTextOut   (centerX - textSizeX/2, 0, str);
    txEnd ();
    }

//-------------------------------------------------------------

void Collision (struct PacMan* pacman, struct Enemy* enemy)
    {
    bool touch = false;

    double d = sqrt((pacman -> x - enemy -> x)*(pacman -> x - enemy -> x) +
                    (pacman -> y - enemy -> y)*(pacman -> y - enemy -> y));

    if (d < pacman -> r + enemy -> r)
        {
        pacman -> lives = pacman -> lives - 1;
        touch = true;
        }

    if (touch)
        {
        txPlaySound ("sounds/die.wav");

        enemy -> x = 480;
        enemy -> y = 300;

        pacman -> x = 250;
        pacman -> y = 300;

        txSleep(20);
        }


    }

//-------------------------------------------------------------

void EatDots (struct PacMan pacman, struct Dot* dot, int* score)
    {
    if (DotDistance (pacman, *dot) <= pacman.r + dot -> r && dot -> visible)
            {
            dot -> visible = false;
            *score += 10;
            txPlaySound ("sounds/move.wav");
            }
    }

//-------------------------------------------------------------

void PacMan::LivesDraw ()
    {
    for (int live = 1; live <= lives; live++)
        {
        int r_lives = 10;
        int dist = 50;

        txSetColor (TX_YELLOW);
        txSetFillColor (TX_YELLOW);

        txCircle (20 + dist*live, 580, r_lives);
        }
    }

//-------------------------------------------------------------

void GameOver ()
    {
    txSetColor (TX_RED);

    txSelectFont ("TimesNewRoman", 150);
    txTextOut (300, 200, "GAME");
    txTextOut (300, 300, "OVER");

    }

//-------------------------------------------------------------

void Win ()
    {
    txSetColor (TX_RED);

    txSelectFont ("TimesNewRoman", 200);
    txTextOut (300, 200, "WIN");
    txSleep(10000);
    }
