#include "TXLib.h"

void Game  (HDC fon);

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

    void Draw ();
    void Move (HDC fon);
    };

struct Enemy
    {
    int x, y;
    int direction;

    void Draw ();
    void Move ();
    };

struct ControlType
    {
    int key_left, key_right, key_up, key_down;
    };

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

    HDC fon  = txLoadImage ("images\\map.bmp");

    Game (fon);

    return 0;
    }

//-------------------------------------------------------------

void Game (HDC fon)
    {
    PacMan pacman = { 300, 300, 20, TX_YELLOW, TX_YELLOW, 1 };

    Dot Dot1 = {  50, 100, 10, true, TX_LIGHTRED, TX_RED };
    Dot Dot2 = { 100, 150, 10, true, TX_LIGHTRED, TX_RED };
    Dot Dot3 = { 200, 300, 10, true, TX_LIGHTRED, TX_RED };

    ControlType player = {VK_RIGHT, VK_LEFT, VK_UP, VK_DOWN};

    int lives = 3;
    //double ax = 0.0, ay = 0.0;
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

        ControlPacMan (&pacman, player);

        pacman.Move (fon);

        //цикл для всех точек EatDots (struct PacMan* pacman, struct Dot* dot)
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

    if (direction == 1) x -= 2;
    if (direction == 2) x += 2;
    if (direction == 3) y -= 2;
    if (direction == 4) y += 2;

    bool wall = false; //не находить на стенки
    COLORREF color2 = txGetPixel (x, y, fon);

    for (int i = x - r; i <= x + r; i++)
        {
        for (int j = y - r; j <= y + r; j++)
            {
            color2 = txGetPixel (i, j, fon);
            if (color2 != RGB (0, 0, 0))
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
    }

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

void PacMan::Draw ()
    {
    txSetColor (color, 2);
    txSetFillColor (fillcolor);

    txCircle (x, y, r);
    txSleep(20);

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
    txSleep(10);

    }

//-------------------------------------------------------------

void ScoreDraw (int score)
    {
    txBegin ();
    txSetColor (TX_GREEN);
    txSelectFont ("TimesNewRoman", 40);
    char str[12] = "";
    sprintf (str, "SCORE : %d", score);

    int centerX = txGetExtentX()/2;

    int textSizeX = txGetTextExtentX ("------------"),
        textSizeY = txGetTextExtentY ("------------");

    txSetFillColor (TX_BLACK);
    txRectangle (centerX - textSizeX, 0,
                 centerX + textSizeX, textSizeY);
    txTextOut   (centerX - textSizeX/2, 0, str);
    txEnd ();
    }

//-------------------------------------------------------------

void Collision (struct PacMan* pacman, struct Enemy* enemy)
    {
    /*ball1 -> vx = -ball1 -> vx;
    ball1 -> vy = -ball1 -> vy;
    ball2 -> vx = -ball2 -> vx;
    ball2 -> vy = -ball2 -> vy;


    double Dx = ball1 -> x - ball2 -> x;
    double Dy = ball1 -> y - ball2 -> y;
    double d = Distance (*ball1, *ball2);
    double s = Dx/d; // sin
    double e = Dy/d; // cos

    double Vn1 = (ball2 -> vx)*dt*s + (ball2 -> vy)*dt*e;
    double Vn2 = (ball1 -> vx)*dt*s + (ball1 -> vy)*dt*e;

    double inside = (ball1 -> r + ball2 -> x - d) / (Vn1 - Vn2);
    if (inside >  0.6) inside =  0.6;
    if (inside < -0.6) inside = -0.6;

    ball1 -> x -= ROUND((ball1 -> vx)*dt*inside);
    ball1 -> y -= ROUND((ball1 -> vy)*dt*inside);
    ball2 -> x -= ROUND((ball2 -> vx)*dt*inside);
    ball2 -> y -= ROUND((ball2 -> vy)*dt*inside);

    Dx = ball1 -> x - ball2 -> x;
    Dy = ball1 -> y - ball2 -> y;
    d = Distance (*ball1, *ball2);
    s = Dx/d;
    e = Dy/d;

    Vn1 = (ball2 -> vx)*dt*s + (ball2 -> vy)*dt*e;
    Vn2 = (ball1 -> vx)*dt*s + (ball1 -> vy)*dt*e;

    double Vt1 = -(ball2 -> vx)*dt*e + (ball2 -> vy)*dt*s;
    double Vt2 = -(ball1 -> vx)*dt*e + (ball1 -> vy)*dt*s;

    double o = Vn2;
    Vn2 = Vn1;
    Vn1 = o;

    ball1 -> vx = ROUND(Vn2*s - Vt2*e);
    ball1 -> vy = ROUND(Vn2*e + Vt2*s);
    ball2 -> vx = ROUND(Vn1*s - Vt1*e);
    ball2 -> vy = ROUND(Vn1*e + Vt1*s);

    ball1 -> x += ROUND((ball1 -> vx)*dt*inside);
    ball1 -> y += ROUND((ball1 -> vy)*dt*inside);
    ball2 -> x += ROUND((ball2 -> vx)*dt*inside);
    ball2 -> y += ROUND((ball2 -> vy)*dt*inside);
    */

    txPlaySound ("sounds/ball_sound.wav");
    }

//-------------------------------------------------------------

void EatDots       (struct PacMan* pacman, struct Dot* dot)
    {
    }

//-------------------------------------------------------------

void LivesDraw (int lives)
    {

    }
