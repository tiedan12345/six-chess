#ifndef GAMETYPE_H
#define GAMETYPE_H
#include <vector>
const int boardSize = 20;// 棋盘尺寸
enum GameType//枚举
{
    PERSON,
    COMPUTER
};
enum GameStatus// 游戏状态,不必写dead
{
    playing,
    win
};

class GameModel
{
public:
    GameModel();
public:
    std::vector<std::vector<int>> ctatus; // 每个点的棋子，空白为0，白子1，黑子-1
    std::vector<std::vector<int>> score; // 每个点的评分
    GameType gameType; // 游戏模式
    GameStatus gameStatus; // 游戏状态
    bool playerFlag; // 标示下棋方
    void startGame(GameType type); // 开始游戏
    void calculation(); // 计算评分
    void person(int row, int col); // 人执行下棋
    void ai(int &clickRow, int &clickCol); // 机器执行下棋
    void update(int row, int col); // 每次落子后更新游戏棋盘
    bool win(int row, int col); // 判断游戏是否胜利
    bool heqi(); // 判断是否和棋
};

#endif // GAMETYPE_H
