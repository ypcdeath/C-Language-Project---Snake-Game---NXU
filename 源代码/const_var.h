#pragma once
#include <stdio.h>  // ��׼��������⣨printf���ļ������ȣ�
#include <stdlib.h> // ��׼�⣨�ڴ���䡢������������˳��ȣ�
#include <conio.h>  // ����̨����⣨�����̰�����
#include <windows.h>// WindowsϵͳAPI������̨��ꡢ��ɫ���Ƶȣ�
#include <time.h>   // ʱ��⣨��ʼ����������ӣ�
#include <stdbool.h>// �������Ϳ⣨true/false��
#include <io.h>     // ������������⣨Ԥ����չ��


// �Ѷȳ�����������Ϸ�ٶȺ�ʳ�︴�Ӷ�
#define EASY 1       // ���Ѷ�
#define MEDIUM 2     // �е��Ѷ�
#define HARD 3       // �����Ѷ�
#define DEVIL 4      // ج���Ѷ�

// �˵�ѡ�������ʶ���˵��Ŀ�ѡ��
#define MENU_START 0       // ��ʼ��Ϸ
#define MENU_DIFFICULTY 1  // �Ѷ�����
#define MENU_EXIT 2        // �˳���Ϸ

// ��߷��ļ�·�������ڳ־û��洢��߷�����
#define HIGHSCORE_FILE "snake_highscore.dat"

// ��ɫ��������ӦWindows����̨��ɫֵ��ǰ��ɫ��
#define RED 12       // ��ɫ
#define GREEN 10     // ��ɫ
#define YELLOW 14    // ��ɫ
#define BLUE 9       // ��ɫ
#define WHITE 15     // ��ɫ

// ��Ϸ���ĳ�����������Ϸ��������ܲ���
#define MAX_SNAKE_LENGTH 100  // �ߵ���󳤶ȣ�����ڵ��������ޣ�
#define NORMAL_SPEED 100      // �����ƶ��ٶȣ�����/֡��
#define SLOW_SPEED 150        // �����ƶ��ٶȣ�Ԥ����
#define FAST_SPEED 50         // �����ƶ��ٶȣ������������
#define MAX_FOOD 100          // ���ʳ������
#define DEVIL_SPEED 10        // ج���Ѷ������ٶ�
#define DEVIL_FAST_SPEED 5    // ج���Ѷȼ����ٶ�

// ʳ�����ɸ��ʣ����Ʋ�ͬ�Ѷ���ʳ����ֵ�Ƶ��
#define FOOD_SPAWN_PROBABILITY 0.02f  // ��������2%
#define DIFFICULTY_BONUS_EASY 0.0f    // ���Ѷ��޶������
#define DIFFICULTY_BONUS_MEDIUM 0.03f // �е��Ѷ�+3%����5%��
#define DIFFICULTY_BONUS_HARD 0.15f   // �����Ѷ�+15%����20%��