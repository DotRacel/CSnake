#include "snake_graphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

static int game_width = 0;
static int game_height = 0;
static char screen_buffer[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
static bool game_initialized = false;

bool init_game_window(int width, int height, const char* title) {
    if (width < MIN_BOARD_SIZE || width > MAX_BOARD_SIZE ||
        height < MIN_BOARD_SIZE || height > MAX_BOARD_SIZE) {
        printf("错误: 游戏区域尺寸必须在 %d 到 %d 之间\n", MIN_BOARD_SIZE, MAX_BOARD_SIZE);
        return false;
    }

    game_width = width;
    game_height = height;

    // 调整终端字符编码为 UTF-8
    system("chcp 65001");

    // 初始化随机数种子
    srand((unsigned int)time(NULL));

    // 清空屏幕缓冲区
    memset(screen_buffer, ' ', sizeof(screen_buffer));

    // 设置控制台标题
    SetConsoleTitle(title);

    printf("=== %s ===\n", title);
    printf("游戏区域: %d x %d\n", width, height);
    printf("使用 WASD 或方向键控制\n");
    printf("按任意键开始游戏...\n");

    game_initialized = true;
    return true;
}

void clear_screen() {
    if (!game_initialized) return;

    system("cls");

    // 清空缓冲区
    for (int i = 0; i < game_height; i++) {
        for (int j = 0; j < game_width; j++) {
            screen_buffer[i][j] = ' ';
        }
    }
}

void draw_block(int x, int y, Symbol symbol) {
    if (!game_initialized || x < 0 || x >= game_width || y < 0 || y >= game_height) {
        return;
    }

    // 根据颜色选择显示字符
    char block_char;
    switch (symbol) {
        case FOOD:    block_char = '#'; break;
        case SNAKE_HEAD:  block_char = 'O'; break;
        case SNAKE_BODY:   block_char = '@'; break;
        default:           block_char = '#'; break;
    }

    screen_buffer[y][x] = block_char;
}

void draw_snake(const Snake* snake) {
    if (!snake || !game_initialized) return;

    // 绘制蛇头
    if (snake->length > 0) {
        draw_block(snake->body[0].x, snake->body[0].y, SNAKE_HEAD);
    }

    // 绘制蛇身
    for (int i = 1; i < snake->length; i++) {
        draw_block(snake->body[i].x, snake->body[i].y, SNAKE_BODY);
    }
}

void draw_food(int x, int y) {
    draw_block(x, y, FOOD);
}

void draw_score(int score) {
    if (!game_initialized) return;

    printf("分数: %d\n");
}

void refresh_screen(void) {
    if (!game_initialized) return;

    // 绘制上边框
    printf("+");
    for (int i = 0; i < game_width; i++) printf("-");
    printf("+\n");

    // 绘制游戏区域
    for (int y = 0; y < game_height; y++) {
        printf("|");
        for (int x = 0; x < game_width; x++) {
            printf("%c", screen_buffer[y][x]);
        }
        printf("|\n");
    }

    // 绘制下边框
    printf("+");
    for (int i = 0; i < game_width; i++) printf("-");
    printf("+\n");

    fflush(stdout);
}

int get_key_input(void) {
    if (_kbhit()) {
        int key = _getch();
        switch (key) {
            case 'w': case 'W': case 72: return DIRECTION_UP;    // W 或上箭头
            case 's': case 'S': case 80: return DIRECTION_DOWN;  // S 或下箭头
            case 'a': case 'A': case 75: return DIRECTION_LEFT;  // A 或左箭头
            case 'd': case 'D': case 77: return DIRECTION_RIGHT; // D 或右箭头
        }
    }
    return -1; // 没有按键
}

void game_delay(int milliseconds) {
    Sleep(milliseconds);
}

void cleanup_game(void) {
    game_initialized = false;
    printf("\n游戏结束，感谢游玩！\n");
}

Point make_point(int x, int y) {
    Point p = {x, y};
    return p;
}

bool point_equals(Point p1, Point p2) {
    return (p1.x == p2.x && p1.y == p2.y);
}

bool point_in_bounds(Point point, int width, int height) {
    return (point.x >= 0 && point.x < width && point.y >= 0 && point.y < height);
}

void init_snake(Snake* snake, int start_x, int start_y, int initial_length) {
    if (!snake || initial_length <= 0 || initial_length > MAX_SNAKE_LENGTH) return;

    snake->length = initial_length;
    snake->direction = DIRECTION_RIGHT;

    // 初始化蛇身，水平排列
    for (int i = 0; i < initial_length; i++) {
        snake->body[i].x = start_x - i;
        snake->body[i].y = start_y;
    }
}

Point move_point(Point head, Direction direction) {
    Point new_head = head;

    switch (direction) {
        case DIRECTION_UP:    new_head.y--; break;
        case DIRECTION_DOWN:  new_head.y++; break;
        case DIRECTION_LEFT:  new_head.x--; break;
        case DIRECTION_RIGHT: new_head.x++; break;
    }

    return new_head;
}

bool snake_hit_itself(const Snake* snake) {
    if (!snake || snake->length <= 1) return false;

    Point head = snake->body[0];
    for (int i = 1; i < snake->length; i++) {
        if (point_equals(head, snake->body[i])) {
            return true;
        }
    }
    return false;
}

Point generate_food(int width, int height, const Snake* snake) {
    Point food;
    bool valid_position;

    do {
        valid_position = true;
        food.x = rand() % width;
        food.y = rand() % height;

        // 检查食物是否生成在蛇身上
        if (snake) {
            for (int i = 0; i < snake->length; i++) {
                if (point_equals(food, snake->body[i])) {
                    valid_position = false;
                    break;
                }
            }
        }
    } while (!valid_position);

    return food;
}

bool directions_opposite(Direction dir1, Direction dir2) {
    return ((dir1 == DIRECTION_UP && dir2 == DIRECTION_DOWN) ||
            (dir1 == DIRECTION_DOWN && dir2 == DIRECTION_UP) ||
            (dir1 == DIRECTION_LEFT && dir2 == DIRECTION_RIGHT) ||
            (dir1 == DIRECTION_RIGHT && dir2 == DIRECTION_LEFT));
}