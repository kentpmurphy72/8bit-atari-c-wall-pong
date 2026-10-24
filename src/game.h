#ifndef GAME_H
#define GAME_H
#define SCALE 16
#define PADDLE_X 8
#define PADDLE_HEIGHT 16
#define TOP 12
#define BOTTOM 94
#define WALL 155
#define EVENT_NONE 0
#define EVENT_BOUNCE 1
#define EVENT_POINT 2
#define EVENT_MISS 3
typedef struct {
    int x, y, vx, vy;
    unsigned int score;
    unsigned char paddle, lives, playing, fire_down;
} Game;
void game_init(Game* g);
unsigned char game_step(Game* g, unsigned char up, unsigned char down, unsigned char fire);
#endif
