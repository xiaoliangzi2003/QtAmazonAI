#include "include/Board.h"
#include <QPainter>
#include <iostream>
#include <QLabel>
#include <QToolButton>
#include <QPoint>
#include <QMessageBox>
#include "windows.h"
#include <ctime>
#include <fstream>

#define max(a,b) (a>b?a:b)
#define min(a,b) (a<b?a:b)
using namespace std;
double total_time=0;
int turn=0;//游戏回合数
QPoint temp[6];
int AIcolor;


Board::Board(QWidget *parent): QWidget(parent)
{
    initializeUI();
}

void Board::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 棋格颜色
    QColor darkColor(100, 100, 100);
    QColor lightColor(200, 200, 200);

    int gridSize = 60; // 棋格大小
    int boardSize = 10; // 棋盘大小
    int margin=150;//棋盘边距
    int borderWidth = 10; // 边框宽度
    painter.setOpacity(0.6);//棋盘不透明度
    qreal textOpacity = 0.8; // 文字不透明度

    // 绘制坐标标识
    painter.setOpacity(textOpacity);
    QFont font;
    font.setPointSize(24);
    painter.setFont(font);
    painter.setPen(Qt::white);
    font.setBold(true); // 加粗字体
    int fontSize = font.pointSize();//获取字体的大小

    // 绘制横坐标标识
    for (int col = 1; col <= boardSize; ++col)
    {
        char label = 'A' + col - 1;
        int x = (col - 1) * gridSize + margin-fontSize/2 + gridSize / 2;
        int y = margin / 2-fontSize;
        painter.drawText(x, y, QString(label));
        painter.drawText(x, y+gridSize*(boardSize+1)+fontSize/2, QString(label));
    }

    // 绘制纵坐标标识
    for (int row = 1; row <= boardSize; ++row)
    {
        int label = boardSize - row + 1;
        int x = margin - gridSize / 2-fontSize;
        int y = (row - 1) * gridSize + margin/2 + gridSize / 2+fontSize/2;
        painter.drawText(x, y, QString::number(label));
        painter.drawText(x+gridSize*(boardSize+1)+fontSize/2, y, QString::number(label));
    }


    // 计算棋盘绘制区域
    QRect boardRect(margin, margin/2, gridSize * boardSize, gridSize * boardSize);

    // 绘制边框
    painter.setPen(QPen(Qt::black, borderWidth));
    painter.drawRect(boardRect);

    // 绘制棋盘
    for (int row = 1; row <= boardSize; ++row)
    {
        for (int col = 1; col <= boardSize; ++col)
        {
            int x = (col - 1) * gridSize + margin;
            int y = (boardSize - row) * gridSize + margin/2;
            QColor color = (row + col) % 2 == 0 ? lightColor : darkColor;//根据行列确定颜色
            painter.fillRect(x, y, gridSize, gridSize, color);
        }
    }
    if(turn==0){
        initBoard(aBoard);
    }
    //绘制棋子和障碍
    QPixmap whiteImg1("D://boyi//Qt6AmazonAI//images//white.png");//白棋
    QPixmap blackImg1("D://boyi//Qt6AmazonAI//images//black.png");//黑棋
    QPixmap arrowImg1("D://boyi//Qt6AmazonAI//images//arrow.png");//障碍
    QSize chessSize(55, 55);
    QPixmap whiteImg = whiteImg1.scaled(chessSize, Qt::KeepAspectRatio);
    QPixmap blackImg= blackImg1.scaled(chessSize, Qt::KeepAspectRatio);
    QPixmap arrowImg= arrowImg1.scaled(chessSize, Qt::KeepAspectRatio);

    for(int i=1;i<=10;i++){
        for(int j=1;j<=10;j++){
            int x=(i-1)*gridSize+margin+boardSize;
            int y=margin/2+(j-1)*gridSize+boardSize-7;
            //棋盘坐标转像素坐标
            if(aBoard[i][j]==WHITECHESS){
                painter.drawPixmap(x, y, whiteImg);
            }else if(aBoard[i][j]==BLACKCHESS){
                painter.drawPixmap(x, y, blackImg);
            }else if(aBoard[i][j]==BARRIER){
                painter.drawPixmap(x-7,y,arrowImg);
            }
            if (chessSelected && i == selectedChessPos.x() && j == selectedChessPos.y()) {
                painter.setPen(QPen(Qt::blue, 3)); // 设置蓝色边框，宽度为3
                painter.drawRect(x - 10, y - 3, gridSize+1 , gridSize+1); // 绘制矩形方框
            }
        }
    }
}

QSize Board::sizeHint() const {
    int gridSize = 60; // 棋格大小
    int boardSize = 10; // 棋盘大小
    int margin = 150; // 边缘边距

    return QSize(gridSize * boardSize + 2 * margin, gridSize * boardSize + 2 * margin);

}

void Board::initBoard(int Board[12][12]) {
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            if (i == 0 || j == 0 || i == 11 || j == 11)
                Board[i][j] = BARRIER;
            else
                Board[i][j] = EMPTY;
        }
    }
    Board[1][4] = WHITECHESS;
    Board[4][1] = WHITECHESS;
    Board[7][1] = WHITECHESS;
    Board[10][4] = WHITECHESS;
    Board[1][7] = BLACKCHESS;
    Board[4][10] = BLACKCHESS;
    Board[7][10] = BLACKCHESS;
    Board[10][7] = BLACKCHESS;
}

void Board::mousePressEvent(QMouseEvent *event)
{
    int gridSize = 60; // 棋格大小
    int margin = 150; // 边缘边距

    // 计算鼠标点击的棋盘位置
    int row = (event->x() - margin + gridSize ) / gridSize; // 列
    int col = (event->y() - margin / 2 + gridSize) / gridSize; // 行

    if(firsthand==BLACKCHESS){
        if(turn%2==0){
            currentplayer=BLACKCHESS;
        }else if(turn%2==1){
            currentplayer=WHITECHESS;
        }
    }else if(firsthand==WHITECHESS){
        if(turn%2==0){
            currentplayer=BLACKCHESS;
        }else if(turn%2==1){
            currentplayer=WHITECHESS;
        }
    }
    // 根据点击次数执行不同操作
    if((mode==0&&tmode==0&&turn%2==0)||(mode==0&&tmode==1&&turn%2==1)||(mode==1)){
        if (clickCount == 0) {
            // 第一次点击，选中棋子位置
            if ((aBoard[row][col] == WHITECHESS && currentplayer == WHITECHESS) ||
                (aBoard[row][col] == BLACKCHESS && currentplayer == BLACKCHESS)) {
                cout << "the " << turn + 1 << " round:" << endl;
                selectedChessPos.setX(row);
                selectedChessPos.setY(col);
                cout << "piece is selected, please move the chess" << endl;
                // 记录起子位置
                pace[0].setX(row);
                pace[0].setY(col);
                update();
                chessSelected = true;
            }
            else {
                cout << "invalid select, please choose your own chess!" << endl;
            }
        }
        if (clickCount == 0&&chessSelected==true) {
            if((aBoard[row][col] == WHITECHESS && currentplayer == WHITECHESS) ||
               (aBoard[row][col] == BLACKCHESS && currentplayer == BLACKCHESS)){
                clickCount=0;
            }else if (aBoard[row][col] == EMPTY) {
                clickCount++;
                bool isEndValid= isValidMove(pace[0].x(),pace[0].y(),row,col,aBoard);
                if(isEndValid){
                    // 第二次点击，选中落子位置
                    pace[1].setX(row);
                    pace[1].setY(col);
                    cout << "second click, end" << endl;
                    updateBoard(aBoard);
                    update();
                    ++clickCount;
                    chessSelected = false;
                }else{
                    cout<<"invalid move"<<endl;
                    clickCount=0;
                }
            } else {
                cout << "invalid end point, please try again!" << endl;
            }
        } else if (clickCount == 2) {
            // 第三次点击，放置障碍位置
            if (aBoard[row][col] == EMPTY) {
                bool isArrowValid= isValidMove(pace[1].x(),pace[1].y(),row,col,aBoard);
                if(isArrowValid){
                    pace[2].setX(row);
                    pace[2].setY(col);
                    updateBoard(aBoard);
                    update();
                    writeLog();
                    ++clickCount;
                    cout << "last click, barrier" << endl;
                    turn++;
                }else{
                    cout<<"invalid arrow place."<<endl;
                }
            } else {
                cout << "invalid barrier point, please try again!" << endl;
            }

            // 重置点击次数
            if(clickCount==3){
                clickCount = 0;
            }
            // 更新棋盘数组并重新绘制棋盘和棋子
            if(JudgeWin(aBoard)==WHITECHESS){
                cout<<"White Win!"<<endl;
                clickCount=-1;
            }else if(JudgeWin(aBoard)==BLACKCHESS){
                cout<<"Black Win!"<<endl;
                clickCount=-1;
            }
        }
    }

    //执行AI操作
    if((turn%2==1&&mode==0&&tmode==0)||(turn%2==0&&mode==0&&tmode==1)||mode==-1){
        if(mode==-1){
            tmode=1-turn%2;
        }
        cout<<"the "<<turn+1<<" round:"<<endl;
        cout<<"search AI move."<<endl;
        clock_t start_time = std::clock();
        searchGoodMove(aBoard);
        //结束计时
        std::clock_t end_time = std::clock();
        double elapsed_time = static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC;
        total_time+=elapsed_time;
        std::cout << "Elapsed time: " << elapsed_time << " seconds" << std::endl;
        std::cout << "total time: " << total_time << " seconds" << std::endl;
        pace[0]={QPoint(BestMove.start.x(),BestMove.start.y())};
        pace[1]={QPoint(BestMove.end.x(),BestMove.end.y())};
        pace[2]={QPoint(BestMove.barrier.x(),BestMove.barrier.y())};
        aBoard[pace[0].x()][pace[0].y()]=EMPTY;
        aBoard[pace[1].x()][pace[1].y()]=BestMove.ID;
        aBoard[pace[2].x()][pace[2].y()]=BARRIER;
        temp[3]={pace[0].x(),pace[0].y()};
        temp[4]={pace[1].x(),pace[1].y()};
        temp[5]={pace[2].x(),pace[2].y()};
        AIcolor=BestMove.ID;
        cout<<"start:"<<char('A'+BestMove.start.x()-1)<<","<<11-BestMove.start.y()<<endl;
        cout<<"end:"<<char('A'+BestMove.end.x()-1)<<","<<11-BestMove.end.y()<<endl;
        cout<<"barrier:"<<char('A'+BestMove.barrier.x()-1)<<","<<11-BestMove.barrier.y()<<endl;
        update();
        writeLog();

        // 更新棋盘数组并重新绘制棋盘和棋子
        if(JudgeWin(aBoard)==WHITECHESS){
            cout<<"White Win!"<<endl;
            clickCount=-1;
        }else if(JudgeWin(aBoard)==BLACKCHESS){
            cout<<"Black Win!"<<endl;
            clickCount=-1;
        }
        turn++;
    }
}


void Board::updateBoard(int Board[12][12]) {
    int x1 = pace[0].x();
    int y1 = pace[0].y();
    temp[0]={x1,y1};
    int x2 = pace[1].x();
    int y2 = pace[1].y();
    temp[1]={x2,y2};
    int x3 = pace[2].x();
    int y3 = pace[2].y();
    temp[2]={x3,y3};
    Board[x1][y1] = EMPTY;
    Board[x2][y2] = currentplayer;
    Board[x3][y3] = BARRIER;
}
void Board::regret(int Board[12][12]){
    Board[temp[0].x()][temp[0].y()] = currentplayer;
    Board[temp[1].x()][temp[1].y()] = EMPTY;
    Board[temp[2].x()][temp[2].y()] = EMPTY;
    Board[temp[3].x()][temp[3].y()] = AIcolor;
    Board[temp[4].x()][temp[4].y()] = EMPTY;
    Board[temp[5].x()][temp[5].y()] = EMPTY;
    pace[0]=QPoint{-1,-1};
    pace[1]=QPoint{-1,-1};
    pace[2]=QPoint{-1,-1};
    turn=turn-2;
}


bool Board::isValidMove(int x1, int y1, int x2, int y2, int Board[12][12])
{
    //(x1,y1)是起始点，（x2,y2）是终点
    int a, b, m, n;
    a = x1;
    b = y1;
    m = x2;
    n = y2;
    if (abs(y2 - y1) == abs(x2 - x1) || x2 - x1 == 0 || y2 - y1 == 0)//确保起始点和落子点符合米字型路线
    {
        //以下代码是棋子八个方向的路径上判断是否有不为空的地方
        if (y2 - y1 > 0 && x2 - x1 == 0)
        {
            for (b = b + 1; b < n; b++)
                if (Board[a][b] != EMPTY)
                    return false;
            return true;
        }
        else if (y2 - y1 > 0 && x2 - x1 > 0)
        {
            for (a = a + 1, b = b + 1; a < m, b < n; a++, b++)
                if (Board[a][b] != EMPTY)
                    return false;
            return true;
        }
        else if (y2 - y1 == 0 && x2 - x1 > 0)
        {
            for (a = a + 1; a < m; a++)
                if (Board[a][b] != EMPTY)
                    return 0;
            return true;
        }
        else if (y2 - y1 < 0 && x2 - x1>0)
        {
            for (a = a + 1, b = b - 1; a<m, b>n; a++, b--)
                if (Board[a][b] != EMPTY)
                    return false;
            return true;
        }
        else if (y2 - y1 < 0 && x2 - x1 == 0)
        {
            for (b = b - 1; b > n; b--)
                if (Board[a][b] != EMPTY)
                    return false;
            return true;
        }
        else if (y2 - y1 < 0 && x2 - x1 < 0)
        {
            for (a = a - 1, b = b - 1; a > m, b > n; a--, b--)
                if (Board[a][b] != EMPTY)
                    return false;
            return true;
        }
        else if (y2 - y1 == 0 && x2 - x1 < 0)
        {
            for (a = a - 1; a > m; a--)
                if (Board[a][b] != EMPTY)
                    return false;
            return true;
        }
        else if (y2 - y1>0 && x2 - x1 < 0)
        {
            for (a = a - 1, b = b + 1; a > m, b < n; a--, b++)
                if (Board[a][b] != EMPTY)
                    return false;
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

int Board::JudgeWin(int Board[12][12])
{
    int C_Black = 0, C_White = 0;
    for (int i = 1; i <= 10; i++)
    {
        for (int j = 1; j <= 10; j++)
        {
            if (Board[i][j] == BLACKCHESS&&Board[i - 1][j - 1] != EMPTY&&Board[i - 1][j] != EMPTY&&Board[i - 1][j + 1] != EMPTY&&Board[i][j + 1] != EMPTY&&Board[i + 1][j + 1] != EMPTY&&Board[i + 1][j] != EMPTY&&Board[i + 1][j - 1] != EMPTY&&Board[i][j - 1] != EMPTY)
                C_Black++;
            else if (Board[i][j] == WHITECHESS&&Board[i - 1][j - 1] != EMPTY&&Board[i - 1][j] != EMPTY&&Board[i - 1][j + 1] != EMPTY&&Board[i][j + 1] != EMPTY&&Board[i + 1][j + 1] != EMPTY&&Board[i + 1][j] != EMPTY&&Board[i + 1][j - 1] != EMPTY&&Board[i][j - 1] != EMPTY)
                C_White++;
        }
    }
    if (C_Black == 4){
        return WHITECHESS;
    }
    else if (C_White == 4){
        return BLACKCHESS;
    }
    else return -2;
}

QToolButton* Board::createIconButton(const QString& iconPath, const QPoint& pos)
{
    QToolButton* button = new QToolButton(this);
    button->setIcon(QIcon(iconPath));
    button->setIconSize(QSize(80, 56));
    button->setAutoRaise(true);
    button->setToolButtonStyle(Qt::ToolButtonIconOnly);
    button->setGeometry(QRect(pos, QSize(72, 50.4)));
    return button;
}

void Board::initializeUI()
{
    // 创建模式按钮
    QToolButton* modeButton = createIconButton("D://boyi//Qt6AmazonAI//images//mode.png", QPoint(10, 10));
    connect(modeButton, &QToolButton::clicked, this, &Board::showModeDialog);
}

void Board::showModeDialog()
{
    // 创建选择对战模式对话框
    QMessageBox msgBox;
    msgBox.setText("请选择对战模式");
    msgBox.addButton("人人对战", QMessageBox::AcceptRole);
    msgBox.addButton("人机对战", QMessageBox::AcceptRole);
    msgBox.addButton("AI对战", QMessageBox::AcceptRole);
    msgBox.addButton("悔棋", QMessageBox::AcceptRole);
    int choice = msgBox.exec();
    if (choice == 0) {
        mode=1;
        // 选择了人人对战
        showPlayerOptions();
    } else if (choice == 1) {
        // 选择了人机对战
        mode=0;
        showPlayerOptions();
        // 执行相应操作
    }else if(choice==2){
        mode=-1;
        firsthand=BLACKCHESS;
        isStart=true;
    }else if(choice==3){
        regret(aBoard);
    }
}


void Board::showPlayerOptions()
{
    // 创建选择先手或后手对话框
    QMessageBox msgBox;
    msgBox.setText("请选择先手或后手");
    msgBox.addButton("先手", QMessageBox::AcceptRole);
    msgBox.addButton("后手", QMessageBox::AcceptRole);
    int choice = msgBox.exec();
    if (choice == 0) {
        // 选择了先手
        // 执行相应操作
        firsthand=BLACKCHESS;
        isStart=true;
        tmode=0;
    } else if (choice == 1) {
        // 选择了后手
        // 执行相应操作
        firsthand=WHITECHESS;
        tmode=1;
        isStart=true;
    }
}

//kingMove的值是指棋子用kingMove走法到Board[i][j]的最少步数，等于0时为不可到达，所有KingMove的值构成KingMove棋盘
void Board::kingMove() {
    int i, j; //索引变量
    //初始化KingMove棋盘
    for (i = 0; i < 12; i++)
    {
        for (j = 0; j < 12; j++)
        {
            kingMoveBlack[i][j] = 0;
            kingMoveWhite[i][j] = 0;
        }
    }
    //遍历棋盘,用kingMoveBlack棋盘存储KingMove走法,这是针对一步就可到达的情况
    for (i = 1; i <=10; i++)
    {
        for (j = 1; j <=10; j++)
        {
            //如果是黑棋，就存储在黑棋的KingMove棋盘
            if (virtualBoard[i][j] == BLACKCHESS)
            {
                if (virtualBoard[i][j - 1] == EMPTY)//正左方
                {
                    kingMoveBlack[i][j - 1] = 1;
                }
                if (virtualBoard[i - 1][j - 1] == EMPTY)//左上方
                {
                    kingMoveBlack[i - 1][j - 1] = 1;
                }
                if (virtualBoard[i - 1][j] == EMPTY)//正上方
                {
                    kingMoveBlack[i - 1][j] = 1;
                }
                if (virtualBoard[i - 1][j + 1] == EMPTY)//右上方
                {
                    kingMoveBlack[i - 1][j + 1] = 1;
                }
                if (virtualBoard[i][j + 1] == EMPTY)//正右方
                {
                    kingMoveBlack[i][j + 1] = 1;
                }
                if (virtualBoard[i + 1][j + 1] == EMPTY)//右下方
                {
                    kingMoveBlack[i + 1][j + 1] = 1;
                }
                if (virtualBoard[i + 1][j] == EMPTY)//正下方
                {
                    kingMoveBlack[i + 1][j] = 1;
                }
                if (virtualBoard[i + 1][j - 1] == EMPTY)//左下方
                {
                    kingMoveBlack[i + 1][j - 1] = 1;
                }
            }
            if (virtualBoard[i][j] == WHITECHESS)//遍历棋盘找到白棋，用kingMoveWhite棋盘存储KingMove走法
            {
                if (virtualBoard[i][j - 1] == EMPTY)//正左方
                {
                    kingMoveWhite[i][j - 1] = 1;
                }
                if (virtualBoard[i - 1][j - 1] == EMPTY)//左上方
                {
                    kingMoveWhite[i - 1][j - 1] = 1;
                }
                if (virtualBoard[i - 1][j] == EMPTY)//正上方
                {
                    kingMoveWhite[i - 1][j] = 1;
                }
                if (virtualBoard[i - 1][j + 1] == EMPTY)//右上方
                {
                    kingMoveWhite[i - 1][j + 1] = 1;
                }
                if (virtualBoard[i][j + 1] == EMPTY)//正右方
                {
                    kingMoveWhite[i][j + 1] = 1;
                }
                if (virtualBoard[i + 1][j + 1] == EMPTY)//右下方
                {
                    kingMoveWhite[i + 1][j + 1] = 1;
                }
                if (virtualBoard[i + 1][j] == EMPTY)//正下方
                {
                    kingMoveWhite[i + 1][j] = 1;
                }
                if (virtualBoard[i + 1][j - 1] == EMPTY)//左下方
                {
                    kingMoveWhite[i + 1][j - 1] = 1;
                }
            }
        }
    }
    //针对一步不可到达的情况
    for (int n = 1; n < 11; n++)//棋子在棋盘上通过KingMove走法最多是10步
    {
        for (i = 1; i <= 10; i++)
        {
            for (j = 1; j <= 10; j++)
            {
                //找到黑棋KingMove对应的步法数
                if (kingMoveBlack[i][j] == n)
                {
                    if (virtualBoard[i][j - 1] == EMPTY && kingMoveBlack[i][j - 1] == 0)
                    {
                        kingMoveBlack[i][j - 1] = n + 1;
                    }
                    if (virtualBoard[i - 1][j - 1] == EMPTY&&kingMoveBlack[i - 1][j - 1] == 0)
                    {
                        kingMoveBlack[i - 1][j - 1] = n + 1;
                    }
                    if (virtualBoard[i - 1][j] == EMPTY&&kingMoveBlack[i - 1][j] == 0)
                    {
                        kingMoveBlack[i - 1][j] = n + 1;
                    }
                    if (virtualBoard[i - 1][j + 1] == EMPTY&&kingMoveBlack[i - 1][j + 1] == 0)
                    {
                        kingMoveBlack[i - 1][j + 1] = n + 1;
                    }
                    if (virtualBoard[i][j + 1] == EMPTY&&kingMoveBlack[i][j + 1] == 0)
                    {
                        kingMoveBlack[i][j + 1] = n + 1;
                    }
                    if (virtualBoard[i + 1][j + 1] == EMPTY&&kingMoveBlack[i + 1][j + 1] == 0)
                    {
                        kingMoveBlack[i + 1][j + 1] = n + 1;
                    }
                    if (virtualBoard[i + 1][j] == EMPTY&&kingMoveBlack[i + 1][j] == 0)
                    {
                        kingMoveBlack[i + 1][j] = n + 1;
                    }
                    if (virtualBoard[i + 1][j - 1] == EMPTY&&kingMoveBlack[i + 1][j - 1] == 0)
                    {
                        kingMoveBlack[i + 1][j - 1] = n + 1;
                    }
                }
                //找到白棋KingMove对应的步法数
                if (kingMoveWhite[i][j] == n)
                {
                    if (virtualBoard[i][j - 1] == EMPTY && kingMoveWhite[i][j - 1] == 0)
                    {
                        kingMoveWhite[i][j - 1] = n + 1;
                    }
                    if (virtualBoard[i - 1][j - 1] == EMPTY&&kingMoveWhite[i - 1][j - 1] == 0)
                    {
                        kingMoveWhite[i - 1][j - 1] = n + 1;
                    }
                    if (virtualBoard[i - 1][j] == EMPTY&&kingMoveWhite[i - 1][j] == 0)
                    {
                        kingMoveWhite[i - 1][j] = n + 1;
                    }
                    if (virtualBoard[i - 1][j + 1] == EMPTY&&kingMoveWhite[i - 1][j + 1] == 0)
                    {
                        kingMoveWhite[i - 1][j + 1] = n + 1;
                    }
                    if (virtualBoard[i][j + 1] == EMPTY&&kingMoveWhite[i][j + 1] == 0)
                    {
                        kingMoveWhite[i][j + 1] = n + 1;
                    }
                    if (virtualBoard[i + 1][j + 1] == EMPTY&&kingMoveWhite[i + 1][j + 1] == 0)
                    {
                        kingMoveWhite[i + 1][j + 1] = n + 1;
                    }
                    if (virtualBoard[i + 1][j] == EMPTY&&kingMoveWhite[i + 1][j] == 0)
                    {
                        kingMoveWhite[i + 1][j] = n + 1;
                    }
                    if (virtualBoard[i + 1][j - 1] == EMPTY&&kingMoveWhite[i + 1][j - 1] == 0)
                    {
                        kingMoveWhite[i + 1][j - 1] = n + 1;
                    }
                }
            }
        }
    }
}

//queenMove的值是指棋子用queenMove走法到Board[i][j]的最少步数，等于0时为不可到达，所有queenMove的值构成QueenMove棋盘
void Board::queenMove() {
    int i, j, k, l;//索引变量
    //对queenmove棋盘初始化
    for (i = 0; i < 12; i++)
    {
        for (j = 0; j < 12; j++)
        {
            queenMoveBlack[i][j] = 0;
            queenMoveWhite[i][j] = 0;
        }
    }
    for (i = 0; j <12; k++)
    {
        for (i = 0; j <12; l++)
        {
            if (virtualBoard[i][j] == BLACKCHESS)//黑棋的Queenmove
            {
                k = i;
                l = j;
                //棋子下方
                while (virtualBoard[k][l - 1] == EMPTY)
                {
                    queenMoveBlack[k][l - 1] = 1;
                    l--;
                }
                k = i;
                l = j;
                //棋子左下方
                while (virtualBoard[k - 1][l - 1] == EMPTY)
                {
                    queenMoveBlack[k- 1][l - 1] = 1;
                    k--;
                    l--;
                }
                k = i;
                l = j;
                //棋子左方
                while (virtualBoard[k - 1][l] == EMPTY)
                {
                    queenMoveBlack[k - 1][l] = 1;
                    k--;
                }
                k = i;
                l = j;
                //棋子左上方
                while (virtualBoard[k - 1][l + 1] == EMPTY)
                {
                    queenMoveBlack[k - 1][l + 1] = 1;
                    k--;
                    l++;
                }
                k = i;
                l = j;
                //棋子上方
                while (virtualBoard[k][l + 1] == EMPTY)
                {
                    queenMoveBlack[k][l + 1] = 1;
                    l++;
                }
                k = i;
                l = j;
                //棋子右上方
                while (virtualBoard[k + 1][l + 1] == EMPTY)
                {
                    queenMoveBlack[k + 1][l + 1] = 1;
                    k++;
                    l++;
                }
                k = i;
                l = j;
                //妻子右方
                while (virtualBoard[k + 1][l] == EMPTY)
                {
                    queenMoveBlack[k + 1][l] = 1;
                    k++;
                }
                k = i;
                l = j;
                //棋子右下方
                while (virtualBoard[k + 1][l - 1] == EMPTY)
                {
                    queenMoveBlack[k + 1][l - 1] = 1;
                    k++;
                    l--;
                }
            }

            //白棋的QueenMove
            if (virtualBoard[i][j] == WHITECHESS)
            {
                k = i;
                l = j;
                while (virtualBoard[k][l - 1] == EMPTY)
                {
                    queenMoveWhite[k][l - 1] = 1;
                    l--;
                }
                k = i;
                l = j;
                while (virtualBoard[k - 1][l - 1] == EMPTY)
                {
                    queenMoveWhite[k - 1][l - 1] = 1;
                    i--;
                    l--;
                }
                k = i;
                l = j;
                while (virtualBoard[k - 1][l] == EMPTY)
                {
                    queenMoveWhite[k - 1][l] = 1;
                    i--;
                }
                k = i;
                l = j;
                while (virtualBoard[k - 1][l + 1] == EMPTY)
                {
                    queenMoveWhite[k - 1][l + 1] = 1;
                    i--;
                    l++;
                }
                k = i;
                l = j;
                while (virtualBoard[k][l + 1] == EMPTY)
                {
                    queenMoveWhite[k][l + 1] = 1;
                    l++;
                }
                k = i;
                l = j;
                while (virtualBoard[k + 1][l + 1] == EMPTY)
                {
                    queenMoveWhite[k + 1][l + 1] = 1;
                    k++;
                    l++;
                }
                k = i;
                l = j;
                while (virtualBoard[k + 1][l] == EMPTY)
                {
                    queenMoveWhite[k + 1][l] = 1;
                    k++;
                }
                k = i;
                l = j;
                while (virtualBoard[k + 1][l - 1] == EMPTY)
                {
                    queenMoveWhite[k + 1][l - 1] = 1;
                    k++;
                    l--;
                }
            }
        }
    }
    for (int n = 1; n < 6; n++)
    {
        for (i = 0 ; i < 12; i++)
        {
            for (j = 0; j<12; j++)
            {
                if (queenMoveBlack[i][j] == n)
                {
                    k = i;
                    l = j;
                    while (virtualBoard[k][l - 1] == EMPTY&&queenMoveBlack[k][l - 1] == 0)
                    {
                        queenMoveBlack[k][l - 1] = n + 1;
                        l--;
                    }
                    k = i;
                    l = j;
                    while (virtualBoard[k - 1][l - 1] == EMPTY&&queenMoveBlack[k - 1][l - 1] == 0)
                    {
                        queenMoveBlack[k - 1][l - 1] = n + 1;
                        k--;
                        l--;
                    }
                    while (virtualBoard[k - 1][l] == EMPTY&&queenMoveBlack[k - 1][l] == 0)
                    {
                        queenMoveBlack[k - 1][l] = n + 1;
                        k--;
                    }
                    k = i;
                    l = j;
                    while (virtualBoard[k - 1][l + 1] == EMPTY&&queenMoveBlack[k - 1][l + 1] == 0)
                    {
                        queenMoveBlack[k - 1][l + 1] = n + 1;
                        k--;
                        j++;
                    }
                    k = i;
                    l = j;
                    while (virtualBoard[k][l + 1] == EMPTY&&queenMoveBlack[k][l + 1] == 0)
                    {
                        queenMoveBlack[k][l + 1] = n + 1;
                        j++;
                    }
                    k = i;
                    l = j;
                    while (virtualBoard[k + 1][l + 1] == EMPTY&&queenMoveBlack[k + 1][l + 1] == 0)
                    {
                        queenMoveBlack[k + 1][l + 1] = n + 1;
                        k++;
                        j++;
                    }
                    k = i;
                    l = j;
                    while (virtualBoard[k + 1][l] == EMPTY&&queenMoveBlack[k + 1][l] == 0)
                    {
                        queenMoveBlack[k + 1][l] = n + 1;
                        k++;
                    }
                    k = i;
                    l = j;
                    while (virtualBoard[k + 1][l - 1] == EMPTY&&queenMoveBlack[k + 1][l - 1] == 0)
                    {
                        queenMoveBlack[k + 1][l - 1] = n + 1;
                        k++;
                        l--;
                    }
                }
                if (queenMoveWhite[i][j] == n)//白棋
                {
                    k = i;
                    l = j;
                    while (virtualBoard[k][l - 1] == EMPTY&&queenMoveWhite[k][l - 1] == 0)
                    {
                        queenMoveWhite[k][l - 1] = n + 1;
                        l--;
                    }k = i;
                    l = j;
                    while (virtualBoard[k - 1][l - 1] == EMPTY&&queenMoveWhite[k - 1][l - 1] == 0)
                    {
                        queenMoveWhite[k - 1][l - 1] = n + 1;
                        k--;
                        l--;
                    }
                    k=i;
                    l=j;
                    while (virtualBoard[k - 1][l] == EMPTY&&queenMoveWhite[k - 1][l] == 0)
                    {
                        queenMoveWhite[k - 1][l] = n + 1;
                        k--;
                    }
                    k = i;
                    l = j;
                    while (virtualBoard[k - 1][l + 1] == EMPTY&&queenMoveWhite[k - 1][l + 1] == 0)
                    {
                        queenMoveWhite[k - 1][l + 1] = n + 1;
                        k--;
                        j++;
                    }
                    k = i;
                    l = j;
                    while (virtualBoard[k][l + 1] == EMPTY&&queenMoveWhite[k][l + 1] == 0)
                    {
                        queenMoveWhite[k][l + 1] = n + 1;
                        j++;
                    }
                    k = i;
                    l = j;
                    while (virtualBoard[k + 1][l + 1] == EMPTY&&queenMoveWhite[k + 1][l + 1] == 0)
                    {
                        queenMoveWhite[k + 1][l + 1] = n + 1;
                        k++;
                        j++;
                    }
                    k = i;
                    l = j;
                    while (virtualBoard[k + 1][l] == EMPTY&&queenMoveWhite[k + 1][l] == 0)
                    {
                        queenMoveWhite[k + 1][l] = n + 1;
                        k++;
                    }
                    k = i;
                    l = j;
                    while (virtualBoard[k + 1][l - 1] == EMPTY&&queenMoveWhite[k + 1][l - 1] == 0)
                    {
                        queenMoveWhite[k + 1][l - 1] = n + 1;
                        k++;
                        l--;
                    }
                }
            }
        }
    }
}

//t1>0时，黑棋占优
void Board::t1Cal() {
    t1 = 0;//先归零
    int i, j;
    for (i = 1; i < 11; i++)
    {
        for (j = 1; j < 11; j++)
        {
            isStable=true;
            if (virtualBoard[i][j] == EMPTY)//扫描棋盘，通过queenmove对棋盘上的空格的控制权进行比较
            {
                if (queenMoveBlack[i][j] == queenMoveWhite[i][j] != 0)//对该空格的控制权相等且双方都能到达
                {
                    t1 = t1 + 0.08;   //这时对该空格的控制权在于行棋方，该值在-1到1之间，代表行棋方的优势，可以进行调整。黑方先行
                }
                if (queenMoveWhite[i][j] < queenMoveBlack[i][j])//白方到达该空格的步数更少
                {
                    /*if (queenMoveWhite[i][j] == 0)   //白方进不去
                    {
                        t1 = t1 + 1;     //给黑方加一分
                    }*/
                    if (queenMoveWhite[i][j] != 0)
                    {
                        t1 = t1 - 1;
                    }
                }
                if (queenMoveWhite[i][j] > queenMoveBlack[i][j])    //黑方到达该空格的步数更少
                {
                    /*if (queenMoveBlack[i][j] == 0)//黑方进不去
                    {
                        t1 = t1 - 1;  //给黑方减一分
                    }*/
                    if (queenMoveBlack[i][j] != 0)
                    {
                        t1 = t1 + 1;
                    }
                }
                if(queenMoveWhite[i][j]!=0&&queenMoveBlack[i][j]!=0)
                    isStable=false;
            }
        }
    }
}

//t2>0时,黑棋占优
void Board::t2Cal() {
    t2 = 0;
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 12; j++) {
            if (virtualBoard[i][j] == EMPTY) {
                if (kingMoveBlack[i][j] == kingMoveWhite[i][j] != 0) {
                    t2 = t2 + 0.08;
                }
                if (kingMoveWhite[i][j] < kingMoveBlack[i][j])//白方到达该空格步数更少
                {
                    if (kingMoveWhite[i][j] == 0) {
                        t2 = t2 + 1;
                    }
                    if (kingMoveWhite[i][j] != 0) {
                        t2 = t2 - 1;
                    }
                }
                if (kingMoveBlack[i][j] < kingMoveWhite[i][j])//黑方到达该空格的步数更少
                {
                    if (kingMoveBlack[i][j] == 0)//黑方进不去
                    {
                        t2 = t2 - 1;
                    }
                    if (kingMoveBlack[i][j] != 0) {
                        t2 = t2 + 1;
                    }
                }
            }
        }
    }
}

void Board::pos1Cal() {
    p1 = 0;
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            if (virtualBoard[i][j] == EMPTY)
            {
                if (queenMoveBlack[i][j] != 0 && queenMoveWhite[i][j] != 0)
                {
                    p1 = p1 + 2*(pow((double)2, -(queenMoveBlack[i][j])) - pow((double)2, -(queenMoveWhite[i][j])));
                }
                if (queenMoveBlack[i][j] != 0 && queenMoveWhite[i][j] == 0)
                {
                    p1 = p1 + 2*(pow((double)2, -(queenMoveBlack[i][j])));
                }
                if (queenMoveBlack[i][j] == 0 && queenMoveWhite[i][j] != 0)
                {
                    p1 = p1 - 2*(pow((double)2, -(queenMoveWhite[i][j])));
                }
            }
        }
    }
}

void Board::pos2Cal() {
    p2 = 0;
    double temp = 0;
    for (int i = 1; i < 11; i++)
    {
        for (int j = 1; j < 11; j++)
        {
            if (virtualBoard[i][j] == EMPTY)
            {
                if (kingMoveBlack[i][j] != 0 && kingMoveWhite[i][j] != 0)
                {
                    temp = kingMoveWhite[i][j] - kingMoveBlack[i][j];
                    p2 = p2 + min(1.0, (double)max(-1.0, temp / 6.0));
                }
                if (kingMoveBlack[i][j] != 0 && kingMoveWhite[i][j] == 0)
                {
                    p2 = p2 + 1;
                }
                if (kingMoveBlack[i][j] == 0 && kingMoveWhite[i][j] != 0)
                {
                    p2 = p2 - 1;
                }
            }
        }
    }
}

void Board::mobilityCal() {
    int i, j;
    for ( i = 0; i < 12; i++)
    {
        for ( j = 0; j < 12; j++)
        {
            mobility[i][j] = 0;
        }
    }
    for (i = 1; i < 11; i++)
    {
        for (j = 1; j < 11; j++)
        {
            if (virtualBoard[i][j] == EMPTY)
            {
                if (virtualBoard[i][j - 1] == EMPTY)
                    mobility[i][j] += 1;
                if (virtualBoard[i - 1][j - 1] == EMPTY)
                    mobility[i][j] += 1;
                if (virtualBoard[i - 1][j] == EMPTY)
                    mobility[i][j] += 1;
                if (virtualBoard[i - 1][j + 1] == EMPTY)
                    mobility[i][j] += 1;
                if (virtualBoard[i][j + 1] == EMPTY)
                    mobility[i][j] += 1;
                if (virtualBoard[i + 1][j + 1] == EMPTY)
                    mobility[i][j] += 1;
                if (virtualBoard[i + 1][j] == EMPTY)
                    mobility[i][j] += 1;
                if (virtualBoard[i + 1][j - 1] == EMPTY)
                    mobility[i][j] += 1;
            }
        }
    }
}

void Board::chessMobility() {
    int k, l, m, i, j;
    int n = 0;
    int h = 0;
    //存储每个棋子通过queenMove获得的灵活度
    allWhite[4] = { 0 };
    allBlack[4] = { 0 };
    for (i = 1; i < 11; i++)
    {
        for (j = 1; j < 11; j++)
        {
            if (virtualBoard[i][j] == BLACKCHESS)//找到一个黑子
            {
                k = i;
                l = j;
                m = 1;
                while (virtualBoard[k][l - 1] == EMPTY&&queenMoveBlack[k][l - 1] != 0)
                {
                    allBlack[n] = allBlack[n] + mobility[k][l - 1]/m;
                    l--;
                    m=m+1;
                }
                k = i;
                l = j;
                m = 1;
                while (virtualBoard[k - 1][l - 1] == EMPTY&&queenMoveBlack[k - 1][l - 1] != 0)
                {
                    allBlack[n] = allBlack[n] + mobility[k - 1][l - 1] / m;
                    k--;
                    l--;
                    m = m + 1;
                }
                k = i;
                l = j;
                m = 1;
                while (virtualBoard[k - 1][l] == EMPTY&&queenMoveBlack[k - 1][l] != 0)
                {
                    allBlack[n] = allBlack[n] + mobility[k - 1][l] / m;
                    k--;
                    m = m + 1;
                }
                k = i;
                l = j;
                m = 1;
                while (virtualBoard[k - 1][l + 1] == EMPTY&&queenMoveBlack[k - 1][l + 1] != 0)
                {
                    allBlack[n] = allBlack[n] + mobility[k - 1][l + 1] / m;
                    k--;
                    l++;
                    m = m + 1;
                }
                k = i;
                l = j;
                m = 1;
                while (virtualBoard[k][l + 1] == EMPTY&&queenMoveBlack[k][l + 1] != 0)
                {
                    allBlack[n] = allBlack[n] + mobility[k][l + 1] / m;
                    l++;
                    m = m + 1;
                }
                k = i;
                l = j;
                m = 1;
                while (virtualBoard[k + 1][l + 1] == EMPTY&&queenMoveBlack[k + 1][l + 1] != 0)
                {
                    allBlack[n] = allBlack[n] + mobility[k + 1][l + 1] / m;
                    k++;
                    l++;
                    m = m + 1;
                }
                k = i;
                l = j;
                m = 1;
                while (virtualBoard[k + 1][l] == EMPTY&&queenMoveBlack[k + 1][l] != 0)
                {
                    allBlack[n] = allBlack[n] + mobility[k + 1][l] / m;
                    k++;
                    m = m + 1;
                }
                k = i;
                l = j;
                m = 1;
                while (virtualBoard[k + 1][l - 1] == EMPTY&&queenMoveBlack[k + 1][l - 1] != 0)
                {
                    allBlack[n] = allBlack[n] + mobility[k + 1][l - 1] / m;
                    k++;
                    l--;
                    m = m + 1;
                }
                n++;
            }
            if (virtualBoard[i][j] == WHITECHESS)//找到白棋
            {
                k = i;
                l = j;
                m = 1;
                while (virtualBoard[k][l - 1] == EMPTY&&queenMoveWhite[k][l - 1] != 0)
                {
                    allWhite[h] = allWhite[h] + mobility[k][l - 1] / m;
                    l--;
                    m = m + 1;
                }
                k = i;
                l = j;
                m = 1;
                while (virtualBoard[k - 1][l - 1] == EMPTY&&queenMoveWhite[k - 1][l - 1] != 0)
                {
                    allWhite[h] = allWhite[h] + mobility[k - 1][l - 1] / m;
                    k--;
                    l--;
                    m = m + 1;
                }
                k = i;
                l = j;
                m = 1;
                while (virtualBoard[k - 1][l] == EMPTY&&queenMoveWhite[k - 1][l] != 0)
                {
                    allWhite[h] = allWhite[h] + mobility[k - 1][l] / m;
                    k--;
                    m = m + 1;
                }
                k = i;
                l = j;
                m = 1;
                while (virtualBoard[k - 1][l + 1] == EMPTY&&queenMoveWhite[k - 1][l + 1] != 0)
                {
                    allWhite[h] = allWhite[h] + mobility[k - 1][l + 1] / m;
                    k--;
                    l++;
                    m = m + 1;
                }
                k = i;
                l = j;
                m = 1;
                while (virtualBoard[k][l + 1] == EMPTY&&queenMoveWhite[k][l + 1] != 0)
                {
                    allWhite[h] = allWhite[h] + mobility[k][l + 1] / m;
                    l++;
                    m = m + 1;
                }
                k = i;
                l = j;
                m = 1;
                while (virtualBoard[k + 1][l + 1] == EMPTY&&queenMoveWhite[k + 1][l + 1] != 0)
                {
                    allWhite[h] = allWhite[h] + mobility[k + 1][l + 1] / m;
                    k++;
                    l++;
                    m = m + 1;
                }
                k = i;
                l = j;
                m = 1;
                while (virtualBoard[k + 1][l] == EMPTY&&queenMoveWhite[k + 1][l] != 0)
                {
                    allWhite[h] = allWhite[h] + mobility[k + 1][l] / m;
                    k++;
                    m = m + 1;
                }
                k = i;
                l = j;
                m = 1;
                while (virtualBoard[k + 1][l - 1] == EMPTY&&queenMoveWhite[k + 1][l - 1] != 0)
                {
                    allWhite[h] = allWhite[h] + mobility[k + 1][l - 1] / m;
                    k++;
                    l--;
                    m = m + 1;
                }
                h++;
            }
        }
    }
}

double Board::evaluateValue() {
    double value;
    //分开局，中局，残局，权重因子不同
    if(turn<20){
        a=0.14,b=0.37,c=0.13,d=0.13,e=0.20;
    }else if(turn>=20&&turn<50){
        a=0.3,b=0.25,c=0.20,d=0.20,e=0.05;
    }else if(turn>=50){
        a=0.80,b=0.10,c=0.05,d=0.05,e=0;
    }

    kingMove();
    queenMove();
    t1Cal();
    t2Cal();
    pos1Cal();
    pos2Cal();
    mobilityCal();
    chessMobility();
    for(int i=0;i<4;i++){
        //totalm>0时，黑子灵活度占优
        totalm+=(allBlack[i]-allWhite[i])/30;
    }
    if(isStable) return t1;
    else return a*t1+b*t2+c*p1+d*p2+e*totalm;
}

void Board::searchGoodMove(int Board[12][12]){
    memcpy(virtualBoard, Board, 144 * sizeof(int));   //复制传入的Board棋盘到virtualBoard
    if(MoveCount>150){
        searchDepth=2;
    }
    if(MoveCount>40&&MoveCount<=150){
        searchDepth=3;
    }
    if(MoveCount>20&&MoveCount<=40){
        searchDepth=4;
    }
    if(MoveCount>10&&MoveCount<=20){
        searchDepth=5;
    }
    if(MoveCount>0&&MoveCount<=10){
        searchDepth=6;
    }
    if(tmode==0){
        AlphaBeta(searchDepth);
    }
    else if(tmode==1){
        PVS(searchDepth);
    }
    makeMove(&BestMove);
    memcpy(Board, virtualBoard, 144 * sizeof(int));//将VirtualBoard的结果传递给实际的Board
    step = 0;//重置AI的步进数
    cout<<"evaluate value:"<<evaluateValue()<<endl;
    cout<<"move count:"<<MoveCount<<endl;
}

double Board::PVS(int depth, double alpha, double beta)
{
    double score = 0;   // 定义评估分数
    int type = 0;
    if (JudgeWin(virtualBoard) != -2) // 分出胜负
    {
        return evaluateValue(); // 返回评估值
    }
    if (depth == 0) // 到达叶子节点
    {
        if ((searchDepth - depth) % 2 == 0) // 极大值结点
        {
            if (tmode == 1)
            {
                return evaluateValue();
            }
            if (tmode == 0)
            {
                return -evaluateValue();
            }
        }
        else
        {
            if (tmode == 1)
            {
                return -evaluateValue();
            }
            if (tmode == 0)
            {
                return evaluateValue();
            }
        }
    }
    /////////// 先后手转换 ///////////
    step = step + 1; // 每搜索一层就是棋局进行了一步
    if (tmode == 1) // 电脑先手，电脑执黑
    {
        if (step % 2 == 1) // 奇数层
        {
            type = BLACKCHESS;
        }
        else if (step % 2 == 0)
        {
            type = WHITECHESS;
            // MessageBox(NULL, "测试！", "提示", MB_OK);
        }
    }
    if (tmode == 0) // 电脑后手
    {
        if (step % 2 == 0) // 奇数层
        {
            type = BLACKCHESS;
        }
        else if (step % 2 == 1) // 偶数层
        {
            type = WHITECHESS;
        }
    }
    /////////////////////////////
    int count = possibleChess(virtualBoard, depth, type); // 记录步法总数，生成所有符合规则的步法
    for (int i = 0; i < count; i++)
    {
        if (i == 0)
        {
            makeMove(&MoveList[depth][i]); // 模拟行棋，在virtualBoard上模拟
            score = -PVS(depth - 1, -beta, -alpha); // 递归向下搜索
            UnMakeMove(&MoveList[depth][i]); // 撤销模拟行棋
        }
        else
        {
            makeMove(&MoveList[depth][i]); // 模拟行棋，在virtualBoard上模拟
            score = -PVS(depth - 1, -alpha - 1, -alpha); // 递归向下搜索，使用窗口[-alpha-1, -alpha]
            if (score > alpha && score < beta) // 进行判断窗口是否需要调整
            {
                score = -PVS(depth - 1, -beta, -score); // 递归向下搜索，使用窗口[-beta, -score]
            }
            UnMakeMove(&MoveList[depth][i]); // 撤销模拟行棋
        }
        if (score > alpha)
        {
            alpha = score;
            if (depth == searchDepth)
            {
                BestMove = MoveList[depth][i];
            }
        }
        if (alpha >= beta)
        {
            break;
        }
    }
    return alpha;
}

double Board::AlphaBeta(int depth, double alpha, double beta)
{
    double score = 0;   // Define evaluation score
    int type = 0;
    if (JudgeWin(virtualBoard) != -2) // Determine the game result
    {
        return evaluateValue(); // Return the evaluation value
    }
    if (depth == 0) // Reached leaf node
    {
        if ((searchDepth - depth) % 2 == 0) // Maximize node
        {
            if (tmode == 1)
            {
                return evaluateValue();
            }
            if (tmode == 0)
            {
                return -evaluateValue();
            }
        }
        else
        {
            if (tmode == 1)
            {
                return -evaluateValue();
            }
            if (tmode == 0)
            {
                return evaluateValue();
            }
        }
    }

    /////////// Change player's turn ///////////
    step = step + 1; // Each search level means one move has been made in the game
    if (tmode == 1) // Computer plays first (black)
    {
        if (step % 2 == 1) // Odd level
        {
            type = BLACKCHESS;
        }
        else if (step % 2 == 0)
        {
            type = WHITECHESS;
        }
    }
    if (tmode == 0) // Computer plays second
    {
        if (step % 2 == 0) // Even level
        {
            type = BLACKCHESS;
        }
        else if (step % 2 == 1)
        {
            type = WHITECHESS;
        }
    }
    /////////////////////////////

    int count = possibleChess(virtualBoard, depth, type); // Record the number of possible moves and generate all valid moves
    for (int i = 0; i < count; i++)
    {
        makeMove(&MoveList[depth][i]); // Make a move on the virtualBoard
        score = -AlphaBeta(depth - 1, -beta, -alpha); // Recursive search
        UnMakeMove(&MoveList[depth][i]); // Undo the move

        if (score >= beta)
        {
            return beta; // Beta cutoff
        }

        if (score > alpha)
        {
            alpha = score;
            if (depth == searchDepth)
            {
                BestMove = MoveList[depth][i];
            }
        }
    }
    return alpha;
}

//寻找所有可行的移动方式
int Board::possibleChess(int Board[12][12], int depth, int Side)
{
    int i, j, k, l, t;
    int Position[12][12] = { 0 };   //临时存储的棋盘，防止对原棋盘误操作
    initOffset();       //调用偏移量
    memcpy(Position, Board, 144 * sizeof(int));   //复制棋盘信息
    MoveCount = 0;  //计步器清零
    for ( i = 1; i <= 10; i++)
    {
        for ( j = 1; j <= 10; j++)
        {
            if (Position[i][j] == Side)//找到棋子
            {
                k = i;
                l = j;
                for ( t = 0; t < 8; t++)//该棋子的周围八个方向进行偏移
                {
                    //尝试每个偏移路径
                    k += Offset[t][0];//进行偏移
                    l += Offset[t][1];
                    while (Position[k][l] == EMPTY)
                    {
                        possibleBar(Position, depth, Side, i, j, k, l);
                        ////继续沿该路径偏移
                        k += Offset[t][0];
                        l += Offset[t][1];
                    }
                    k = i;//恢复棋子坐标，为下一个方向的偏移做准备
                    l = j;
                }
            }
        }
    }
    return MoveCount;//返回生成的步法总数
}

void Board::initOffset()
{
    Offset[0][0] = 0; Offset[0][1] = -1;//正左方偏移，依次类推
    Offset[1][0] = -1; Offset[1][1] = -1;
    Offset[2][0] = -1; Offset[2][1] = 0;
    Offset[3][0] = -1; Offset[3][1] = 1;
    Offset[4][0] = 0; Offset[4][1] = 1;
    Offset[5][0] = 1; Offset[5][1] = 1;
    Offset[6][0] = 1; Offset[6][1] = 0;
    Offset[7][0] = 1; Offset[7][1] = -1;
}

void Board::possibleBar(int Board[12][12], int depth, int Side, int startX, int startY, int endX, int endY)
{
    int k, l;
    //模拟行棋落子的过程，便于对放置障碍判断
    Board[startX][startY] = EMPTY;
    Board[endX][endY] = Side;
    k = endX;
    l = endY;
    for (int t = 0; t < 8; t++)
    {
        k += Offset[t][0];
        l += Offset[t][1];
        while (Board[k][l] == EMPTY)
        {
            //存放步法
            MoveList[depth][MoveCount].start={QPoint(startX,startY)};//存放起始位置坐标
            MoveList[depth][MoveCount].end = {QPoint(endX,endY)};//存放终点位置坐标
            MoveList[depth][MoveCount].barrier = {QPoint(k,l)};//存放障碍的坐标
            MoveList[depth][MoveCount].ID = Side;
            MoveList[depth][MoveCount].score = 0;
            //计步器计数更新
            MoveCount++;

            //继续沿该路径偏移
            k += Offset[t][0];
            l += Offset[t][1];
        }
        k = endX;//恢复坐标，为下一个方向的偏移做准备
        l = endY;
    }
    //恢复棋盘，撤销模拟的行棋
    Board[endX][endY] = EMPTY;
    Board[startX][startY] = Side;
}

//行棋模拟
void Board::makeMove(chessMove* move) {
    // 在这里编写函数的实现
    virtualBoard[move->start.x()][move->start.y()] = EMPTY;
    virtualBoard[move->end.x()][move->end.y()] = move->ID;
    virtualBoard[move->barrier.x()][move->barrier.y()] = BARRIER;
}

//撤棋模拟
void Board::UnMakeMove(chessMove* move)
{
    virtualBoard[move->barrier.x()][move->barrier.y()] = EMPTY;
    virtualBoard[move->end.x()][move->end.y()] = EMPTY;
    virtualBoard[move->start.x()][move->start.y()] = move->ID;
}

void Board::writeLog() {
    string file="D://boyi//Qt6AmazonAI//output//log.txt";
    ofstream outFile(file,std::ios::app);
    if(turn%2==0){
        outFile<<turn/2+1;
    }
    outFile<<" "<<char('a'+pace[0].x()-1)<<11-pace[0].y()<<char('a'+pace[1].x()-1)<<11-pace[1].y()
    <<"("<<char('a'+pace[2].x()-1)<<11-pace[2].y()<<")";
    if(turn%2==1)
        outFile<<endl;
}