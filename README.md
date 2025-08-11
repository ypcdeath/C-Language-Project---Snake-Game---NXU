# C-Language Project - Snake-Game - NXU
《贪吃蛇小游戏》 宁夏大学C语言实训项目
贪吃蛇小游戏：需求与设计及未来扩展
一、引言 
1.1 文档目的 
本文档旨在详细描述贪吃蛇游戏的需求规格和设计实现，涵盖游戏功能、系统架构、模块设计、数据结构及核心算法，为游戏的理解、维护和扩展提供依据。 
1.2 项目概述 
本项目是一款基于 Windows 控制台的经典贪吃蛇游戏，支持多难度级别、多类型食物系统和分数记录功能。游戏通过键盘控制蛇的移动方向，以吃食物增长长度并获取分数，撞到边界或自身则游戏结束。 
二、需求文档 
2.1 项目概述 
游戏名称：经典贪吃蛇游戏 
开发环境：Windows 操作系统，Visual Studio 编译器 
技术栈：C 语言，Windows API 控制台操作 
核心玩法：玩家通过方向键控制蛇移动，吃食物增长长度并得分，避免碰撞 
2.2 功能需求 
2.2.1 用户界面需求 
主菜单：包含 "开始游戏"、"难度设置"、"退出游戏" 三个选项，支持上下键选择和回车键确认 
游戏界面： 
 游戏区域：由边界包围的可移动区域 
 信息栏：显示当前分数、最高分、控制提示、难度等级和食物类型说明 
结束界面：显示游戏结束信息、最终分数，若创纪录则显示 "NEW HIGH SCORE!"，并提供重启或返回菜单选项 
2.2.2 游戏核心需求 
 蛇的行为： 
 初始长度为 3 节，初始方向向右 
 支持上下左右移动，不可直接反向移动（如向右时不能直接向左） 
 吃到食物后增长长度，长度上限为 100 节 
 长按方向键可触发加速移动 
 食物系统： 
 多种食物类型，每种食物有不同的颜色、符号、分数值和增长值 
 食物随机生成，不与蛇身或其他食物重叠 
 部分食物具有特殊效果（加速、减速等） 
 不同难度下食物生成概率不同 
 碰撞检测： 
 边界碰撞：蛇头超出游戏区域边界则游戏结束 
 自身碰撞：蛇头与蛇身任何部位重叠则游戏结束 
2.2.3 控制需求 
 方向控制：W（上）、S（下）、A（左）、D（右） 
 功能控制： 
 P 键：暂停 / 继续游戏 
 X 键：退出游戏 
 R 键：重置游戏 
2.2.4 难度系统需求 
 支持 4 种难度级别：简单（EASY）、中等（MEDIUM）、困难（HARD）、噩梦（DEVIL） 
 不同难度区别： 
 移动速度：难度越高速度越快 
 食物类型：简单难度仅 2 种食物，中等难度 4 种，困难及以上 6 种 
 生成概率：难度越高，食物生成概率越大 
2.2.5 计分与存储需求 
 分数计算：根据食物类型获得不同分数（10× 食物基础分） 
 最高分记录：自动保存历史最高分至本地文件，游戏结束时更新并显示 
2.3 非功能需求 
 性能需求：游戏运行流畅，无明显卡顿，帧率随难度动态调整 
 易用性需求：操作简单直观，界面元素清晰可辨，提供明确的操作提示 
 兼容性需求：支持 Windows 操作系统，兼容 Visual Studio 2015 及以上版本编译环境 
2.4 运行环境 
 操作系统：Windows 7/8/10/11 
 硬件要求：基本 PC 配置（支持控制台显示） 
 软件依赖：C 标准库、Windows API 
三、设计文档 
3.1 总体设计 
3.1.1 系统架构 
游戏采用模块化设计，主要包含以下模块： 
 用户界面模块：负责菜单绘制、游戏元素渲染和信息显示 
 输入处理模块：捕获键盘输入并转换为游戏指令 
 游戏逻辑模块：处理蛇的移动、食物生成、碰撞检测等核心逻辑 
 数据存储模块：负责最高分的读取和保存 
 配置模块：管理游戏难度、速度等参数配置 
3.1.2 模块交互 

[用户输入] → [输入处理模块] → [游戏逻辑模块] → [数据存储模块]   
                                                                                            ↓
[用户界面模块] ←------------------------------------------------------ 
3.2 详细设计 
3.2.1 数据结构设计 
3.2.1.1 控制台信息结构体 

typedef struct {   
    int width;    // 控制台宽度（字符数）   
    int height;   // 控制台高度（字符数）   
    int offsetX;  // X方向偏移（边框留白）   
    int offsetY;  // Y方向偏移（边框留白）   
} ConsoleInfo;   
用于存储控制台尺寸和游戏区域偏移量，辅助元素定位。 
3.2.1.2 蛇节点与蛇结构体 

// 蛇节点结构体（链表节点）   
typedef struct SnakeNode {   
    int x;                  // 节点X坐标   
    int y;                  // 节点Y坐标   
    struct SnakeNode* next; // 指向下一节点的指针   
} SnakeNode;   
// 蛇结构体（链表管理）   
typedef struct {   
    SnakeNode* head; // 链表头节点（蛇头）   
    SnakeNode* tail; // 链表尾节点（蛇尾）   
    int length;      // 蛇的长度   
    enum Direction dir; // 当前移动方向   
} Snake;   
采用单向链表实现蛇的结构，支持高效的头部插入（移动）和尾部删除（未吃食物时）操作。 
3.2.1.3 食物结构体 

typedef struct {   
    int x[MAX_FOOD];       // 食物X坐标数组   
    int y[MAX_FOOD];       // 食物Y坐标数组   
    int count;             // 该类型食物的数量   
    int score;             // 吃该食物获得的分数   
    int color;             // 食物显示颜色   
    char symbol;           // 食物显示符号   
    int growth;            // 吃后蛇增长的节数   
    enum Effict eff;       // 食物的特殊效果   
} Food;   
支持多种食物类型，每种食物具有独立的属性（分数、颜色、增长值等）。 
3.2.1.4 游戏状态结构体 

typedef struct {   
    Snake snake;               // 蛇的信息   
    Food food[6];              // 食物数组（最多6种类型）   
    int foodTypes;             // 当前启用的食物类型数量   
    int score;                 // 当前游戏分数   
    bool gameOver;             // 游戏是否结束   
    int width;                 // 游戏区域宽度   
    int height;                // 游戏区域高度   
    int highScore;             // 历史最高分   
    int difficulty;            // 当前游戏难度   
    bool paused;               // 游戏是否暂停   
    enum Effict ef;            // 当前生效的食物效果   
    int speed;                 // 当前游戏速度   
} GameState;   
集中管理游戏的所有状态信息，包括蛇、食物、分数、难度等核心数据。 
3.2.2 核心模块设计 
3.2.2.1 初始化模块 
 函数：initGame(GameState* game, int difficulty)
 功能：初始化游戏状态，包括： 
 蛇的初始长度（3 节）和位置（屏幕中间） 
 食物类型和初始食物生成 
 分数、难度等参数设置 
 关键代码： 

// 创建初始3个节点（蛇头到蛇尾依次排列）   
int startX = game->width / 2;   
int startY = game->height / 2;   
for (int i = 0; i < 3; i++) {   
    SnakeNode* newNode = createSnakeNode(startX - i, startY);   
    if (!game->snake.head) {  // 第一个节点（头和尾都是该节点）   
        game->snake.head = newNode;   
        game->snake.tail = newNode;   
    } else {  // 后续节点（添加到尾部）   
        game->snake.tail->next = newNode;   
        game->snake.tail = newNode;   
    }   
    game->snake.length++;   
}   

3.2.2.2 输入处理模块 
 函数：input(GameState* game, enum Direction currentDir)
 功能：捕获键盘输入，控制蛇的移动方向、暂停 / 继续、退出等操作 
 核心逻辑： 
 使用_kbhit()检测按键，_getch()获取键值 
 方向键控制：防止直接反向移动（如向右时不能直接向左） 
 支持加速功能：长按方向键触发快速移动 
 关键代码： 

switch (key) {   
case 'a':  // 左移   
    if (game->snake.dir != RIGHT)   
        game->snake.dir = LEFT;   
    isSpeeding = true;  // 长按加速   
    break;   
case 'd':  // 右移   
    if (game->snake.dir != LEFT)   
        game->snake.dir = RIGHT;   
    isSpeeding = true;   
    break;   
// 其他按键处理...   
}   
3.2.2.3 游戏逻辑模块 
 函数：update(GameState* game)
 功能：处理蛇的移动、食物碰撞、长度变化和碰撞检测 
 核心流程： 
 计算新蛇头位置，创建新节点并插入链表头部 
 检测是否吃到食物：若是则增长蛇身，更新分数和食物效果；若否则删除尾部节点 
 碰撞检测：检测蛇头是否撞墙或自身 
 随机生成新食物 
 关键代码（蛇移动）： 

// 计算新蛇头位置   
int newHeadX = game->snake.head->x;   
int newHeadY = game->snake.head->y;   
switch (game->snake.dir) {   
case LEFT:  newHeadX--; break;   
case RIGHT: newHeadX++; break;   
case UP:    newHeadY--; break;   
case DOWN:  newHeadY++; break;   
}   
// 创建新蛇头节点（链表头部插入）   
SnakeNode* newHead = createSnakeNode(newHeadX, newHeadY);   
newHead->next = game->snake.head;   
game->snake.head = newHead;   
game->snake.length++;   
3.2.2.4 绘制模块 
 函数：drawBoundary(GameState* game)、drawGameElements(GameState* game, bool isSpeeding)
 功能：绘制游戏边界、蛇、食物和信息栏 
 优化策略：仅更新变化区域（如蛇尾位置用空格覆盖），减少不必要的绘制操作 
 关键代码（绘制蛇）： 

// 绘制蛇头   
setColor(isSpeeding ? RED : 4);   
setCursorPosition(console.offsetX + game->snake.head->x + 1,   
    console.offsetY + game->snake.head->y + 1);   
printf("O");   
// 绘制蛇身   
SnakeNode* current = game->snake.head ? game->snake.head->next : NULL;   
while (current) {   
    setCursorPosition(console.offsetX + current->x + 1, console.offsetY + current->y + 1);   
    printf("o");   
    current = current->next;   
}   
3.2.2.5 难度与速度控制模块 
 速度设置：根据难度和加速状态动态调整速度 
 简单 / 中等 / 困难难度：正常速度 100ms，加速 50ms 
 噩梦难度：正常速度 10ms，加速 5ms 
 食物概率：难度越高，食物生成概率越大（简单 2%，中等 5%，困难 8%） 
 关键代码： 

// 根据难度和加速状态设置基础速度   
if (difficulty != DEVIL)   
    speed = isSpeeding ? FAST_SPEED : NORMAL_SPEED;   
else   
    speed = isSpeeding ? DEVIL_FAST_SPEED : DEVIL_SPEED;   
// 应用食物效果（修改速度）   
switch (game.ef) {   
case normal: speed *= 1.0; break;   
case Speed:  speed *= 0.5; break;  // 加速（速度减半）   
case slow:   speed *= 2.5; break;  // 减速（速度×2.5）   
}   

3.2.2.6 存储模块 
 函数：loadHighScore()、saveHighScore(int score)
 功能：使用二进制文件存储最高分，游戏启动时读取，结束时更新 
 关键代码： 

// 加载最高分   
int loadHighScore() {   
    FILE* file = fopen(HIGHSCORE_FILE, "rb");   
    int highScore = 0;   
    if (file) {   
        fread(&highScore, sizeof(int), 1, file);   
        fclose(file);   
    }   
    return highScore;   
}   
3.2.3 算法设计 
3.2.3.1 蛇移动算法 
采用链表头部插入 + 尾部删除机制： 
 每次移动在头部添加新节点（新蛇头位置） 
 若未吃到食物，删除尾部节点以保持长度不变 
 若吃到食物，保留尾部节点实现长度增长 
3.2.3.2 食物生成算法 
 基于难度的概率生成食物（难度越高概率越大） 
 随机生成坐标，确保： 
 在游戏区域内 
 不与蛇身重叠 
 不与现有食物重叠 
 最多尝试 100 次生成有效位置，避免死循环 
3.2.3.3 碰撞检测算法 
 边界碰撞：检测蛇头坐标是否超出游戏区域范围 
 自身碰撞：遍历蛇身节点，检测蛇头是否与任何蛇身节点坐标重叠 
3.3 界面设计 
3.3.1 主菜单界面 
 包含游戏标题（ASCII 艺术字） 
 三个菜单项：开始游戏、难度设置、退出游戏 
 选中项高亮显示（黄色），支持上下键导航 
 
 
3.3.2 游戏界面 
 边界：由 "#" 字符组成的矩形框 
 蛇：头部为 "O"，身体为 "o"，加速时变为红色 
 食物：不同类型食物使用不同符号和颜色（如 "*" 绿色、"@" 蓝色） 
 信息栏：显示分数、最高分、控制提示、难度和食物类型说明 
  
3.3.3 结束界面 
 显示 "GAME OVER" 提示 
 展示最终分数，若创纪录则显示 "NEW HIGH SCORE!" 
 提供操作提示：按 R 重启或 ESC 返回菜单 
功能实现清单 

功能模块	具体功能	实现状态
菜单系统	主菜单导航	已实现
	难度设置（4 级）	已实现
蛇控制	方向移动（WASD）	已实现
	加速移动（长按方向键）	已实现
	长度增长	已实现
食物系统 	多类型食物	已实现
	特殊效果（加速、减速）	已实现
	随机生成	已实现
碰撞检测	边界碰撞	已实现
	自身碰撞	已实现
分数系统	分数计算	已实现
	最高分记录	已实现
其他功能	暂停 / 继续（P 键）	已实现
	游戏重置（R 键）	已实现
	退出游戏（X 键）	已实现
四、总结与展望 
4.1 项目总结 
本项目完成了贪吃蛇游戏的核心功能，包括蛇的移动与增长、食物系统、碰撞检测、分数记录和多难度设置，通过模块化设计和合理的数据结构（如链表）保证了游戏的高效运行。 
4.2 未来扩展方向 
 增加音效系统，提升游戏体验 
 实现关卡系统，增加游戏挑战性 
 支持自定义皮肤（蛇、食物的外观） 
 增加多人对战模式 
 移植到图形界面（如 SDL 或 Qt） 
附录：核心常量定义 

// 难度常量   
#define EASY 1   
#define MEDIUM 2   
#define HARD 3   
#define DEVIL 4   
// 颜色常量   
#define RED 12   
#define GREEN 10   
#define YELLOW 14   
#define BLUE 9   
#define WHITE 15   
// 游戏参数   
#define MAX_SNAKE_LENGTH 100  // 蛇最大长度   
#define NORMAL_SPEED 100      // 正常速度（毫秒）   
#define FAST_SPEED 50         // 加速速度   
#define MAX_FOOD 100          // 最大食物数量   

