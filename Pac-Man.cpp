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

    int direction;
    int mouth_open;

    void Draw ();
    void Move (HDC fon);
    };

struct Enemy
    {
    int x, y;
    int direction;

    void Draw (HDC enemy);
    void Move (HDC fon);
    };

struct ControlType
    {
    int key_left, key_right, key_up, key_down;
    };

void Game  ();
void Collision     (struct PacMan* pacman, struct Enemy* enemy);
void EatDots       (struct PacMan* pacman, struct Dot* dot);
void ControlPacMan (struct PacMan* pacman, struct ControlType player);
void ScoreDraw     (int score);
void LivesDraw     (int lives);

double DotDistance   (struct PacMan pacman, struct Dot dot);
double EnemyDistance (struct PacMan pacman, struct Dot dot);

int main()
    {
    txCreateWindow (960, 600);

    Game ();

    return 0;
    }

//-------------------------------------------------------------

void Game ()
    {
    HDC fon  = txLoadImage ("images\\map.bmp");
    HDC enemy1_look = txLoadImage ("images\\enemy.bmp");

    PacMan pacman = { 250, 300, 20, TX_YELLOW, TX_YELLOW, 1, 0 };
    Enemy  enemy1 = { 460, 200, 1 };

    // array of dots
    Dot Dot1 = {  50, 100, 10, true, TX_LIGHTRED, TX_RED };
    Dot Dot2 = { 100, 150, 10, true, TX_LIGHTRED, TX_RED };
    Dot Dot3 = { 200, 300, 10, true, TX_LIGHTRED, TX_RED };

    ControlType player = {VK_RIGHT, VK_LEFT, VK_UP, VK_DOWN};

    int lives = 3;
    int score = 0;

    while (!txGetAsyncKeyState (VK_ESCAPE))
        {
        txBegin ();
        txBitBlt  (txDC(), 0, 0, 960, 600, fon, 0, 0);

        ScoreDraw (score);
        LivesDraw (lives);

        Dot1.Draw ();
        Dot2.Draw ();
        Dot3.Draw ();

        pacman.Draw ();
        enemy1.Draw (enemy1_look);

        ControlPacMan (&pacman, player);

        pacman.Move (fon);
        enemy1.Move (fon);

        //loop for all dots EatDots (struct PacMan* pacman, struct Dot* dot)
        //need array of dots

        if (DotDistance (pacman, Dot1) <= pacman.r + Dot1.r && Dot1.visible)
            {
            Dot1.visible = false;
            score += 10;
            }
        if (DotDistance (pacman, Dot2) <= pacman.r + Dot2.r && Dot2.visible)
            {
            Dot2.visible = false;
            score += 10;
            }
        if (DotDistance (pacman, Dot3) <= pacman.r + Dot2.r && Dot3.visible)
            {
            Dot3.visible = false;
            score += 10;
            }

        txEnd ();
        txSleep ();
        }

    txDeleteDC (fon);
    txDeleteDC (enemy1_look);
    }

//-------------------------------------------------------------

double DotDistance (struct PacMan pacman, struct Dot dot)
    {
    return sqrt ((pacman.x - dot.x)*(pacman.x - dot.x) + (pacman.y - dot.y)*(pacman.y - dot.y));
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

void PacMan::Draw ()
    {
    txSetColor (color);
    txSetFillColor (fillcolor);

    txCircle (x, y, r);
    mouth_open ++;
    if (mouth_open < 5)
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
    if (mouth_open > 10) mouth_open = 0;
    }

//-------------------------------------------------------------

void Enemy::Draw (HDC enemy)
    {
    txBitBlt  (txDC(), x, y, 40, 40, enemy, 0, 0);
    }

void Enemy::Move (HDC fon)
    {
    int oldx = x;
    int oldy = y;

    if (direction == 1) x -= 1;
    if (direction == 2) x += 1;
    if (direction == 3) y -= 1;
    if (direction == 4) y += 1;

    bool wall = false; //find a wall

    if (txGetPixel (x, y, fon) != RGB (0, 0, 0)) wall = true;
    if (txGetPixel (x + 40, y, fon) != RGB (0, 0, 0)) wall = true;
    if (txGetPixel (x, y + 40, fon) != RGB (0, 0, 0)) wall = true;

    if (wall)
        {
        x = oldx;
        y = oldy;
        direction = rand() % 4 + 1;
        }
    if (x <= 2) x = 958;
    if (x >= 959) x = 3;
    }

//-------------------------------------------------------------
//-------------------------------------------------------------

void ControlPacMan (struct PacMan* pacman, struct ControlType player)
    {
    if (txGetAsyncKeyState (player.key_right)) (*pacman).direction = 1;
    if (txGetAsyncKeyState (player.key_left))  (*pacman).direction = 2;
    if (txGetAsyncKeyState (player.key_up))    (*pacman).direction = 3;
    if (txGetAsyncKeyState (player.key_down))  (*pacman).direction = 4;
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
    //txPlaySound ("sounds/.wav");
    }

//-------------------------------------------------------------

void EatDots (struct PacMan* pacman, struct Dot* dot)
    {
    }

//-------------------------------------------------------------

void LivesDraw (int lives)
    {
    for (int live = 1; live <= lives; live++)
        {
        int r = 10;
        int dist = 50;

        txSetColor (TX_YELLOW);
        txSetFillColor (TX_YELLOW);

        txCircle (20 + dist*live, 580, r);
        }
    }
