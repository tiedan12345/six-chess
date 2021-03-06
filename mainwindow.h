#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "gametype.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);// 鼠标移动情况，方便落子
    void mouseReleaseEvent(QMouseEvent *event);// 实际落子

private:
    GameModel *game; // 游戏指针
    GameType game_type; // 存储游戏类型
    int clickPosRow, clickPosCol; // 存储将点击的位置
    void playGame();
    void checkGame(int y, int x);

private slots://槽
    void chessOneByPerson(); // 人执行
    void chessOneByAI(); // AI下棋
    void initPVPGame();
    void initPVEGame();
};

#endif // MAINWINDOW_H
