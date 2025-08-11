#pragma once
#include <stdio.h>  // 标准输入输出库（printf、文件操作等）
#include <stdlib.h> // 标准库（内存分配、随机数、程序退出等）
#include <conio.h>  // 控制台输入库（检测键盘按键）
#include <windows.h>// Windows系统API（控制台光标、颜色控制等）
#include <time.h>   // 时间库（初始化随机数种子）
#include <stdbool.h>// 布尔类型库（true/false）
#include <io.h>     // 输入输出辅助库（预留扩展）


// 难度常量：控制游戏速度和食物复杂度
#define EASY 1       // 简单难度
#define MEDIUM 2     // 中等难度
#define HARD 3       // 困难难度
#define DEVIL 4      // 噩梦难度

// 菜单选项常量：标识主菜单的可选项
#define MENU_START 0       // 开始游戏
#define MENU_DIFFICULTY 1  // 难度设置
#define MENU_EXIT 2        // 退出游戏

// 最高分文件路径：用于持久化存储最高分数据
#define HIGHSCORE_FILE "snake_highscore.dat"

// 颜色常量：对应Windows控制台颜色值（前景色）
#define RED 12       // 红色
#define GREEN 10     // 绿色
#define YELLOW 14    // 黄色
#define BLUE 9       // 蓝色
#define WHITE 15     // 白色

// 游戏核心常量：控制游戏规则和性能参数
#define MAX_SNAKE_LENGTH 100  // 蛇的最大长度（链表节点数量上限）
#define NORMAL_SPEED 100      // 正常移动速度（毫秒/帧）
#define SLOW_SPEED 150        // 慢速移动速度（预留）
#define FAST_SPEED 50         // 加速移动速度（长按方向键）
#define MAX_FOOD 100          // 最大食物数量
#define DEVIL_SPEED 10        // 噩梦难度正常速度
#define DEVIL_FAST_SPEED 5    // 噩梦难度加速速度

// 食物生成概率：控制不同难度下食物出现的频率
#define FOOD_SPAWN_PROBABILITY 0.02f  // 基础概率2%
#define DIFFICULTY_BONUS_EASY 0.0f    // 简单难度无额外概率
#define DIFFICULTY_BONUS_MEDIUM 0.03f // 中等难度+3%（总5%）
#define DIFFICULTY_BONUS_HARD 0.15f   // 困难难度+15%（总20%）