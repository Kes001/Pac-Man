#include "TXLib.h"

int main()
    {
    txCreateWindow (960, 600);

    HDC fon  = txLoadImage ("images\\map.bmp");


    HDC pacman  = txLoadImage ("images\\sprite.bmp");

    int sizeX = txGetExtentX (pacman)/16,
        sizeY = txGetExtentY (pacman)/16;

    int t = 1;
    int nanim = 0;

    while (true)
        {
        txBitBlt  (txDC(), 0, 0, 0, 0, fon, 0, 0);
        txTransparentBlt (txDC(), 0, 0, sizeX, sizeY, pacman, nanim*sizeX, (t%16)*sizeY);


        if (txGetAsyncKeyState (VK_F1)) nanim = 0;
        if (txGetAsyncKeyState (VK_F2))   nanim = 1;
        if (txGetAsyncKeyState (VK_F3))   nanim = 2;
        if (txGetAsyncKeyState (VK_F4))   nanim = 3;
        if (txGetAsyncKeyState (VK_F5))   nanim = 4;
        if (txGetAsyncKeyState (VK_F6))   nanim = 5;
        if (txGetAsyncKeyState (VK_F7))   nanim = 6;
        if (txGetAsyncKeyState (VK_F8))   nanim = 7;
        if (txGetAsyncKeyState (VK_F9))   nanim = 8;
        if (txGetAsyncKeyState (VK_F10))   nanim = 9;
        t++;

        txSleep(100);
        }
    txDeleteDC (fon);
    txDeleteDC (pacman);

    return 0;
    }
