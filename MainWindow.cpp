#include "include/MainWindow.h"
#include <QPixmap>
#include <QPalette>
#include <QApplication>
#include <QScreen>
#include <QMainWindow>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
    setFixedSize(900, 750);// 设置窗口大小

    // 设置背景图片
    QPalette palette;
    QPixmap backgroundImage("D://boyi//Qt6AmazonAI//images//bg.png");  // 使用资源文件路径
    palette.setBrush(backgroundRole(), QBrush(backgroundImage));
    setPalette(palette);

    //位于显示屏中间
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry =  screen->availableGeometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();
    int windowWidth = width();
    int windowHeight = height();
    int x = (screenWidth - windowWidth) / 2;
    int y = (screenHeight-windowHeight)/2;
    move(x, y);

    //添加棋盘
    Board *board = new Board(this);
    setCentralWidget(board);
}

MainWindow::~MainWindow() noexcept {

}


