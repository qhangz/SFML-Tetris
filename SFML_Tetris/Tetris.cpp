#pragma once

#include "Tetris.h"


Tetris::Tetris()
{
    dx = 0;         // X方向偏移量
    rotate = false; //是否旋转
    colorNum = 1;   //色块的颜色
    timer = 0;
    delay = 10; //下落的速度
}
Tetris::~Tetris()
{
}
//p270
void Tetris::xMove()
{
    ///<-水平Move->///
    for (int i = 0; i < 4; i++)
    {
        tempSquare[i] = currentSquare[i];
        currentSquare[i].x += dx;
    }
    if (!hitTest())
    {
        for (int i = 0; i < 4; i++)
        {
            currentSquare[i] = tempSquare[i];
        }
    }
}
void Tetris::Logic()
{
    if (!animationFlag)
    {
        traditionLogic();
    }
    if (newShapeFlag)
    {
        if (animationFlag == false)
        {
            checkLine();
            isWin();
            if (gameOver == true)
            {

            }
            else {
                if (animationFlag == false)
                {
                    newShapeFunc();
                }
            }


        }
        else
        {
            animationCtrlValue -= 0.1;
            if (animationCtrlValue <= 0)
            {
                animationFlag = false;
                animationCtrlValue = 1;
                for (int i = 0; i < 4; i++)
                {
                    animationRow[i] = -1;
                }
                clearLine();
                newShapeFunc();
            }
        }
    }
}
void Tetris::traditionLogic()
{

    if (hold)
    {
        holdFunc();
        hold = false;
    }
    xMove();
    if (rotate)
    {
        rotateFunc();
        rotate = false;
    }
    //slowLoading();
    yMove();
    shadow();
    if (hardDrop)
    {
        hardDropFunc();
        hardDrop = false;
    }
    // checkLine();
}

bool Tetris::hitTest()
{
    for (int i = 0; i < 4; i++)
    {
        if (currentSquare[i].x < 0 || currentSquare[i].x >= STAGE_WIDTH || currentSquare[i].y >= STAGE_HEIGHT)
        {
            return false;
        }
        else if (Field[currentSquare[i].y][currentSquare[i].x])
        {
            return false;
        }
    }
    return true;
}
////////////
// Rotate
void Tetris::rotateFunc()
{
    int originalHeight = currentSquare[0].y;
    for (int j = 0; j < 4; j++)
    {
        Vector2i p = currentSquare[j]; //设置旋转中心点
        for (int i = 0; i < 4; i++)
        {
            //顺时针旋转90
            int x = currentSquare[i].y - p.y; //原Y方向距离中心点的差值，作为新的差值，传递给X方向
            int y = currentSquare[i].x - p.x; //原X方向距离中心点的差值，作为新的差值，传递给Y方向
            currentSquare[i].x = p.x - x;     //新坐标X=中心点坐标-新的X方向差值
            currentSquare[i].y = p.y + y;     //新坐标Y=中心点坐标+新的Y方向差值
        }
        if (hitTest())
        {
            int detaY = 0;
            detaY = currentSquare[0].y - originalHeight; //新老重心的高度差
            if (detaY != 0)
            {
                for (int i = 0; i < 4; i++)
                {
                    currentSquare[i].y -= detaY;
                }
            }
            if (!hitTest())
            {
                for (int i = 0; i < 4; i++)
                {
                    currentSquare[i] = tempSquare[i];
                }
            }
            else
            {
                break;
            }
        }
        else
        {
            for (int i = 0; i < 4; i++)
            {
                currentSquare[i] = tempSquare[i];
            }
        }
    }
}

//////////////
// Tick下落
void Tetris::yMove()
{
    if (timer > delay)
    {
        //在图形下落之前先将各坐标进行保存
        for (int i = 0; i < 4; i++)
        {
            tempSquare[i] = currentSquare[i];
            currentSquare[i].y += 1;
        }

        if (!hitTest()) //如果发生碰撞则将未撞之前的坐标进行标记
        {
            for (int i = 0; i < 4; i++)
            {
                Field[tempSquare[i].y][tempSquare[i].x] = colorNum;
            }
            newShapeFunc();
            newShapeFlag = true;
        }
        timer = 0;
    }
}
void Tetris::newShapeFunc()
{
    colorNum = nextColorNum;
    currentShapeNum = nextShapeNum;
    //更新下一个方块图形
    nextShapeNum = Bag7();
    nextColorNum = rand() % 7 + 1;
    for (int i = 0; i < 4; i++)
    {
        currentSquare[i] = nextSquare[i];
        currentSquare[i].x += STAGE_WIDTH / 2;
        nextSquare[i].x = Figures[nextShapeNum][i] % 2;
        nextSquare[i].y = Figures[nextShapeNum][i] / 2;
    }

    shadow();
    newShapeFlag = false;
}
void Tetris::checkLine()
{
    int k = STAGE_HEIGHT - 1;
    int yCount = 0;
    for (int i = STAGE_HEIGHT - 1; i > 0; i--)  //从舞台下方往上进行遍历
    {
        int xCount = 0;
        for (int j = 0; j < STAGE_WIDTH; j++)   //统计每行的格子数
        {
            if (Field[i][j])    //背景图上，相应坐标有格子，则统计格子数
            {
                xCount++;
            }
            // Field[k][j]=Field[i][j];     //将当前统计行的内容，复制到目标行
        }
        //如果当前行满格，则标行停留在当前行，下个循环的时候，行间的复制，
        //则能覆盖原先行的内容，达到消行的目的。这里的消行操作，实
        //质是用上方行的内容，覆益满格行的内容
        if (xCount < STAGE_WIDTH)   
            k--;
        else
        {
            animationRow[yCount] = i;
            yCount++;
            animationFlag = true;
        }
    }
    switch (yCount)
    {
    case 1:
        score += 10;
        break;
    case 2:
        score += 30;
        break;
    case 3:
        score += 60;
        break;
    case 4:
        score += 100;
        break;
    }
}



void Tetris::Initial(Texture* tex)
{
    //
    for (int i = 0; i < STAGE_HEIGHT; i++)
    {
        for (int j = 0; j < STAGE_WIDTH; j++)
        {
            Field[i][j] = 0;
        }
    }
    animationFlag = true;
    animationCtrlValue = 1.0;
    tTiles = tex;
    dx = 0;
    gameOver = 0;
    hardDrop = false;
    rotate = false;
    hold = false; //是否有hold块图形
    colorNum = 1;
    timer = 0;
    delay = 0.5;
    holdSquareCornPoint = { HOLD_CORNER_X, HOLD_CORNER_Y };
    sTiles.setTexture(*tTiles);
    b7Int = 0;
    if (role == rolePLAYER1)
    {
        mCornPoint = { P1_STAGE_CORNER_X, P1_STAGE_CORNER_Y };
        nextSquareCornPoint = { P1_NEXT_CORNER_X, P1_NEXT_CORNER_Y };
    }
    if (role == rolePLAYER2)
    {
        mCornPoint = { P2_STAGE_CORNER_X, P2_STAGE_CORNER_Y };
        nextSquareCornPoint = { P2_NEXT_CORNER_X, P2_NEXT_CORNER_Y };
    }
    sTiles.setTexture(*tTiles);
    //初始化方块图形
    colorNum = 1 + rand() % 7;
    currentShapeNum = rand() % 7;
    //更新下一个方块图形
    nextColorNum = 1 + rand() % 7;
    nextShapeNum = rand() % 7;
    // int n=rand()%7;
    for (int i = 0; i < 4; i++)
    {
        currentSquare[i].x = Figures[currentShapeNum][i] % 2 + STAGE_WIDTH / 2;
        currentSquare[i].y = Figures[currentShapeNum][i] / 2;
        nextSquare[i].x = Figures[nextShapeNum][i] % 2;
        nextSquare[i].y = Figures[nextShapeNum][i] / 2;
        // animation
        animationRow[i] = -1;
    }
    holdShapeNum = -1; //在游戏开始，将hold区的图形设置为一个异常值

    for (int i = 0; i > STAGE_HEIGHT; i++)
    {
        for (int j = 0; j > STAGE_WIDTH; j++)
        {
            Field[i][j] = 0;
        }
    }
}

void Tetris::Input(sf::Event event)
{
    //cout << "role" << role << endl;
    if (role == rolePLAYER1)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::A)
            {
                dx = -1;
                //cout << "dx" << dx << endl;
                // rotate=false;
            }
            if (event.key.code == sf::Keyboard::D)
            {
                dx = 1;
                // rotate=false;
            }
            if (event.key.code == sf::Keyboard::W)
            {
                rotate = true;
            }
            if (event.key.code == sf::Keyboard::S)
            {
                delay = 0.1;
            }
        }
        if (event.type == Event::KeyReleased)
        {
            if (event.key.code == Keyboard::LControl)
            {
                hold = true;
            }
            if (event.key.code == sf::Keyboard::Space)
            {
                hardDrop = true;
            }
            if (event.key.code == sf::Keyboard::A)
            {
                dx = 0;
            }
            if (event.key.code == sf::Keyboard::D)
            {
                dx = 0;
            }
            // if (event.key.code == sf::Keyboard::W)
            // {
            //     if (currentShapeNum != shapeO)
            //     {
            //         rotate = true;
            //     }
            // }
            if (event.key.code == sf::Keyboard::S)
            {
                delay = 0.5;
            }
        }
    }
    if (role == rolePLAYER2)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Left)
            {
                dx = -1;
                // rotate=false;
            }
            if (event.key.code == sf::Keyboard::Right)
            {
                dx = 1;
                // rotate=false;
            }
            if (event.key.code == sf::Keyboard::Up)
            {
                if (currentShapeNum != shapeO)
                {
                    rotate = true;
                }
            }
            if (event.key.code == sf::Keyboard::Down)
            {
                delay = 0.1;
            }
        }
        if (event.type == Event::KeyReleased)
        {
            if (event.key.code == sf::Keyboard::RControl)
            {
                hold = true;
            }
            if (event.key.code == sf::Keyboard::Enter)
            {
                hardDrop = true;
            }
            if (event.key.code == sf::Keyboard::Left)
            {
                dx = 0;
            }
            if (event.key.code == sf::Keyboard::Right)
            {
                dx = 0;
            }
            //   if(event.key.code==sf::Keyboard::Up)
            //   {
            //     rotate=false;
            //   }
            if (event.key.code == sf::Keyboard::Down)
            {
                delay = 0.5;
            }
        }
    }
}

void Tetris::Draw(sf::RenderWindow* w)
{

    window = w;
    if (animationFlag == false)
    {
        //绘制Shadow的方块
        for (int i = 0; i < 4; i++)
        {
            sTiles.setTextureRect(sf::IntRect(colorNum * GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
            sTiles.setPosition(shadowSquare[i].x * GRIDSIZE, shadowSquare[i].y * GRIDSIZE);
            sTiles.setColor(Color(50, 50, 50, 255));
            sTiles.move(mCornPoint.x, mCornPoint.y);
            window->draw(sTiles);
            sTiles.setColor(Color(255, 255, 255, 255));
        }
        //活动的方块
        for (int i = 0; i < STAGE_HEIGHT; i++)
        {
            for (int j = 0; j < STAGE_WIDTH; j++)
            {
                if (Field[i][j])
                {
                    sTiles.setTextureRect(IntRect(Field[i][j] * GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
                    sTiles.setPosition(j * GRIDSIZE, i * GRIDSIZE);
                    sTiles.move(mCornPoint.x, mCornPoint.y);
                    window->draw(sTiles);
                }
            }
        }
        for (int i = 0; i < 4; i++)
        {
            sTiles.setPosition(currentSquare[i].x * GRIDSIZE, currentSquare[i].y * GRIDSIZE);
            sTiles.setTextureRect(sf::IntRect(colorNum * GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
            sTiles.move(mCornPoint.x, mCornPoint.y);
            window->draw(sTiles);
        }
    }
    //绘制固定的方块
    for (int i = 0; i < STAGE_HEIGHT; i++)
    {
        if (i == animationRow[0] || i == animationRow[1] || i == animationRow[2] || i == animationRow[3])
        {
            animationFunc(i);
        }
        else
        {
            for (int j = 0; j < STAGE_WIDTH; j++)
            {
                if (Field[i][j] == 0)
                {
                    continue;
                }
                sTiles.setTextureRect(IntRect(Field[i][j] * GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
                sTiles.setPosition(j * GRIDSIZE, i * GRIDSIZE);
                sTiles.move(mCornPoint.x, mCornPoint.y);
                // animationFlag=false;
                window->draw(sTiles);
            }
        }
    }
    //绘制Next区的方块
    for (int i = 0; i < 4; i++)
    {
        sTiles.setPosition(nextSquare[i].x * GRIDSIZE, nextSquare[i].y * GRIDSIZE);
        sTiles.setTextureRect(sf::IntRect(nextColorNum * GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
        sTiles.move(nextSquareCornPoint.x, nextSquareCornPoint.y);
        window->draw(sTiles);
    }
    //绘制Hold区的方块
    if (holdShapeNum > -1)
    {
        for (int i = 0; i < 4; i++)
        {
            sTiles.setPosition(holdSquare[i].x * GRIDSIZE, holdSquare[i].y * GRIDSIZE);
            sTiles.setTextureRect(sf::IntRect(holdShapeNum * GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
            sTiles.move(holdSquareCornPoint.x, holdSquareCornPoint.y);
            window->draw(sTiles);
        }
    }
}

int Tetris::Bag7()
{
    int num;
    srand(time(NULL));
    memset(b7array, -1, sizeof(b7array));
    num = rand() % 7;
    for (int i = 0; i < b7Int; i++)
    {
        if (b7array[i] == num)
        {
            i = -1;
            num = rand() % 7;
        }
    }
    b7array[b7Int] = num;
    b7Int++;
    if (b7Int == 7)
    {
        b7Int = 0;
        for (int i = 0; i < 7; i++)
        {
            b7array[i] = -1;
        }
    }
    return num;
}

int Tetris::holdcolorNum = 0, Tetris::holdShapeNum = 0;
Vector2i Tetris::holdSquare[4] = { {0, 0}, {0, 0}, {0, 0}, {0, 0} };

void Tetris::holdFunc()
{
    Vector2i backUpSquare[4];
    tempcolorNum = holdcolorNum;
    tempShapeNum = holdShapeNum;

    holdcolorNum = colorNum;
    holdShapeNum = currentShapeNum;

    for (int i = 0; i < 4; i++)
    {
        holdSquare[i].x = Figures[holdShapeNum][i] % 2;
        holdSquare[i].y = Figures[holdShapeNum][i] / 2;
        tempSquare[i].x = Figures[tempShapeNum][i] % 2;
        tempSquare[i].y = Figures[tempShapeNum][i] / 2;
        backUpSquare[i] = currentSquare[i];
    }

    if (tempShapeNum < 0) // hold区的异常值表示hold区为空的状态，所以要从Next区取值
    {
        //原hold区为空，从Next区取值
        newShapeFunc();
    }
    else
    {
        colorNum = tempcolorNum;
        currentShapeNum = tempShapeNum;
        //为从hold区置换过来额方块图形计算其在舞台上的坐标
        int minCurrentX = currentSquare[0].x, minCurrentY = currentSquare[0].y;
        int minTempX = tempSquare[0].x, minTempY = tempSquare[0].y;
        int dx, dy;
        for (int i = 1; i < 4; i++)
        {
            if (currentSquare[i].x < minCurrentX)
            {
                minCurrentX = currentSquare[i].x;
            }
            if (currentSquare[i].y < minCurrentY)
            {
                minCurrentY = currentSquare[i].y;
            }
            if (tempSquare[i].x < minTempX)
            {
                minTempX = tempSquare[i].x;
            }
            if (tempSquare[i].y < minTempY)
            {
                minTempY = tempSquare[i].y;
            }
        }
        dx = minCurrentX - minTempX;
        dy = minCurrentY - minTempY;
        for (int i = 0; i < 4; i++)
        {
            currentSquare[i].x = tempSquare[i].x + dx;
            currentSquare[i].y = tempSquare[i].y + dy;
            holdSquare[i].x = Figures[holdShapeNum][i] % 2;
            holdSquare[i].y = Figures[holdShapeNum][i] / 2;
        }
    }
    if (!hitTest())
    {
        colorNum = holdcolorNum;
        holdcolorNum = tempcolorNum;
        holdShapeNum = tempShapeNum;
        for (int i = 0; i < 4; i++)
        {
            currentSquare[i] = backUpSquare[i];
            holdSquare[i].x = Figures[holdShapeNum][i] % 2;
            holdSquare[i].y = Figures[holdShapeNum][i] / 2;
        }
    }
}
void Tetris::hardDropFunc()
{
    for (int j = 0; j < STAGE_HEIGHT; j++)
    {
        for (int i = 0; i < 4; i++)
        {
            currentSquare[i].y++;
        }
        if (!hitTest())
        {
            for (int i = 0; i < 4; i++)
            {
                currentSquare[i].y--;
            }
            break;
        }
    }
    for (int i = 0; i < 4; i++)
    {
        currentSquare[i] = shadowSquare[i];
    }
}
void Tetris::slowLoading()
{
    for (int i = 0; i < 4; i++)
    {
        tempSquare[i] = currentSquare[i];
        currentSquare[i].y++;
    }
    if (!hitTest())
    {
        delay = 3;
    }
    else
    {
        delay = 1;
    }
    for (int i = 0; i < 4; i++)
    {
        currentSquare[i] = tempSquare[i];
    }
}

void Tetris::shadow()
{
    for (int i = 0; i < 4; i++)
    {
        shadowSquare[i] = currentSquare[i];
    }
    for (int j = 0; j < STAGE_HEIGHT; j++)
    {
        for (int i = 0; i < 4; i++)
        {
            currentSquare[i].y++;
        }
        if (!hitTest())
        {
            for (int i = 0; i < 4; i++)
            {
                currentSquare[i].y--;
            }
            break;
        }
    }
    for (int i = 0; i < 4; i++)
    {
        tempSquare[i] = currentSquare[i];
        currentSquare[i] = shadowSquare[i];
        shadowSquare[i] = tempSquare[i];
    }
}
void Tetris::clearLine()
{
    int k = STAGE_HEIGHT - 1;
    for (int i = STAGE_HEIGHT - 1; i > 0; i--)
    {
        int xCount = 0;
        for (int j = 0; j < STAGE_WIDTH; j++)
        {
            if (Field[i][j] != 0)
            {
                xCount++;
            }
            Field[k][j] = Field[i][j];
        }
        if (xCount < STAGE_WIDTH)
        {
            k--;
        }
    }
}
void Tetris::animationFunc(int i)
{
    Vector2f p;
    sTiles.scale(animationCtrlValue, animationCtrlValue);
    p = sTiles.getOrigin();
    sTiles.setOrigin(GRIDSIZE / 2, GRIDSIZE / 2);
    sTiles.rotate(360 * animationCtrlValue);
    for (int j = 0; j < STAGE_WIDTH; j++)
    {
        sTiles.setTextureRect(IntRect(Field[i][j] * GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
        sTiles.setPosition(j * GRIDSIZE, i * GRIDSIZE);
        sTiles.move(mCornPoint.x + GRIDSIZE / 2, mCornPoint.y + GRIDSIZE / 2);
        window->draw(sTiles);
    }
    sTiles.scale(1.0 / animationCtrlValue, 1.0 / animationCtrlValue);
    sTiles.rotate(-360 * animationCtrlValue);
    sTiles.setOrigin(p);
}

void Tetris::isWin()
{
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            cout << "Field:" << endl;
            cout << Field[i][j] << " ";
        }
        cout << endl;
    }
    if (Field[2][5] || Field[2][6])
    {
        gameOver = true;
        cout << gameOver << endl;
    }
}