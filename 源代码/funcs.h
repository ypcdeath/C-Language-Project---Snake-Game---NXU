#pragma once
// 函数声明：提前声明所有函数，确保编译器能识别调用
int drawStartScreen(int dif);       // 绘制开始菜单，返回选中的难度
void drawGameTitle();               // 绘制游戏标题（ASCII艺术字）
void drawMenu(int selected);        // 绘制主菜单，高亮选中项
void clearScreen();                 // 清空控制台屏幕
int drawDifficultyMenu();           // 绘制难度选择菜单，返回选中的难度
void setColor(int color);           // 设置控制台文本颜色
void setCursorPosition(int x, int y);// 设置光标位置（x列，y行）
void hideCursor();                  // 隐藏控制台光标（避免闪烁）