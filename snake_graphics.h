#ifndef SNAKE_GRAPHICS_H
#define SNAKE_GRAPHICS_H

// 游戏配置常量
#define MAX_SNAKE_LENGTH 1000
#define MIN_BOARD_SIZE 10
#define MAX_BOARD_SIZE 50

// 符号，代表蛇的身体、头、食物等
typedef enum {
    FOOD,
    SNAKE_HEAD,
    SNAKE_BODY,
} Symbol;

// 方向枚举，表示上下左右
typedef enum {
    DIRECTION_UP,
    DIRECTION_DOWN,
    DIRECTION_LEFT,
    DIRECTION_RIGHT
} Direction;

// 点结构体，表示游戏中的坐标
typedef struct {
    int x;
    int y;
} Point;

// 蛇的结构体，封装蛇的所有信息
typedef struct {
    Point body[MAX_SNAKE_LENGTH];  // 蛇身体各节点
    int length;                    // 蛇的长度
    Direction direction;           // 蛇的移动方向
} Snake;

/**
 * 初始化游戏窗口
 * @参数 width: 游戏区域宽度（格子数）
 * @参数 height: 游戏区域高度（格子数）
 * @参数 title: 窗口标题
 * @返回值: 成功返回true，失败返回false
 */
bool init_game_window(int width, int height, const char* title);

/**
 * 清空整个游戏区域
 */
void clear_screen();

/**
 * 在指定位置绘制一个方块（用于食物、障碍物等）
 * @参数 x, y: 坐标位置
 * @参数 symbol: 方块符号
 */
void draw_block(int x, int y, Symbol symbol);

/**
 * 绘制整条蛇
 * @参数 snake: 蛇的结构体指针
 */
void draw_snake(const Snake* snake);

/**
 * 在指定位置绘制食物
 * @参数 x, y: 食物坐标
 */
void draw_food(int x, int y);

/**
 * 显示游戏分数
 * @参数 score: 当前分数
 */
void draw_score(int score);

/**
 * 刷新屏幕显示（双缓冲）
 */
void refresh_screen(void);

/**
 * 获取键盘输入
 * @返回值: 返回按下的方向键，如果没有按键返回-1
 */
int get_key_input(void);

/**
 * 延时函数，控制游戏速度
 * @参数 milliseconds: 延时毫秒数
 */
void game_delay(int milliseconds);

/**
 * 检查两个点是否相等
 * @参数 p1, p2: 要比较的两个点
 * @返回值: 相等返回true，否则返回false
 */
bool point_equals(Point p1, Point p2);

/**
 * 检查点是否在有效范围内
 * @参数 point: 要检查的点
 * @参数 width, height: 游戏区域尺寸
 * @返回值: 在范围内返回true，否则返回false
 */
bool point_in_bounds(Point point, int width, int height);

/**
 * 清理资源并关闭游戏窗口
 */
void cleanup_game(void);

/**
 * 创建一个新的点
 * @参数 x, y: 坐标
 * @返回值: 新创建的点
 */
Point make_point(int x, int y);

/**
 * 初始化蛇
 * @参数 snake: 蛇的结构体指针
 * @参数 start_x, start_y: 蛇头初始位置
 * @参数 initial_length: 蛇的初始长度
 */
void init_snake(Snake* snake, int start_x, int start_y, int initial_length);

/**
 * 根据方向移动蛇头
 * @参数 head: 当前蛇头位置
 * @参数 direction: 移动方向
 * @返回值: 新的蛇头位置
 */
Point move_point(Point head, Direction direction);

/**
 * 检查蛇是否撞到自己
 * @参数 snake: 蛇的结构体指针
 * @返回值: 撞到返回true，否则返回false
 */
bool snake_hit_itself(const Snake* snake);

/**
 * 在随机位置生成食物（注意避免食物生成在蛇身上）
 * @参数 width, height: 游戏区域尺寸
 * @参数 snake: 蛇的结构体
 * @返回值: 食物的位置
 */
Point generate_food(int width, int height, const Snake* snake);

/**
 * 检查方向是否相反
 * @参数 dir1, dir2: 两个方向
 * @返回值: 相反返回true，否则返回false
 */
bool directions_opposite(Direction dir1, Direction dir2);

#endif // SNAKE_GRAPHICS_H