#include "TXLib.h"

const HDC sprites  = txLoadImage ("images\\sprite.bmp");
const int SIZEX = txGetExtentX (sprites)/16,
          SIZEY = txGetExtentY (sprites)/16;

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

    int lives;
    int direction;

    void Draw (HDC pacman, int time);
    void Move (HDC fon);
    void LivesDraw ();
    };

struct Enemy
    {
    int x, y;
    int r;
    int direction;

    void Draw (HDC enemy, int time, int number);
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

    //StartGame ();
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

    PacMan pacman = { 250, 300, 20, 3, 1 };

    Enemy  enemy1 = { 480, 300, 20, 3 };
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

            pacman.Draw (sprites, time);

            enemy1.Draw (sprites, time, 12);//loop for all enemies
            enemy2.Draw (sprites, time, 13);
            enemy3.Draw (sprites, time, 14);
            enemy4.Draw (sprites, time, 15);

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
            txSleep(5);

            if (time > 300) time = 0;
            if (score == 30) win = true;
            }
        ControlPacMan (&pacman, player);
        }
    txDeleteDC (fon);
    txDeleteDC (sprites);
    if (win) Win();
    else     GameOver();
    }

//-------------------------------------------------------------

double DotDistance (struct PacMan pacman, struct Dot dot)
    {
    return sqrt ((pacman.x - dot.x)*(pacman.x - dot.x) + (pacman.y - dot.y)*(pacman.y - dot.y));
    }

//-------------------------------------------------------------

void PacMan::Draw (HDC pacman, int time)
    {
    if (direction == 1)
        {
        txTransparentBlt (txDC(), x - r, y - r, SIZEX, SIZEY, pacman, 9*SIZEX, (time%5)*SIZEY);
        }
    if (direction == 2)
        {
        txTransparentBlt (txDC(), x - r, y - r, SIZEX, SIZEY, pacman, 10*SIZEX, (time%5)*SIZEY);
        }
    if (direction == 3)
        {
        txTransparentBlt (txDC(), x - r, y - r, SIZEX, SIZEY, pacman, 9*SIZEX, (time%5 + 4)*SIZEY);
        }
    if (direction == 4)
        {
        txTransparentBlt (txDC(), x - r, y - r, SIZEX, SIZEY, pacman, 10*SIZEX, (time%5 + 4)*SIZEY);
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

void Enemy::Draw (HDC enemy, int time, int number)
    {
    if (direction == 1)
        {
        txTransparentBlt (txDC(), x - r, y - r, SIZEX, SIZEY, enemy, number*SIZEX, (time%2 + 4)*SIZEY);
        }
    if (direction == 2)
        {
        txTransparentBlt (txDC(), x - r, y - r, SIZEX, SIZEY, enemy, number*SIZEX, (time%2)*SIZEY);
        }
    if (direction == 3)
        {
        txTransparentBlt (txDC(), x - r, y - r, SIZEX, SIZEY, enemy, number*SIZEX, (time%2 + 6)*SIZEY);
        }
    if (direction == 4)
        {
        txTransparentBlt (txDC(), x - r, y - r, SIZEX, SIZEY, enemy, number*SIZEX, (time%2 + 2)*SIZEY);
        }
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

    if (time == 300) direction = rand() % 4 + 1;
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

        for (int cadr = 0; cadr <= 13; cadr++)
            {
            txBitBlt (txDC(), pacman -> x - pacman ->r, pacman -> y - pacman -> r,
                               SIZEX, SIZEY, sprites, 8*SIZEX, cadr*SIZEY);
            txSleep(50);
            }

        enemy -> x = 480;
        enemy -> y = 300;
        enemy -> direction = 3;

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
