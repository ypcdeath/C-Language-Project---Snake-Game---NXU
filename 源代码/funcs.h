#pragma once
// ������������ǰ�������к�����ȷ����������ʶ�����
int drawStartScreen(int dif);       // ���ƿ�ʼ�˵�������ѡ�е��Ѷ�
void drawGameTitle();               // ������Ϸ���⣨ASCII�����֣�
void drawMenu(int selected);        // �������˵�������ѡ����
void clearScreen();                 // ��տ���̨��Ļ
int drawDifficultyMenu();           // �����Ѷ�ѡ��˵�������ѡ�е��Ѷ�
void setColor(int color);           // ���ÿ���̨�ı���ɫ
void setCursorPosition(int x, int y);// ���ù��λ�ã�x�У�y�У�
void hideCursor();                  // ���ؿ���̨��꣨������˸��