#include <QPainter>
#include <QTimer>
#include <QMouseEvent>
#include <QMessageBox>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <math.h>
#include "mainwindow.h"

const int BoardMargin=30; // 棋盘边缘空隙
const int Radius=15; // 棋子半径
const int MarkSize=6; // 落子标记边长
const int BlockSize=40; // 格子的大小
const int PosDelta=20; // 鼠标点击的模糊距离上限
const int AIDelay=700; // AI下棋的思考时间

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
        setFixedSize(BoardMargin*2+BlockSize*boardSize,BoardMargin*2+BlockSize*boardSize);// 设置固定棋盘大小（宽，高）
        setMouseTracking(true);// 开启鼠标hover功能
        QMenu *caidan = menuBar()->addMenu(tr("游戏模式")); // 添加菜单栏
        QAction *action1 = new QAction("Person VS Person", this);
        connect(action1, SIGNAL(triggered()), this, SLOT(initPVPGame()));
        caidan->addAction(action1);
        QAction *action2 = new QAction("Person VS Computer", this);
        connect(action2, SIGNAL(triggered()), this, SLOT(initPVEGame()));
        caidan->addAction(action2);
        playGame(); // 开始游戏
}
MainWindow::~MainWindow()
{
    if (game)
       {
           delete game;
           game=nullptr;
       }
}
void MainWindow::playGame()
{
    // 初始化游戏模型
    game=new GameModel;
    initPVPGame();
}
void MainWindow::initPVPGame()
{
    game_type=PERSON;
    game->gameStatus=playing;
    game->startGame(game_type);
    update();
}
void MainWindow::initPVEGame()
{
    game_type=COMPUTER;
    game->gameStatus=playing;
    game->startGame(game_type);
    update();
}
void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    // 绘制棋盘
    painter.setRenderHint(QPainter::Antialiasing, true); // 抗锯齿
    QPen pen; // 调整线条宽度
    pen.setWidth(2);
    painter.setPen(pen);
    for (int i = 0; i < boardSize + 1; i++)
    {
        painter.drawLine(BoardMargin +BlockSize * i, BoardMargin, BoardMargin + BlockSize * i, size().height() - BoardMargin);
        painter.drawLine(BoardMargin, BoardMargin + BlockSize * i, size().width() - BoardMargin, BoardMargin + BlockSize * i);
    }
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    // 绘制落子标记(防止鼠标出框越界)
    if (clickPosRow > 0 && clickPosRow < boardSize &&
        clickPosCol > 0 && clickPosCol < boardSize &&
        game->ctatus[clickPosRow][clickPosCol] == 0)
    {
        if (game->playerFlag)
            brush.setColor(Qt::white);//己方下白子
        else
            brush.setColor(Qt::black);//对方下黑子
        painter.setBrush(brush);
        painter.drawRect(BoardMargin + BlockSize * clickPosCol - MarkSize / 2, BoardMargin + BlockSize * clickPosRow - MarkSize / 2, MarkSize, MarkSize);
    }//绘制一个矩形作为落子标记，己方标记为白色，对方标记为黑色
    for (int i = 0; i < boardSize; i++)
        for (int j = 0; j < boardSize; j++)
        {
            if (game->ctatus[i][j] == 1)
            {
                brush.setColor(Qt::white);
                painter.setBrush(brush);
                painter.drawEllipse(BoardMargin + BlockSize * j - Radius, BoardMargin + BlockSize * i - Radius, Radius * 2, Radius * 2);
            }
            else if (game->ctatus[i][j] == -1)
            {
                brush.setColor(Qt::black);
                painter.setBrush(brush);
                painter.drawEllipse(BoardMargin + BlockSize * j - Radius, BoardMargin + BlockSize * i - Radius, Radius * 2, Radius * 2);
            }
        }// 绘制棋子，圆形是椭圆的一种（圆心，直径，直径）
    if (clickPosRow > 0 && clickPosRow < boardSize &&
        clickPosCol > 0 && clickPosCol < boardSize&&
        (game->ctatus[clickPosRow][clickPosCol] == 1 ||
            game->ctatus[clickPosRow][clickPosCol] == -1))
    {
        if (game->win(clickPosRow, clickPosCol) && game->gameStatus == playing)
        {
            game->gameStatus =win;
            QString str;
            if (game->ctatus[clickPosRow][clickPosCol] == 1)
                str = "white player ";
            else if (game->ctatus[clickPosRow][clickPosCol] == -1)
                str = "black player ";
            QMessageBox::StandardButton btnValue = QMessageBox::information(this, "congratulations", str + "win!");// 判断输赢
            if (btnValue == QMessageBox::Yes)
            {
                game->startGame(game_type);
                game->gameStatus = playing;
            } // 重置游戏状态
        }
    }

    if (game->heqi())
    {
        QMessageBox::StandardButton btnValue = QMessageBox::information(this, "sorry", "It's a dead game!");
        if (btnValue == QMessageBox::Yes)
        {
            game->startGame(game_type);
            game->gameStatus = playing;
        }

    }//和棋
}
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    // 通过鼠标的hover确定落子的标记
    int x = event->x();
    int y = event->y();
    if (x >= BoardMargin + BlockSize / 2 &&
            x < size().width() - BoardMargin &&
            y >= BoardMargin + BlockSize / 2 &&
            y < size().height()- BoardMargin)
    {
        int col = x / BlockSize;
        int row = y / BlockSize;// 获取最近的左上角的点
        int leftTopPosX = BoardMargin + BlockSize * col;
        int leftTopPosY = BoardMargin + BlockSize * row;
        // 根据距离算出合适的点击位置,一共四个点，根据半径距离选最近的
        clickPosRow = -1; // 初始化最终的值
        clickPosCol = -1;
        int len = 0; // 计算完后取整就可以了
        // 确定一个误差在范围内的点，且只可能确定一个出来
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));
        if (len < PosDelta)
        {
            clickPosRow = row;
            clickPosCol = col;
        }
        len = sqrt((x - leftTopPosX - BlockSize) * (x - leftTopPosX - BlockSize) + (y - leftTopPosY) * (y - leftTopPosY));
        if (len < PosDelta)
        {
            clickPosRow = row;
            clickPosCol = col + 1;
        }
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY - BlockSize) * (y - leftTopPosY - BlockSize));
        if (len < PosDelta)
        {
            clickPosRow = row + 1;
            clickPosCol = col;
        }
        len = sqrt((x - leftTopPosX - BlockSize) * (x - leftTopPosX - BlockSize) + (y - leftTopPosY - BlockSize) * (y - leftTopPosY - BlockSize));
        if (len < PosDelta)
        {
            clickPosRow = row + 1;
            clickPosCol = col + 1;
        }
    }
    update();
}
void MainWindow::mouseReleaseEvent(QMouseEvent*event)
{
    if (!(game_type == COMPUTER && !game->playerFlag))
    {
        chessOneByPerson();
        if (game->gameType == COMPUTER && !game->playerFlag)// AI下棋
        {
            QTimer::singleShot(AIDelay, this, SLOT(chessOneByAI()));//延迟
        }
    }
}

void MainWindow::chessOneByPerson()
{
    // 根据当前存储的坐标下子
    // 只有有效点击才下子，并且该处没有子
    if (clickPosRow != -1 && clickPosCol != -1 && game->ctatus[clickPosRow][clickPosCol] == 0)
    {
        game->person(clickPosRow, clickPosCol);
        update();
    }
}

void MainWindow::chessOneByAI()
{
    game->ai(clickPosRow, clickPosCol);
    update();
}



