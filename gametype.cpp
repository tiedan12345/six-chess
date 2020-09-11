#include "gametype.h"
#include <utility>
#include <time.h>
GameModel::GameModel()
{
}

void GameModel::startGame(GameType type)
{
    gameType=type;// 初始棋盘
    ctatus.clear();//清空
    for (int i=0;i<boardSize;i++)
    {
        std::vector<int>lineBoard;
        for (int j = 0; j < boardSize; j++)
            lineBoard.push_back(0);
        ctatus.push_back(lineBoard);
    }
    if (gameType == COMPUTER)
    {
        score.clear();//需要初始化评分数组
        for (int i = 0; i < boardSize; i++)
        {
            std::vector<int> lineScores;
            for (int j = 0; j < boardSize; j++)
                lineScores.push_back(0);
            score.push_back(lineScores);
        }
    }
    playerFlag = true;


}

void GameModel::update(int row, int col)
{
    if (playerFlag)
        ctatus[row][col] = 1;
    else
        ctatus[row][col] = -1;
    playerFlag = !playerFlag;// 换手
}

void GameModel::person(int row, int col)//人下棋
{
    update(row, col);
}

void GameModel::ai(int &clickRow, int &clickCol)//ai部分
{
    calculation();
    int maxScore = 0;
    std::vector<std::pair<int, int>> maxPoints;
    for (int row = 1; row < boardSize; row++)
        for (int col = 1; col < boardSize; col++)
        {
            if (ctatus[row][col] == 0)
            {
                if (score[row][col] > maxScore)          // 找最大的数和坐标
                {
                    maxPoints.clear();
                    maxScore = score[row][col];
                    maxPoints.push_back(std::make_pair(row, col));
                }
                else if (score[row][col] == maxScore)     // 如果有多个最大的数，都存起来
                    maxPoints.push_back(std::make_pair(row, col));
            }
        }
    srand((unsigned)time(0));//如果最大的数有好几个，就随机落子
    int index = rand() % maxPoints.size();
    std::pair<int, int> pointPair = maxPoints.at(index);
    clickRow = pointPair.first; // 记录落子点
    clickCol = pointPair.second;
    update(clickRow, clickCol);
}
void GameModel::calculation()
{
    int personNum = 0; // 玩家连成子的个数
    int aiNum = 0; // AI连成子的个数
    int emptyNum = 0; // 各方向空白位的个数
    score.clear();
    for (int i = 0; i < boardSize; i++)
    {
        std::vector<int> lineScores;
        for (int j = 0; j < boardSize; j++)
            lineScores.push_back(0);
        score.push_back(lineScores);
    }
    for (int row = 0; row < boardSize; row++)
        for (int col = 0; col < boardSize; col++)
        {
            if (row > 0 && col > 0 &&
                ctatus[row][col] == 0)  //空白点（除棋盘四周）
            {
                // 遍历周围八个方向，相当于坐标
                for (int y = -1; y <= 1; y++)
                    for (int x = -1; x <= 1; x++)
                    {
                        personNum = 0;
                        aiNum = 0;
                        emptyNum = 0;
                        if (!(y == 0 && x == 0))
                        {
                            for (int i = 1; i <= 5; i++)
                            {
                                if (row + i * y > 0 && row + i * y < boardSize &&
                                    col + i * x > 0 && col + i * x < boardSize&&
                                    ctatus[row + i * y][col + i * x] == 1)
                                {
                                    personNum++;
                                }
                                else if (row + i * y > 0 && row + i * y < boardSize&&
                                         col + i * x > 0 && col + i * x < boardSize&&
                                         ctatus[row + i * y][col + i * x] == 0)
                                {
                                    emptyNum++;
                                    break;
                                }
                                else
                                    break;
                            }

                            for (int i = 1; i <= 5; i++)
                            {
                                if (row - i * y > 0 && row - i * y < boardSize&&
                                    col - i * x > 0 && col - i * x < boardSize&&
                                    ctatus[row - i * y][col - i * x] == 1)
                                {
                                    personNum++;
                                }
                                else if (row - i * y > 0 && row - i * y < boardSize&&
                                         col - i * x > 0 && col - i * x < boardSize&&
                                         ctatus[row - i * y][col - i * x] == 0)
                                {
                                    emptyNum++;
                                    break;
                                }
                                else
                                    break;
                            }

                           if (personNum == 1)
                               score[row][col] += 10;

                           else if(personNum == 2)
                               score[row][col] += 13;

                           else if(personNum ==3)
                               score[row][col] += 15;

                           else if(personNum == 4)
                           {
                               if(emptyNum == 1)
                                  score[row][col] += 15;

                                else if(emptyNum == 2)
                               score[row][col] += 8000;
                           }
                           else if(personNum >= 5)
                               score[row][col] += 10000;

                            emptyNum = 0;

                            for (int i = 1; i <= 5; i++)
                            {
                                if (row + i * y > 0 && row + i * y < boardSize&&
                                    col + i * x > 0 && col + i * x < boardSize&&
                                    ctatus[row + i * y][col + i * x] == -1)
                                {
                                    aiNum++;
                                }
                                else if (row + i * y > 0 && row + i * y < boardSize&&
                                         col + i * x > 0 && col + i * x < boardSize&&
                                         ctatus[row +i * y][col + i * x] == 0)
                                {
                                    emptyNum++;

                                }
                                else
                                    break;
                            }

                            for (int i = 1; i <= 5; i++)
                            {
                                if (row - i * y > 0 && row - i * y < boardSize&&
                                    col - i * x > 0 && col - i * x < boardSize&&
                                    ctatus[row - i * y][col - i * x] == -1)
                                {
                                    aiNum++;
                                }
                                else if (row - i * y > 0 && row - i * y < boardSize&&
                                         col - i * x > 0 && col - i * x < boardSize&&
                                         ctatus[row - i * y][col - i * x] == 0)
                                {
                                    emptyNum++;
                                    break;

                                }
                                else
                                    break;
                            }
                            if(aiNum == 0)
                                score[row][col] += 1;

                            else if(aiNum == 1)
                                score[row][col] += 13;

                            else if(aiNum == 2)
                                score[row][col] += 17;

                            else if(aiNum == 3)
                            {
                                if(emptyNum == 1)
                                    score[row][col] += 18;
                                else if(emptyNum == 2)
                                    score[row][col] += 27;
                            }

                            else if(aiNum == 4)
                            {
                                if(emptyNum == 1)
                                       score[row][col] += 29;

                                else if(emptyNum == 2)
                                        score[row][col] += 800;

                                else if(emptyNum == 0)
                                    score[row][col] += 0;
                            }

                            else if(aiNum >= 5)
                                score[row][col] += 9000;
                        }
                    }

            }
        }
}

bool GameModel::win(int row, int col)
{
    // 水平方向
    for (int i = 0; i < 6; i++)
    {
        if (col - i > 0 && col - i + 5< boardSize&&
            ctatus[row][col - i] == ctatus[row][col - i + 1] &&
            ctatus[row][col - i] == ctatus[row][col - i + 2] &&
            ctatus[row][col - i] == ctatus[row][col - i + 3] &&
            ctatus[row][col - i] == ctatus[row][col - i + 4]&&
            ctatus[row][col - i] == ctatus[row][col - i + 5])
            return true;
    }

    // 竖直方向
    for (int i = 0; i < 6; i++)
    {
        if (row - i > 0 &&row - i + 5 < boardSize&&
            ctatus[row - i][col] == ctatus[row - i + 1][col] &&
            ctatus[row - i][col] == ctatus[row - i + 2][col] &&
            ctatus[row - i][col] == ctatus[row - i + 3][col] &&
            ctatus[row - i][col] == ctatus[row - i + 4][col]&&
            ctatus[row - i][col] == ctatus[row - i + 5][col])
            return true;
    }

    // 左斜方向
    for (int i = 0; i < 6; i++)
    {
        if (row + i < boardSize&&row + i - 5 > 0 &&col - i > 0 &&col - i + 5 < boardSize&&
            ctatus[row + i][col - i] == ctatus[row + i - 1][col - i + 1] &&
            ctatus[row + i][col - i] == ctatus[row + i - 2][col - i + 2] &&
            ctatus[row + i][col - i] == ctatus[row + i - 3][col - i + 3] &&
            ctatus[row + i][col - i] == ctatus[row + i - 4][col - i + 4]&&
            ctatus[row + i][col - i] == ctatus[row + i - 5][col - i + 5])
            return true;
    }

    // 右斜方向
    for (int i = 0; i < 6; i++)
    {
        if (row - i > 0 &&row - i + 5 < boardSize&&col - i > 0 &&col - i + 5< boardSize&&
            ctatus[row - i][col - i] == ctatus[row - i + 1][col - i + 1] &&
            ctatus[row - i][col - i] == ctatus[row - i + 2][col - i + 2] &&
            ctatus[row - i][col - i] == ctatus[row - i + 3][col - i + 3] &&
            ctatus[row - i][col - i] == ctatus[row - i + 4][col - i + 4]&&
            ctatus[row - i][col - i] == ctatus[row - i + 5][col - i + 5])
            return true;
    }

    return false;
}

bool GameModel::heqi()
{
    // 所有空格全部填满
    for (int i = 1; i < boardSize; i++)
        for (int j = 1; j < boardSize; j++)
        {
            if (!(ctatus[i][j] == 1 || ctatus[i][j] == -1))
                return false;
        }
    return true;
}


