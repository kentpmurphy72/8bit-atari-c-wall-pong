#include "game.h"

void game_init(Game* g)
{
    g->paddle = 45;
    g->x = 12 * SCALE;
    g->y = 53 * SCALE;
    g->vx = 20;
    g->vy = 12;
    g->score = 0;
    g->lives = 3;
    g->playing = 0;
    g->fire_down = 0;
}

unsigned char game_step(Game* g, unsigned char up, unsigned char down, unsigned char fire)
{
    unsigned char event = EVENT_NONE;
    int old_x, hit;
    unsigned char pressed = fire && !g->fire_down;
    g->fire_down = fire;
    if (!g->lives) {
        if (pressed) {
            game_init(g);
            g->fire_down = 1;
        }
        return EVENT_NONE;
    }
    if (up && g->paddle > TOP) g->paddle -= 2;
    if (down && g->paddle < BOTTOM - PADDLE_HEIGHT) g->paddle += 2;
    if (g->paddle < TOP) g->paddle = TOP;
    if (g->paddle > BOTTOM - PADDLE_HEIGHT) g->paddle = BOTTOM - PADDLE_HEIGHT;
    if (!g->playing) {
        g->x = 12 * SCALE;
        g->y = (g->paddle + PADDLE_HEIGHT / 2) * SCALE;
        if (pressed) {
            g->playing = 1;
            g->vx = 20;
            g->vy = 12;
        }
        return EVENT_NONE;
    }
    old_x = g->x;
    g->x += g->vx;
    g->y += g->vy;
    if (g->y < TOP * SCALE) {
        g->y = 2 * TOP * SCALE - g->y;
        g->vy = -g->vy;
        event = EVENT_BOUNCE;
    }
    if (g->y > (BOTTOM - 2) * SCALE) {
        g->y = 2 * (BOTTOM - 2) * SCALE - g->y;
        g->vy = -g->vy;
        event = EVENT_BOUNCE;
    }
    if (g->x >= (WALL - 2) * SCALE) {
        g->x = 2 * (WALL - 2) * SCALE - g->x;
        g->vx = -g->vx;
        if (g->score < 999) ++g->score;
        event = EVENT_POINT;
    }
    if (g->vx < 0 && old_x >= (PADDLE_X + 2) * SCALE &&
        g->x <= (PADDLE_X + 2) * SCALE &&
        g->y + 2 * SCALE > g->paddle * SCALE &&
        g->y < (g->paddle + PADDLE_HEIGHT) * SCALE) {
        g->x = 2 * (PADDLE_X + 2) * SCALE - g->x;
        g->vx = -g->vx;
        if (g->vx < 40) ++g->vx;
        hit = g->y / SCALE + 1 - (g->paddle + PADDLE_HEIGHT / 2);
        g->vy = hit * 3;
        if (g->vy == 0) g->vy = 5;
        event = EVENT_BOUNCE;
    }
    if (g->x < 0) {
        --g->lives;
        g->playing = 0;
        event = EVENT_MISS;
    }
    return event;
}
