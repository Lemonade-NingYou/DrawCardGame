#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

// 定义棋子类型和游戏状态
enum ChessPieces 
{
    EMPTY = 0,
    BLACK = 1,  // 玩家执黑
    WHITE = 2   // 电脑执白
};

enum GameStatus 
{
    PLAYING,
    BLACK_WIN,
    WHITE_WIN,
    DRAW
};

// 棋盘大小
#define BOARD_SIZE 15

// 全局变量
enum ChessPieces board[BOARD_SIZE][BOARD_SIZE];
enum GameStatus status = PLAYING;

// 初始化棋盘
void initBoard() 
{
    for (int i = 0; i < BOARD_SIZE; i++) 
    {
        for (int j = 0; j < BOARD_SIZE; j++) 
        {
            board[i][j] = EMPTY;
        }
    }
}

// 打印棋盘
void printBoard() 
{
    printf("\n  ");
    for (int i = 0; i < BOARD_SIZE; i++) 
    {
        printf("\033[33m%2d\033[0m", i);
    }
    printf("\n");

    for (int i = 0; i < BOARD_SIZE; i++) 
    {
        printf("\033[33m%2d\033[0m",i);
        for (int j = 0; j < BOARD_SIZE; j++) 
        {
            switch (board[i][j]) 
            {
                case EMPTY: printf("\033[33m ·\033[0m"); break;
                case BLACK: printf("\033[32m ●\033[0m"); break;
                case WHITE: printf("\033[34m ○\033[0m"); break;
            }
        }
    printf("\n");
    }
}

// 检查坐标是否合法
bool isValidMove(int x, int y) 
{
    return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE && board[x][y] == EMPTY;
}

// 落子
bool makeMove(int x, int y, enum ChessPieces player) 
{
    if (!isValidMove(x, y)) 
    {
        return false;
    }
    board[x][y] = player;
    return true;
}

// 检查是否有五子连珠
bool checkWin(int x, int y, enum ChessPieces player) 
{
    int directions[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};

    for (int d = 0; d < 4; d++) 
    {
        int dx = directions[d][0];
        int dy = directions[d][1];
        int count = 1;  // 当前位置已经有一个棋子

        // 正方向检查
        for (int i = 1; i < 5; i++) 
        {
            int nx = x + i * dx;
            int ny = y + i * dy;
            if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && board[nx][ny] == player) 
            {
                count++;
            } 
            else 
            {
                break;
            }
        }
        
        // 反方向检查
        for (int i = 1; i < 5; i++) 
        {
            int nx = x - i * dx;
            int ny = y - i * dy;
            if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && board[nx][ny] == player) 
            {
                count++;
            } 
            else 
            {
                break;
            }
        }
        
        if (count >= 5) 
        {
            return true;
        }
    }

    return false;
}

// 检查棋盘是否已满
bool isBoardFull() 
{
    for (int i = 0; i < BOARD_SIZE; i++) 
    {
        for (int j = 0; j < BOARD_SIZE; j++) 
        {
            if (board[i][j] == EMPTY) 
            {
                return false;
            }
        }
    }
    return true;
}

// 评估棋盘上某个位置的分数
int evaluatePosition(int x, int y, enum ChessPieces player) {
    if (!isValidMove(x, y)) return -1; // 无效位置
    
    int score = 0;
    int directions[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
    
    // 检查四个方向
    for (int d = 0; d < 4; d++) {
        int dx = directions[d][0];
        int dy = directions[d][1];
        int count = 1;  // 当前位置的棋子
        
        // 计算该方向的连子数
        for (int i = 1; i < 5; i++) {
            int nx = x + i * dx;
            int ny = y + i * dy;
            if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && 
                board[nx][ny] == player) {
                count++;
            } else {
                break;
            }
        }
        
        for (int i = 1; i < 5; i++) {
            int nx = x - i * dx;
            int ny = y - i * dy;
            if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && 
                board[nx][ny] == player) {
                count++;
            } else {
                break;
            }
        }
        
        // 根据连子数加分（可调整权重）
        switch (count) {
            case 5: score += 10000; break;  // 五连
            case 4: score += 1000; break;   // 四连
            case 3: score += 100; break;    // 三连
            case 2: score += 10; break;     // 两连
        }
    }
    
    return score;
}

// 智能AI落子
void computerMoveSmart() {
    int bestX = -1, bestY = -1;
    int maxScore = -1;
    enum ChessPieces opponent = BLACK;
    
    // 遍历所有空位
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == EMPTY) {
                // 计算进攻得分
                int attackScore = evaluatePosition(i, j, WHITE);
                // 计算防守得分
                int defenseScore = evaluatePosition(i, j, opponent);
                
                // 综合得分（可调整权重）
                int totalScore = attackScore * 1.2 + defenseScore * 1.5;
                
                // 如果是关键位置（如活四、冲四），直接选择
                if (attackScore >= 5000 || defenseScore >= 5000) {
                    bestX = i;
                    bestY = j;
                    maxScore = totalScore;
                    break;
                }
                
                if (totalScore > maxScore) {
                    maxScore = totalScore;
                    bestX = i;
                    bestY = j;
                }
            }
        }
        // 修复：将条件判断移到内层循环外
        if (bestX != -1 && (maxScore >= 5000)) break;
    }
    
    // 落子
    if (bestX != -1 && bestY != -1) {
        makeMove(bestX, bestY, WHITE);
        printf("电脑在位置 [%d, %d] 落子\n", bestX, bestY);
        
        if (checkWin(bestX, bestY, WHITE)) {
            status = WHITE_WIN;
        } else if (isBoardFull()) {
            status = DRAW;
        }
    }
}

// 简单的AI落子（随机策略）
void computerMove() 
{
    int x, y;
    do 
    {
        x = rand() % BOARD_SIZE;
        y = rand() % BOARD_SIZE;
    } 
    while (!isValidMove(x, y));
    
    makeMove(x, y, WHITE);
    printf("电脑在位置 [%d, %d] 落子\n", x, y);

    if (checkWin(x, y, WHITE)) 
    {
        status = WHITE_WIN;
    } 
    else if (isBoardFull()) 
    {
        status = DRAW;
    }
}

int main() 
{
    srand(time(NULL));
    initBoard();
    
    printf("欢迎来到五子棋游戏！\n");
    printf("你执白棋(●)，电脑执黑棋(○)\n");
    while (status == PLAYING)
    {
        printBoard();
    
        // 玩家回合
        int x, y;
        do 
        {
            printf("请输入落子位置 (行 列): ");
            if (scanf("%d %d", &x, &y) != 2) 
            {
                printf("输入格式错误，请重新输入！\n");
                while (getchar() != '\n'); // 清除输入缓冲区
                continue;
            }
            if (!isValidMove(x, y)) 
            {
                printf("无效的位置，请重新选择！\n");
            }
        } 
        while (!isValidMove(x, y));
    
        makeMove(x, y, BLACK);
        if (checkWin(x, y, BLACK)) 
        {
            status = BLACK_WIN;
        }
        else if (isBoardFull()) 
        {
            status = DRAW;
        } 
        else 
        {
            // 电脑回合
            printf("\033[H\033[J");
            computerMoveSmart();
        }
    }
    printf("\033[H\033[J");
    printBoard();

    switch (status) 
    {
        case BLACK_WIN:
            printf("恭喜你，你赢了！\n");
            break;
        case WHITE_WIN:
            printf("很遗憾，电脑赢了！\n");
            break;
        case DRAW:
            printf("游戏\n");
            break;
        default:
            break;
    }
    return 0;
}
