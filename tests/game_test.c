#include <assert.h>
#include <stdio.h>
#include "game.h"
static unsigned char step(Game* g) { return game_step(g,0,0,0); }
int main(void)
{
    Game g;
    unsigned int i;
    game_init(&g);
    assert(g.lives==3 && !g.playing && g.score==0);
    game_step(&g,0,0,1);
    assert(g.playing && g.vx>0);
    g.x=(WALL-2)*SCALE-4; g.vx=20;
    assert(step(&g)==EVENT_POINT && g.vx<0 && g.score==1);
    g.y=TOP*SCALE+2; g.vy=-12;
    assert(step(&g)==EVENT_BOUNCE && g.vy>0 && g.y>=TOP*SCALE);
    g.y=(BOTTOM-2)*SCALE-2; g.vy=12;
    assert(step(&g)==EVENT_BOUNCE && g.vy<0);
    g.x=(PADDLE_X+2)*SCALE+4; g.y=(g.paddle+2)*SCALE; g.vx=-20; g.vy=0;
    assert(step(&g)==EVENT_BOUNCE && g.vx>0 && g.vy<0);
    g.x=(PADDLE_X+2)*SCALE+4; g.y=(BOTTOM-3)*SCALE; g.vx=-20; g.vy=0;
    step(&g); assert(g.vx<0);
    for(i=0;i<3;++i) {
        g.playing=1; g.x=1; g.vx=-20;
        assert(step(&g)==EVENT_MISS);
        assert(g.lives==2-i && !g.playing);
    }
    game_step(&g,0,0,1);
    assert(g.lives==3 && g.score==0 && !g.playing);
    game_step(&g,0,0,1); assert(!g.playing);
    step(&g); game_step(&g,0,0,1); assert(g.playing);
    for(i=0;i<100;++i) game_step(&g,1,0,0);
    assert(g.paddle==TOP);
    for(i=0;i<100;++i) game_step(&g,0,1,0);
    assert(g.paddle==BOTTOM-PADDLE_HEIGHT);
    /* Follow the ball through a long rally: exercise repeated collisions. */
    game_init(&g); game_step(&g,0,0,1);
    for(i=0;i<10000;++i) {
        int center=g.paddle+PADDLE_HEIGHT/2;
        game_step(&g,center>g.y/SCALE+1,center<g.y/SCALE+1,0);
        assert(g.lives==3 && g.playing);
        assert(g.y>=TOP*SCALE && g.y<=(BOTTOM-2)*SCALE);
    }
    assert(g.score>20);
    puts("PASS: serve, walls, scoring, paddle deflection, misses, restart, bounds, long rally");
    return 0;
}
