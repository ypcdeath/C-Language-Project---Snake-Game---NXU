#define _CRT_SECURE_NO_WARNINGS // ����VS�������İ�ȫ���棨��fopen������
#include "const_var.h"
#include "funcs.h"

// ����̨��Ϣ�ṹ�壺�洢����̨�ߴ��ƫ����������Ԫ�ض�λ��
typedef struct {
    int width;    // ����̨��ȣ��ַ�����
    int height;   // ����̨�߶ȣ��ַ�����
    int offsetX;  // X����ƫ�ƣ��߿����ף�
    int offsetY;  // Y����ƫ�ƣ��߿����ף�
} ConsoleInfo;

// ����ö�٣������ߵ��ƶ����򣨿����߼����ģ�
enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

// Ч��ö�٣�����ʳ�������Ч����Ӱ����Ϸ�ٶȣ�
enum Effict { Speed, slow, reverse, normal };

// �߽ڵ�ṹ�壨����ڵ㣩���洢�ߵ�һ���ڶε�����
typedef struct SnakeNode {
    int x;                  // �ڵ�X���꣨��Ϸ�����ڣ�
    int y;                  // �ڵ�Y���꣨��Ϸ�����ڣ�
    struct SnakeNode* next; // ָ����һ���ڵ��ָ�루������ģ�
} SnakeNode;

// �߽ṹ�壨������������������ߵ�����ṹ
typedef struct {
    SnakeNode* head; // ����ͷ�ڵ㣨��ͷ��
    SnakeNode* tail; // ����β�ڵ㣨��β��
    int length;      // �ߵĳ��ȣ��ڵ�������
    enum Direction dir; // ��ǰ�ƶ�����
} Snake;

// ʳ��ṹ�壺�洢����ʳ������Ժ�λ��
typedef struct {
    int x[MAX_FOOD];       // ʳ��X��������
    int y[MAX_FOOD];       // ʳ��Y��������
    int count;             // ������ʳ�������
    int score;             // �Ը�ʳ���õķ���
    int color;             // ʳ����ʾ��ɫ
    char symbol;           // ʳ����ʾ���ţ���*��@��
    int growth;            // �Ժ��������Ľ���
    enum Effict eff;       // ʳ�������Ч�������١����ٵȣ�
} Food;

// ��Ϸ״̬�ṹ�壺�洢������Ϸ�ĺ�������
typedef struct {
    Snake snake;               // �ߵ���Ϣ������ṹ��
    Food food[6];              // ʳ�����飨���6�����ͣ�
    int foodTypes;             // ��ǰ���õ�ʳ���������������Ѷ����ӣ�
    int score;                 // ��ǰ��Ϸ����
    bool gameOver;             // ��Ϸ�Ƿ������trueΪ������
    int width;                 // ��Ϸ�����ȣ��ַ�����
    int height;                // ��Ϸ����߶ȣ��ַ�����
    int highScore;             // ��ʷ��߷�
    int difficulty;            // ��ǰ��Ϸ�Ѷȣ�EASY/MEDIUM�ȣ�
    bool paused;               // ��Ϸ�Ƿ���ͣ��trueΪ��ͣ��
    enum Effict ef;            // ��ǰ��Ч��ʳ��Ч��
    int speed;                 // ��ǰ��Ϸ�ٶȣ�����/֡��
} GameState;


// ���ܣ����ÿ���̨�ı���ɫ
// ������color - Windows����̨��ɫֵ����RED=12��
void setColor(int color) {
    // ��ȡ��׼����豸�����Windows API��
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    // �����ı����ԣ���ɫ��
    SetConsoleTextAttribute(hConsole, color);
}


// ���ܣ����ÿ���̨���λ��
// ������x - �����꣬y - ������
void setCursorPosition(int x, int y) {
    COORD coord;  // Windows���������ṹ��
    coord.X = x;
    coord.Y = y;
    // �ƶ���굽ָ��λ�ã�Windows API��
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


// ���ܣ����ؿ���̨��꣨�������ƶ�ʱ�Ĺ����˸��
void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;  // �����Ϣ�ṹ��
    info.dwSize = 100;         // ����С��1-100��
    info.bVisible = FALSE;     // ��겻�ɼ�
    // ���ù����Ϣ��Windows API��
    SetConsoleCursorInfo(consoleHandle, &info);
}


// ���ܣ���ȡ����̨��Ϣ����ȡ��߶ȡ�ƫ������
// ���أ�ConsoleInfo�ṹ�壬��������̨�ߴ��ƫ��
ConsoleInfo getConsoleInfo() {
    ConsoleInfo info;
    CONSOLE_SCREEN_BUFFER_INFO csbi;  // Windows����̨��������Ϣ

    // ��ȡ����̨��������Ϣ���������ڳߴ磩
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    // �������̨ʵ�ʿ�Ⱥ͸߶ȣ��ַ�����
    info.width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    info.height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    // ������Ϸ����ƫ�������߿�����2�ַ���
    info.offsetX = 2;
    info.offsetY = 2;

    return info;
}


// ���ܣ����ļ�������ʷ��߷�
// ���أ���ȡ������߷֣��ļ��������򷵻�0��
int loadHighScore() {
    // �Զ�����ֻ��ģʽ����߷��ļ�
    FILE* file = fopen(HIGHSCORE_FILE, "rb");
    int highScore = 0;  // Ĭ����߷�Ϊ0

    if (file) {  // ���ļ�����
        // ���ļ���ȡ1��int�������ݣ���߷֣�
        fread(&highScore, sizeof(int), 1, file);
        fclose(file);  // �ر��ļ�
    }
    return highScore;
}


// ���ܣ�����ǰ��߷ֱ��浽�ļ�
// ������score - ��Ҫ�������߷�
void saveHighScore(int score) {
    // �Զ�����д��ģʽ���ļ����������򴴽���
    FILE* file = fopen(HIGHSCORE_FILE, "wb");
    if (file) {  // ���ļ��򿪳ɹ�
        // д����߷�����
        fwrite(&score, sizeof(int), 1, file);
        fclose(file);  // �ر��ļ�
    }
}


// ���ܣ���ʼ��ʳ�����ͣ������Ѷ����ÿ���ʳ�
// ������game - ��Ϸ״ָ̬��
void initFoodTypes(GameState* game) {
    // ��ʼ��6��ʳ������ԣ���ɫ�����š�����������ֵ��Ч����
    game->food[0].color = GREEN;
    game->food[0].symbol = '*';
    game->food[0].score = 1;
    game->food[0].growth = 1;
    game->food[0].eff = normal;

    game->food[1].color = BLUE;
    game->food[1].symbol = '@';
    game->food[1].score = 3;
    game->food[1].growth = 3;
    game->food[1].eff = normal;

    game->food[2].color = YELLOW;
    game->food[2].symbol = '$';
    game->food[2].score = 5;
    game->food[2].growth = 5;
    game->food[2].eff = normal;

    game->food[3].color = 0xB;  // ��ɫ
    game->food[3].symbol = 'S';
    game->food[3].score = 1;
    game->food[3].growth = 1;
    game->food[3].eff = Speed;

    game->food[4].color = 0xE;  // ����ɫ
    game->food[4].symbol = 'L';
    game->food[4].score = 2;
    game->food[4].growth = 2;
    game->food[4].eff = slow;

    game->food[5].color = 0xD;  // ��ɫ
    game->food[5].symbol = 'R';
    game->food[5].score = 3;
    game->food[5].growth = 3;
    game->food[5].eff = reverse;

    // �����Ѷ��������õ�ʳ����������
    game->foodTypes = (game->difficulty == EASY) ? 2 :  // �򵥣�2��
        (game->difficulty == MEDIUM) ? 4 : 6;  // �еȣ�4�֣�����/ج�Σ�6��
}


// ���ܣ������µ��߽ڵ㣨�������������
// ������x - �ڵ�X���꣬y - �ڵ�Y����
// ���أ��½ڵ�ָ�루ʧ�ܷ���NULL��
SnakeNode* createSnakeNode(int x, int y) {
    // Ϊ�½ڵ�����ڴ�
    SnakeNode* node = (SnakeNode*)malloc(sizeof(SnakeNode));
    if (node) {  // ����ɹ�
        node->x = x;       // ��ʼ��X����
        node->y = y;       // ��ʼ��Y����
        node->next = NULL; // ��һ�ڵ��ʼΪNULL
    }
    return node;  // �����½ڵ�
}


// ���ܣ��ͷ��ߵ�����������ֹ�ڴ�й©��
// ������snake - �߽ṹ��ָ��
void freeSnake(Snake* snake) {
    SnakeNode* current = snake->head;  // ��ͷ����ʼ����
    while (current) {  // ѭ��ֱ�����нڵ㱻�ͷ�
        SnakeNode* next = current->next;  // ������һ�ڵ��ַ
        free(current);                    // �ͷŵ�ǰ�ڵ�
        current = next;                   // �ƶ�����һ�ڵ�
    }
    // �����ߵ�״̬
    snake->head = NULL;
    snake->tail = NULL;
    snake->length = 0;
}


// ���ܣ���ʼ����Ϸ״̬���ߡ�ʳ������ȣ�
// ������game - ��Ϸ״ָ̬�룬difficulty - ��Ϸ�Ѷ�
void initGame(GameState* game, int difficulty) {
    ConsoleInfo console = getConsoleInfo();  // ��ȡ����̨��Ϣ
    game->ef = normal;  // ��ʼ��ʳ��Ч��

    // ������Ϸ����ߴ磨��ȥ�߿����Ϣ����
    game->width = console.width - 4;
    game->height = console.height - 6;
    // ȷ����Ϸ����С����С�ߴ磨10x10��
    if (game->width < 10) game->width = 10;
    if (game->height < 10) game->height = 10;

    // ��ʼ����������ʼ����3�ڣ�
    game->snake.head = NULL;
    game->snake.tail = NULL;
    game->snake.length = 0;
    game->snake.dir = RIGHT;  // ��ʼ��������

    // ������ʼ3���ڵ㣨��ͷ����β�������У�
    int startX = game->width / 2;  // ��ʼX���꣨��Ļ�м䣩
    int startY = game->height / 2; // ��ʼY���꣨��Ļ�м䣩
    for (int i = 0; i < 3; i++) {
        SnakeNode* newNode = createSnakeNode(startX - i, startY);
        if (!game->snake.head) {  // ��һ���ڵ㣨ͷ��β���Ǹýڵ㣩
            game->snake.head = newNode;
            game->snake.tail = newNode;
        }
        else {  // �����ڵ㣨��ӵ�β����
            game->snake.tail->next = newNode;  // β�ڵ�ָ���½ڵ�
            game->snake.tail = newNode;        // ����β�ڵ�
        }
        game->snake.length++;  // ����+1
    }

    // ��ʼ����������ӣ�ȷ��ÿ����Ϸ��������ͬ��
    srand((unsigned int)time(NULL));

    // ��ʼ���ѶȺ�ʳ������
    game->difficulty = difficulty;
    initFoodTypes(game);

    // ��ʼ��ʳ�������1������ʳ�0�ţ�
    for (int i = 0; i < game->foodTypes; i++) {
        game->food[i].count = (i == 0) ? 1 : 0;  // ��0��ʳ���ʼ����Ϊ1
        if (i == 0) {  // Ϊ0��ʳ�����ɳ�ʼλ��
            game->food[i].x[0] = rand() % (game->width - 2) + 1;
            game->food[i].y[0] = rand() % (game->height - 2) + 1;
        }
    }

    // ��ʼ����Ϸ״̬����
    game->score = 0;           // ��ʼ����0
    game->gameOver = false;    // ��Ϸδ����
    game->highScore = loadHighScore();  // ������ʷ��߷�
    game->paused = false;      // ��ʼδ��ͣ
}


// ���ܣ�������Ϸ�߽����Ϣ��
// ������game - ��Ϸ״ָ̬��
void drawBoundary(GameState* game) {
    ConsoleInfo console = getConsoleInfo();  // ��ȡ����̨��Ϣ
    setColor(WHITE);  // �߿�������ð�ɫ

    // �����ϱ߿�һ��'#'�ַ���
    setCursorPosition(console.offsetX, console.offsetY);
    for (int i = 0; i < game->width + 2; i++)  // ���=��Ϸ�����+2�����ұ߿�
        printf("#");

    // �������ұ߿�ÿ�����׺���β��һ��'#'��
    for (int i = 1; i <= game->height; i++) {
        setCursorPosition(console.offsetX, console.offsetY + i);  // ��߽�
        printf("#");
        setCursorPosition(console.offsetX + game->width + 1, console.offsetY + i);  // �ұ߽�
        printf("#");
    }

    // �����±߿�һ��'#'�ַ���
    setCursorPosition(console.offsetX, console.offsetY + game->height + 1);
    for (int i = 0; i < game->width + 2; i++)
        printf("#");

    // ������Ϣ������ǰ����
    setCursorPosition(console.offsetX, console.offsetY + game->height + 2);
    printf("Score: %d", game->score);

    // ������Ϣ������߷�
    setCursorPosition(console.offsetX, console.offsetY + game->height + 3);
    printf("High Score: %d", game->highScore);

    // ������Ϣ����������ʾ
    setCursorPosition(console.offsetX, console.offsetY + game->height + 4);
    printf("Controls: W(Up), S(Down), A(Left), D(Right), P(Pause), X(Exit)");

    // ���Ƶ�ǰ�Ѷ�
    const char* difficultyStr;
    switch (game->difficulty) {
    case EASY:   difficultyStr = "Easy"; break;
    case MEDIUM: difficultyStr = "Medium"; break;
    case HARD:   difficultyStr = "Hard"; break;
    default:     difficultyStr = "unknown"; break;
    }
    setCursorPosition(console.offsetX + 20, console.offsetY + game->height + 4);
    printf("difficulty: %s", difficultyStr);

    // ����ʳ������˵������ʾÿ��ʳ��ķ��ţ�
    setCursorPosition(console.offsetX + 40, console.offsetY + game->height + 4);
    printf("Food: ");
    for (int i = 0; i < game->foodTypes; i++) {
        setColor(game->food[i].color);  // ʹ��ʳ��������ɫ
        printf("%c ", game->food[i].symbol);
    }
    setColor(WHITE);  // �ָ���ɫ
}


// ���ܣ�������ϷԪ�أ��ߡ�ʳ���ͣ��Ϣ��
// ������game - ��Ϸ״ָ̬�룬isSpeeding - �Ƿ��ڼ���״̬
void drawGameElements(GameState* game, bool isSpeeding) {
    ConsoleInfo console = getConsoleInfo();  // ��ȡ����̨��Ϣ

    // �����һ֡����β�����������
    static int lastTailX = -1, lastTailY = -1;  // ��̬����������һ֡β����
    if (lastTailX != -1 && lastTailY != -1) {
        // ��λ����βλ�ã��ÿո񸲸�
        setCursorPosition(console.offsetX + lastTailX + 1, console.offsetY + lastTailY + 1);
        printf(" ");
    }

    // ���µ�ǰ��β���꣨������һ֡�����
    if (game->snake.tail) {
        lastTailX = game->snake.tail->x;
        lastTailY = game->snake.tail->y;
    }

    // ������ͷ������ͷ�ڵ㣩
    if (game->snake.head) {
        setColor(isSpeeding ? RED : 4);  // ����ʱ��ɫ����������ɫ��4��
        setCursorPosition(console.offsetX + game->snake.head->x + 1,
            console.offsetY + game->snake.head->y + 1);
        printf("O");  // ��ͷ����
    }

    // ������������������head->next��ʼ��
    setColor(isSpeeding ? RED : 4);
    SnakeNode* current = game->snake.head ? game->snake.head->next : NULL;
    while (current) {  // ѭ��ֱ������ĩβ��currentΪNULL��
        setCursorPosition(console.offsetX + current->x + 1, console.offsetY + current->y + 1);
        printf("o");  // �������
        current = current->next;  // �ƶ�����һ�ڵ�
    }

    // ��������ʳ��
    for (int i = 0; i < game->foodTypes; i++) {  // ����ʳ������
        for (int j = 0; j < game->food[i].count; j++) {  // ���������͵�ÿ��ʳ��
            setColor(game->food[i].color);  // ʳ����ɫ
            setCursorPosition(console.offsetX + game->food[i].x[j] + 1,
                console.offsetY + game->food[i].y[j] + 1);
            printf("%c", game->food[i].symbol);  // ʳ�����
        }
    }

    // ���·�����ʾ�����������ֲ��֣�
    setColor(WHITE);
    setCursorPosition(console.offsetX + 7, console.offsetY + game->height + 2);
    printf("%d", game->score);

    // ����Ϸ��ͣ����ʾ��ͣ��Ϣ
    if (game->paused) {
        setColor(YELLOW);  // ��ɫ����
        setCursorPosition(console.offsetX + game->width / 2 - 9, console.offsetY + game->height / 2);
        printf("��Ϸ��ͣ��");
        setCursorPosition(console.offsetX + game->width / 2 - 12, console.offsetY + game->height / 2 + 1);
        printf("�� P ��������Ϸ");
    }
}


// ���ܣ������û����루�������ƶ�����ͣ�����õȣ�
// ������game - ��Ϸ״ָ̬�룬currentDir - ��ǰ�ƶ�����
// ���أ��Ƿ��ڼ���״̬�������������
bool input(GameState* game, enum Direction currentDir) {
    bool isSpeeding = false;  // �Ƿ����

    if (_kbhit()) {  // ��⵽�а�������
        int key = _getch();  // ��ȡ����������س���
        switch (key) {
        case 'a':  // ����
            // ��ֱֹ�ӷ���������ʱ����ֱ������
            if (game->snake.dir != RIGHT)
                game->snake.dir = LEFT;
            isSpeeding = true;  // ��������
            break;
        case 'd':  // ����
            if (game->snake.dir != LEFT)
                game->snake.dir = RIGHT;
            isSpeeding = true;
            break;
        case 'w':  // ����
            if (game->snake.dir != DOWN)
                game->snake.dir = UP;
            isSpeeding = true;
            break;
        case 's':  // ����
            if (game->snake.dir != UP)
                game->snake.dir = DOWN;
            isSpeeding = true;
            break;
        case 'x':  // �˳���Ϸ
            game->gameOver = true;  // �����Ϸ����
            break;
        case 'r':  // ������Ϸ
            freeSnake(&game->snake);  // �ͷž�������
            initGame(game, game->difficulty);  // ���³�ʼ��
            clearScreen();  // ����
            break;
        case 'p':
        case 'P':  // ��ͣ/����
            game->paused = !game->paused;  // �л���ͣ״̬
            clearScreen();  // ����ˢ��
            break;
        }
    }

    return isSpeeding;  // �����Ƿ����
}


// ���ܣ����������ʳ�ȷ�������߻�����ʳ���ص���
// ������game - ��Ϸ״ָ̬��
void spawnFood(GameState* game) {
    // ���ѡ��һ��ʳ�����ͣ�0��foodTypes-1��
    int foodType = rand() % game->foodTypes;

    // ��������ʳ���Ѵ������������������
    if (game->food[foodType].count >= MAX_FOOD)
        return;

    bool validPosition = false;  // λ���Ƿ���Ч�����ص���
    int maxAttempts = 100;       // ����Դ�������ֹ��ѭ����
    int newX, newY;              // ��ʳ������

    do {
        // ����������꣨�������ߣ�
        newX = rand() % (game->width - 2) + 1;
        newY = rand() % (game->height - 2) + 1;
        validPosition = true;

        // ����Ƿ��������ص�����������
        SnakeNode* current = game->snake.head;
        while (current) {
            if (newX == current->x && newY == current->y) {
                validPosition = false;  // �ص�����Ч
                break;
            }
            current = current->next;  // �ƶ�����һ�ڵ�
        }

        // ����Ƿ�������ʳ���ص�
        if (validPosition) {
            for (int i = 0; i < game->foodTypes; i++) {
                for (int j = 0; j < game->food[i].count; j++) {
                    if (newX == game->food[i].x[j] && newY == game->food[i].y[j]) {
                        validPosition = false;
                        break;
                    }
                }
                if (!validPosition) break;
            }
        }

        maxAttempts--;  // ���ٳ��Դ���
    } while (!validPosition && maxAttempts > 0);  // ֱ��λ����Ч���Ժľ�

    if (validPosition) {  // ���ҵ���Чλ��
        // �����ʳ�ﵽ�����͵�����
        game->food[foodType].x[game->food[foodType].count] = newX;
        game->food[foodType].y[game->food[foodType].count] = newY;
        game->food[foodType].count++;  // ����+1
    }
}


// ���ܣ�������Ϸ״̬�����ƶ�����ʳ���ײ���ȣ�
// ������game - ��Ϸ״ָ̬��
void update(GameState* game) {
    // ����Ϸ��ͣ��������״̬
    if (game->paused) {
        return;
    }

    // ����Ϊ�գ��쳣�������ֱ�ӷ���
    if (!game->snake.head) {
        return;
    }

    // ��������ͷλ�ã����ݵ�ǰ����
    int newHeadX = game->snake.head->x;
    int newHeadY = game->snake.head->y;
    switch (game->snake.dir) {
    case LEFT:  newHeadX--; break;  // ���ƣ�x-1
    case RIGHT: newHeadX++; break;  // ���ƣ�x+1
    case UP:    newHeadY--; break;  // ���ƣ�y-1
    case DOWN:  newHeadY++; break;  // ���ƣ�y+1
    }

    // ��������ͷ�ڵ㣨����ͷ�����룩
    SnakeNode* newHead = createSnakeNode(newHeadX, newHeadY);
    if (!newHead) {  // �ڴ����ʧ��
        game->gameOver = true;
        return;
    }
    newHead->next = game->snake.head;  // �½ڵ�ָ���ͷ
    game->snake.head = newHead;        // ����ͷ�ڵ�Ϊ�½ڵ�
    game->snake.length++;              // ����+1

    // ����Ƿ�Ե�ʳ��
    bool ateFood = false;  // �Ƿ�Ե�ʳ��
    for (int k = 0; k < game->foodTypes; k++) {  // ����ʳ������
        for (int i = 0; i < game->food[k].count; i++) {  // ����������ʳ��
            // ����ͷ������ʳ��������ͬ���Ե�ʳ�
            if (newHeadX == game->food[k].x[i] && newHeadY == game->food[k].y[i]) {
                // ��ȡʳ������
                int growth = game->food[k].growth;  // ��������
                game->ef = game->food[k].eff;       // ʳ��Ч��
                game->score += 10 * game->food[k].score;  // �ӷ�

                // �Ƴ����Ե���ʳ��
                if (game->food[k].count > 1) {  // ������>1�����Ǳ���λ��
                    for (int j = i; j < game->food[k].count - 1; j++) {
                        game->food[k].x[j] = game->food[k].x[j + 1];
                        game->food[k].y[j] = game->food[k].y[j + 1];
                    }
                }
                game->food[k].count--;  // ����-1
                ateFood = true;

                // ����growthֵ��������β����ӽڵ㣩
                for (int g = 0; g < growth; g++) {
                    // ����β�ڵ����괴���½ڵ�
                    SnakeNode* newNode = createSnakeNode(game->snake.tail->x, game->snake.tail->y);
                    if (newNode) {
                        game->snake.tail->next = newNode;  // β�ڵ�ָ���½ڵ�
                        game->snake.tail = newNode;        // ����β�ڵ�
                        game->snake.length++;              // ����+1
                    }
                }
                break;  // ������ǰʳ������ѭ��
            }
        }
        if (ateFood) break;  // ��������ʳ������ѭ��
    }

    // δ�Ե�ʳ��Ƴ�β�ڵ㣨���ֳ��Ȳ��䣩
    if (!ateFood) {
        // �ҵ������ڶ����ڵ㣨��β�ڵ㣩
        SnakeNode* current = game->snake.head;
        SnakeNode* prev = NULL;
        while (current->next) {  // ������β�ڵ��ǰһ��
            prev = current;
            current = current->next;
        }

        // �Ƴ�β�ڵ�
        prev->next = NULL;    // ��β�ڵ��next��ΪNULL
        free(game->snake.tail);  // �ͷž�β�ڵ�
        game->snake.tail = prev; // ����β�ڵ�
        game->snake.length--;    // ����-1
    }

    // �Ե�ʳ���ȷ��������1��ʳ�����
    if (ateFood) {
        for (int i = 0; i < game->foodTypes; i++) {
            if (game->food[i].count == 0) {  // ��ĳ����ʳ������Ϊ0
                spawnFood(game);  // ������ʳ��
                break;
            }
        }
    }

    // ���������������ʳ��Ѷ�Խ�߸���Խ��
    float spawnProb = FOOD_SPAWN_PROBABILITY;  // ����2%
    switch (game->difficulty) {
    case MEDIUM: spawnProb += DIFFICULTY_BONUS_MEDIUM; break;  // �е�+3%����5%��
    case HARD:   spawnProb += DIFFICULTY_BONUS_HARD; break;    // ����+6%����8%��
    }
    // �������������С�ڸ���������ʳ��
    if ((float)rand() / RAND_MAX < spawnProb) {
        spawnFood(game);
    }

    // ��ײ��⣺ײǽ��������Ϸ����߽磩
    if (newHeadX < 0 || newHeadX >= game->width ||
        newHeadY < 0 || newHeadY >= game->height) {
        game->gameOver = true;  // �����Ϸ����
    }

    // ��ײ��⣺ײ������ͷ�������ص���
    SnakeNode* current = game->snake.head->next;  // ����ͷ��һ���ڵ㿪ʼ
    while (current) {
        if (newHeadX == current->x && newHeadY == current->y) {
            game->gameOver = true;  // �����Ϸ����
            break;
        }
        current = current->next;  // �ƶ�����һ�ڵ�
    }
}


// ���ܣ���տ���̨��Ļ
void clearScreen() {
    system("cls");  // ����ϵͳ����cls����
}


// ���ܣ�������Ϸ���⣨ASCII�����֣�
void drawGameTitle() {
    ConsoleInfo console = getConsoleInfo();
    int centerX = console.width / 2;  // ����̨����X����

    // ���ƺ�ɫ����ASCII����
    setColor(RED);
    setCursorPosition(centerX - 18, 5);
    printf("  _________              __           ");
    setCursorPosition(centerX - 18, 6);
    printf(" /   _____/ ____   _____/  |_  ____   ");
    setCursorPosition(centerX - 18, 7);
    printf(" \\_____  \\_/ __ \\_/ ___\\   __\\/ __ \\  ");
    setCursorPosition(centerX - 18, 8);
    printf(" /        \\  ___/\\  \\___|  | \\  ___/  ");
    setCursorPosition(centerX - 18, 9);
    printf("/_______  /\\___  >\\___  >__|  \\___  > ");
    setCursorPosition(centerX - 18, 10);
    printf("        \\/     \\/     \\/          \\/  ");

    // ������ɫ������
    setColor(GREEN);
    setCursorPosition(centerX - 7, 12);
    printf("����̰������Ϸ");

    // ������ɫ�ָ���
    setColor(BLUE);
    setCursorPosition(centerX - 19, 14);
    printf("======================================");
}


// ���ܣ��������˵�����ʼ��Ϸ���Ѷ����á��˳���Ϸ��
// ������selected - ��ǰѡ�еĲ˵�������
void drawMenu(int selected) {
    ConsoleInfo console = getConsoleInfo();
    int centerX = console.width / 2;  // ����̨����X����

    // ����"��ʼ��Ϸ"ѡ�ѡ��ʱ��ɫ������
    setColor(selected == MENU_START ? YELLOW : WHITE);
    setCursorPosition(centerX - 6, 18);
    printf(selected == MENU_START ? "> ��ʼ��Ϸ <" : "  ��ʼ��Ϸ  ");

    // ����"�Ѷ�����"ѡ��
    setColor(selected == MENU_DIFFICULTY ? YELLOW : WHITE);
    setCursorPosition(centerX - 6, 20);
    printf(selected == MENU_DIFFICULTY ? "> �Ѷ����� <" : "  �Ѷ�����  ");

    // ����"�˳���Ϸ"ѡ��
    setColor(selected == MENU_EXIT ? YELLOW : WHITE);
    setCursorPosition(centerX - 6, 22);
    printf(selected == MENU_EXIT ? "> �˳���Ϸ <" : "  �˳���Ϸ  ");
}


// ���ܣ������Ѷ�ѡ��˵����򵥡��еȡ����ѡ�ج�Σ�
// ���أ�ѡ�е��Ѷȣ�EASY/MEDIUM/HARD/DEVIL��
int drawDifficultyMenu() {
    clearScreen();  // ����
    drawGameTitle();  // ���Ʊ���

    int selected = EASY;  // Ĭ��ѡ�м��Ѷ�
    int key;  // �洢����ֵ

    ConsoleInfo console = getConsoleInfo();
    int centerX = console.width / 2;  // ����̨����X����

    while (1) {  // ѭ���ȴ��û�ѡ��
        setColor(WHITE);
        setCursorPosition(centerX - 10, 16);
        printf("��ѡ����Ϸ�Ѷ�:");

        // ����"��"�Ѷ�ѡ��
        setColor(selected == EASY ? YELLOW : WHITE);
        setCursorPosition(centerX - 6, 18);
        printf(selected == EASY ? "> �� <" : "  ��  ");

        // ����"�е�"�Ѷ�ѡ��
        setColor(selected == MEDIUM ? YELLOW : WHITE);
        setCursorPosition(centerX - 6, 20);
        printf(selected == MEDIUM ? "> �е� <" : "  �е�  ");

        // ����"����"�Ѷ�ѡ��
        setColor(selected == HARD ? YELLOW : WHITE);
        setCursorPosition(centerX - 6, 22);
        printf(selected == HARD ? "> ���� <" : "  ����  ");

        // ����"ج��"�Ѷ�ѡ��
        setColor(selected == DEVIL ? YELLOW : WHITE);
        setCursorPosition(centerX - 6, 24);
        printf(selected == DEVIL ? "> ج�� <" : "  ج��  ");

        // ���Ʋ�����ʾ
        setColor(BLUE);
        setCursorPosition(centerX - 15, 26);
        printf("ʹ�� �� �� ��ѡ�񣬰�Enterȷ��");

        if (_kbhit()) {  // ��⵽����
            key = _getch();  // ��ȡ����

            if (key == 224 || key == 0) {  // ���������չASCII�룩
                key = _getch();  // ��ȡʵ�ʷ����ֵ
                switch (key) {
                case 72:  // �ϼ�ͷ
                    // ѡ�������ƣ��򵥡�ج��ѭ����
                    selected = (selected == EASY) ? DEVIL : selected - 1;
                    break;
                case 80:  // �¼�ͷ
                    // ѡ�������ƣ�ج�Ρ���ѭ����
                    selected = (selected == DEVIL) ? EASY : selected + 1;
                    break;
                }
            }
            else if (key == 13) {  // �س�����ȷ��ѡ��
                clearScreen();
                return selected;  // ����ѡ�е��Ѷ�
            }
            else if (key == 27) {  // ESC����ȡ��������-1��
                clearScreen();
                return -1;
            }
        }
        Sleep(100);  // �����ӳ٣�����CPUռ��
    }
}


// ���ܣ����ƿ�ʼ�˵��������棩������ѡ�е��Ѷ�
// ������dif - ��ǰ�Ѷȣ����ڱ����û�֮ǰ��ѡ��
int drawStartScreen(int dif) {
    int selected = MENU_START;  // Ĭ��ѡ��"��ʼ��Ϸ"
    int currentDifficulty = dif;  // ��ǰ�Ѷ�

    while (1) {  // ѭ���ȴ��û�����
        drawGameTitle();  // ���Ʊ���
        drawMenu(selected);  // ���Ʋ˵�

        if (_kbhit()) {  // ��⵽����
            int key = _getch();  // ��ȡ����

            if (key == 224 || key == 0) {  // �����
                key = _getch();  // ��ȡʵ�ʷ����ֵ
                switch (key) {
                case 72:  // �ϼ�ͷ��ѡ��������
                    selected = (selected == MENU_START) ? MENU_EXIT : selected - 1;
                    break;
                case 80:  // �¼�ͷ��ѡ��������
                    selected = (selected == MENU_EXIT) ? MENU_START : selected + 1;
                    break;
                }
            }
            else if (key == 13) {  // �س�����ȷ��ѡ��
                switch (selected) {
                case MENU_START:  // ��ʼ��Ϸ
                    clearScreen();
                    return currentDifficulty;  // ���ص�ǰ�Ѷ�

                case MENU_DIFFICULTY:  // �Ѷ�����
                    clearScreen();
                    int newDifficulty = drawDifficultyMenu();  // ��ȡ���Ѷ�
                    if (newDifficulty != -1) {  // ���û�ȷ��ѡ��
                        currentDifficulty = newDifficulty;  // ���µ�ǰ�Ѷ�
                    }
                    break;

                case MENU_EXIT:  // �˳���Ϸ
                    exit(0);  // ��ֹ����
                }
            }
            else if (key == 27) {  // ESC�����˳���Ϸ��
                exit(0);
            }
        }
        Sleep(100);  // �����ӳ٣�����CPUռ��
    }
}


// ��������������ڣ�������Ϸ��������
int main() {
    system("title ̰������Ϸ - ��R���� ��P��ͣ");  // ���ô��ڱ���
    hideCursor();  // ���ع�꣨������˸��

    // ��ʾ��Ϸ��ʾ��3������˵���
    printf("tips:�������Լ���...");
    Sleep(500);  // �ȴ�0.5��
    printf("  ��ͬ��ʳ���в�ͬ��Ч���ͷ���...");
    Sleep(500);  // �ٵȴ�0.5��

    int difficulty = EASY;  // Ĭ���Ѷ�Ϊ��

    while (1) {  // ��Ϸ��ѭ�������ظ���ʼ��
        // ��ʾ��ʼ�˵�����ȡ�û�ѡ����Ѷ�
        difficulty = drawStartScreen(difficulty);

        GameState game;  // ��Ϸ״̬����
        initGame(&game, difficulty);  // ��ʼ����Ϸ

        int speed = NORMAL_SPEED;  // ��ʼ�ٶ�
        bool isSpeeding = false;   // �Ƿ����

        // ��Ϸѭ����ÿ֡���£�ֱ����Ϸ������
        while (!game.gameOver) {
            drawBoundary(&game);  // ���Ʊ߿����Ϣ��
            drawGameElements(&game, isSpeeding);  // �����ߡ�ʳ���
            isSpeeding = input(&game, game.snake.dir);  // ��������

            // �����ѶȺͼ���״̬���û����ٶ�
            if (difficulty != DEVIL)
                speed = isSpeeding ? FAST_SPEED : NORMAL_SPEED;
            else
                speed = isSpeeding ? DEVIL_FAST_SPEED : DEVIL_SPEED;

            // Ӧ��ʳ��Ч�����޸��ٶȣ�
            switch (game.ef) {
            case normal: speed *= 1.0; break;  // ��Ч��
            case Speed:  speed *= 0.5; break;  // ���٣��ٶȼ��룩
            case slow:   speed *= 2.5; break;  // ���٣��ٶȡ�2.5��
            case reverse:speed *= 0.01; break; // ����Ч�������죩
            }

            update(&game);  // ������Ϸ״̬�����ƶ�����ײ���ȣ�
            Sleep(speed);  // ����֡�ʣ��ȴ�speed���룩
        }

        // ��Ϸ��������
        ConsoleInfo console = getConsoleInfo();

        // ��ʾ��Ϸ������Ϣ
        setCursorPosition(console.offsetX, console.offsetY + game.height + 6);
        setColor(YELLOW);
        printf("  ===== GAME OVER =====");

        // ��ʾ���շ���
        setCursorPosition(console.offsetX, console.offsetY + game.height + 7);
        printf("  Final Score: %d", game.score);

        // ������¼�����²�������߷�
        if (game.score > game.highScore) {
            game.highScore = game.score;
            saveHighScore(game.highScore);  // ��������߷�
            setColor(GREEN);
            setCursorPosition(console.offsetX, console.offsetY + game.height + 8);
            printf("NEW HIGH SCORE!");
        }

        // ��ʾ����������ʾ
        setColor(WHITE);
        setCursorPosition(console.offsetX, console.offsetY + game.height + 10);
        printf("  Press 'R' to restart or 'ESC' to return to menu");

        // �ͷ��������ڴ棨��ֹй©��
        freeSnake(&game.snake);

        // �ȴ��û������������򷵻ز˵���
        while (1) {
            if (_kbhit()) {  // ��⵽����
                int key = _getch();
                if (key == 'r' || key == 'R') {  // R����������Ϸ
                    clearScreen();
                    break;
                }
                else if (key == 27) {  // ESC�������ز˵�
                    clearScreen();
                    break;
                }
            }
            Sleep(100);  // �����ӳ�
        }
    }

    return 0;  // ��������������ʵ�ʲ���ִ�е��˴���
}