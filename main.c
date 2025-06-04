#include "snake_graphics.h"
#include <conio.h>
#include <stdio.h>

int main() {
    // 游戏配置
    const int BOARD_WIDTH = 50;
    const int BOARD_HEIGHT = 15;
    const int INITIAL_SNAKE_LENGTH = 3;

    // 初始化游戏
    if (!init_game_window(BOARD_WIDTH, BOARD_HEIGHT, "贪吃蛇")) {
        return 1;
    }

    // 等待用户按任意键开始游戏
    bool started = false;
    while(!started) {
        if (_kbhit()) started = true;
    }

    // 初始化蛇
    Snake snake;
    init_snake(&snake, BOARD_WIDTH/2, BOARD_HEIGHT/2, INITIAL_SNAKE_LENGTH);

    // 生成第一个食物
    Point food = generate_food(BOARD_WIDTH, BOARD_HEIGHT, &snake);

    // 游戏状态
    int score = 0;
    bool game_running = true;

    // 游戏主循环
    while (game_running) {
        // 获取用户输入
        int key = get_key_input();
        if (key != -1) {
            Direction new_direction = (Direction)key;
            // 防止蛇反向移动
            if (!directions_opposite(snake.direction, new_direction)) {
                snake.direction = new_direction;
            }
        }

        // 移动蛇
        Point new_head = move_point(snake.body[0], snake.direction);

        // 检查边界碰撞
        if (!point_in_bounds(new_head, BOARD_WIDTH, BOARD_HEIGHT)) {
            printf("撞墙了！游戏结束！\n");
            game_running = false;
        }

        // 检查自撞
        if (snake_hit_itself(&snake)) {
            printf("撞到自己了！游戏结束！\n");
            game_running = false;
        }

        // 移动蛇身
        for (int i = snake.length - 1; i > 0; i--) {
            snake.body[i] = snake.body[i - 1];
        }
        snake.body[0] = new_head;

        // 检查是否吃到食物
        if (point_equals(new_head, food)) {
            score += 10;
            snake.length++;
            food = generate_food(BOARD_WIDTH, BOARD_HEIGHT, &snake);
            printf("吃到食物！分数: %d\n", score);
        }

        // 清空并重绘屏幕
        clear_screen();
        draw_snake(&snake);
        draw_food(food.x, food.y);
        refresh_screen();

        // 显示分数
        draw_score(score);

        // 控制游戏速度
        game_delay(200);
    }

    // 清理资源
    cleanup_game();
    return 0;
}
