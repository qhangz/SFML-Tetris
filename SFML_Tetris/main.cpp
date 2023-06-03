#include"TetrisGame.h"
#include"Tetris.h"

int main()
{
    Game Tetris;
    while(Tetris.window.isOpen())
    {
        Tetris.gameRun();
    }

    return 0;
}