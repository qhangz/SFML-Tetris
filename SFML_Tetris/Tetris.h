#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <windows.h>
#include <iostream>
#include <sstream>


#define GRIDSIZE 35
#define STAGE_WIDTH 10
#define STAGE_HEIGHT 20
#define P1_STAGE_CORNER_X 156
#define P1_STAGE_CORNER_Y 154
#define P2_STAGE_CORNER_X 844
#define P2_STAGE_CORNER_Y 154
#define P1_NEXT_CORNER_X 587
#define P1_NEXT_CORNER_Y 105
#define P2_NEXT_CORNER_X 702
#define P2_NEXT_CORNER_Y 105
#define HOLD_CORNER_X 660
#define HOLD_CORNER_Y 255
using namespace std;
using namespace sf;

typedef enum PLAYROLE
{
    roleNONE,
    rolePLAYER1,
    rolePLAYER2,
};
typedef enum gridShape
{
    shapeI,
    shapeS,
    shapeZ,
    shapeT,
    shapeL,
    shapeJ,
    shapeO,
};

class Tetris
{
public:
    Tetris();
    ~Tetris();
    Vector2i mCornPoint, nextSquareCornPoint, holdSquareCornPoint; //游戏区域位置

    int gridSize; //块大小
    int imgBGNo, imgSkinNo;
    int role;
    Texture* tTiles;
    Texture tBackground, tButtons, tNum, tTimer, tCounter, tGameOver;        //创建纹理对象
    Sprite sBackground, sTiles, sButtons, sNum, sTimer, sCounter, sGameOver; //创建精灵对象

    int Field[STAGE_HEIGHT][STAGE_WIDTH] = { 0 };
    Vector2i currentSquare[4], nextSquare[4], tempSquare[4], shadowSquare[4];
    int Figures[7][4] =
    {
        3, 5, 1, 7, // I
        4, 5, 2, 7, // S
        4, 5, 3, 6, // Z
        5, 3, 4, 7, // T
        5, 3, 2, 7, // L
        5, 7, 3, 6, // J
        2, 3, 4, 5, // O
    };

    int dx;
    bool rotate, hold, hardDrop, newShapeFlag, animationFlag;
    bool gameOver = 0;
    int animationRow[4];
    int score;
    float animationCtrlValue;
    float timer, delay;

    int DALAYVALUE = 0.05;
    int colorNum, nextColorNum, tempcolorNum;
    int currentShapeNum, nextShapeNum, tempShapeNum;

    static int holdcolorNum, holdShapeNum;
    static Vector2i holdSquare[4];
    int b7array[7] = { 0 }, b7Int;
    int Bag7();
    RenderWindow* window;
    void Initial(Texture* tex);
    void Input(sf::Event event);
    void Logic();
    void Draw(sf::RenderWindow* window);
    bool hitTest();

    void checkLine();
    void xMove();
    void rotateFunc();
    void yMove();
    void holdFunc();
    void slowLoading();
    void hardDropFunc();
    void shadow();

    void traditionLogic();
    void clearLine();
    void newShapeFunc();
    void animationFunc(int i);
    void isWin();
};
class TetrisObject
{
    int colorNum, nextColorNum, tempcolorNum;
    int currentShapeNum, nextShapeNum, tempShapeNum;
    Vector2i currentSquare[4], nextSquare[4], tempSquare[4], shadowSquare[4];
    static Vector2i holdSquare[4];
    static int holdcolorNum, holdShapeNum;

};








