#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include <QMouseEvent>

//宏定义
#define EMPTY 0
#define BLACKCHESS 1
#define WHITECHESS 2
#define BARRIER 3
#define INFI 9999 //无限大

extern int mode;

void doAIMove(int Board[12][12]);

extern int turn;//回合数

typedef struct domove{
    QPoint start;//起子
    QPoint end;//落子
    QPoint barrier;//障碍
    int ID;//行棋方
    double score;//这种走法的评估分数
}chessMove;

class QToolButton;
class QMessageBox;

class Board : public QWidget
{
    Q_OBJECT
public:
    Board(QWidget *parent = nullptr);
    void initBoard(int board[12][12]);//棋盘数据结构初始化
    int JudgeWin(int Board[12][12]);//判断是否胜利
    int mode=0;//默认人机对战模式
    void searchGoodMove(int Board[12][12]);//最佳路径
    int tmode;
    double AlphaBeta(int depth, double alpha=-INFI, double beta=INFI);


protected:
    void paintEvent(QPaintEvent *event) override;//绘制棋盘
    QSize sizeHint() const override;
    int aBoard[12][12]={0};//创建一个棋盘
    void mousePressEvent(QMouseEvent *event);//鼠标点击事件
    void updateBoard(int Board[12][12]);//更新棋盘
    void regret(int Board[12][12]);//悔棋
    int firsthand=BLACKCHESS;//默认先手(黑子)
    void kingMove();//计算kingMove的值
    void queenMove();//计算queenMove的值
    void t1Cal();//计算queenMove的t1特征值
    void t2Cal();//计算queenMove的t2特征值
    void pos1Cal();//计算queenmove的position特征值
    void pos2Cal();//计算kingmove的position特征值
    void mobilityCal();//mobability空格灵活度计算
    void chessMobility();//每个棋子灵活度计算
    double evaluateValue();//最终局面评估
    int step=0;//AI决策时，当前搜索到的层数，step=0为当前回合的棋局
    chessMove BestMove = { QPoint(0, 0), QPoint(0, 0), QPoint(0, 0) };//将最佳移动方式初始化
    int searchDepth=2;//设置搜索层数为3
    int Offset[8][2] = { 0 };//定义偏移量
    int MoveCount = 0;//移动次数
    chessMove MoveList[8][2500] ;
    double PVS(int depth, double alpha=-INFI, double beta=INFI);//α-β剪枝-PVS搜索，用来查找最优的走法
    void makeMove(chessMove* move);//行棋模拟
    void UnMakeMove(chessMove* move);//撤棋模拟
    void initOffset();//偏移量初始化
    int possibleChess(int Board[12][12], int depth, int Side);//可行放置棋子
    void possibleBar(int Board[12][12], int depth, int Side, int FromX, int FromY, int ToX, int ToY);//可行放置棋子
    void writeLog();//生成棋谱文档txt

private slots:
    void showModeDialog();
    void showPlayerOptions();

private:
    QPoint selectedChessPos; // 保存被选中的棋子的位置
    QPoint pace[3]={QPoint(0, 0), QPoint(0, 0), QPoint(0, 0)};;//存储棋子的步伐
    bool chessSelected; // 标记棋子是否被选中
    bool isMovingChess;//是否在移动棋子
    int clickCount=0;//鼠标点击次数
    int currentplayer;//当前玩家
    bool isValidMove(int x1, int y1, int x2, int y2, int Board[12][12]);//当前移动方式是否合法
    QToolButton* createIconButton(const QString& iconPath, const QPoint& pos);
    void initializeUI();
    bool isStart=false;//游戏是否开始
    int virtualBoard[12][12];//虚拟棋盘，用来给AI走的
    double t1,t2=0;//territory特征值
    double p1,p2=0;//position特征值
    double totalm=0;
    double a,b,c,d,e=0;
    int kingMoveBlack[12][12]={0};
    int kingMoveWhite[12][12]={0};
    int queenMoveBlack[12][12]={0};
    int queenMoveWhite[12][12]={0};
    int mobility[12][12]={0};
    int allWhite[4]={0};
    int allBlack[4]={0};
    bool isStable;
};

#endif // BOARD_H
