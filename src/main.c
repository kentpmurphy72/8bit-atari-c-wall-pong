#include <atari.h>
#include <conio.h>
#include <string.h>
#include "game.h"
#define REG(a) (*(volatile unsigned char*)(a))
/* Align the bitmap inside one 4K bank: ANTIC's LMS counter wraps at 4K. */
static unsigned char bitmap_store[7936];
static unsigned char dl_store[1152];
static unsigned char* screen;
static unsigned char* dl;
static unsigned int rows[96];
static const unsigned char digits[10][5] = {
 {7,5,5,5,7},{2,6,2,2,7},{7,1,7,4,7},{7,1,7,1,7},{5,5,7,1,1},
 {7,4,7,1,7},{7,4,7,5,7},{7,1,1,1,1},{7,5,7,5,7},{7,5,7,1,7}
};
static void pixel(unsigned char x, unsigned char y, unsigned char c)
{
    unsigned char shift = (3 - (x & 3)) * 2;
    unsigned char* p = (unsigned char*)(rows[y] + (x >> 2));
    *p = (*p & ~(3 << shift)) | (c << shift);
}
static void rect(unsigned char x, unsigned char y, unsigned char w, unsigned char h, unsigned char c)
{
    unsigned char i,j;
    for (j = y; j < y+h; ++j)
        for (i = x; i < x+w; ++i) pixel(i,j,c);
}
static void number(unsigned int value, unsigned char x, unsigned char count)
{
    unsigned char d,r,c;
    while (count--) {
        d = value % 10;
        value /= 10;
        for (r=0;r<5;++r) for(c=0;c<3;++c)
            pixel(x+c,3+r,(digits[d][r] & (4 >> c)) ? 1 : 0);
        x -= 5;
    }
}
static void video(void)
{
    unsigned char i;
    unsigned int addr;
    screen = (unsigned char*)(((unsigned int)bitmap_store + 4095) & 0xF000);
    dl = (unsigned char*)(((unsigned int)dl_store + 1023) & 0xFC00);
    memset(screen,0,3840);
    dl[0]=0x70; dl[1]=0x70; dl[2]=0x70;
    dl[3]=0x4D;
    addr=(unsigned int)screen;
    dl[4]=addr & 255; dl[5]=addr >> 8;
    for(i=0;i<95;++i) dl[6+i]=0x0D;
    dl[101]=0x41;
    addr=(unsigned int)dl;
    dl[102]=addr & 255; dl[103]=addr >> 8;
    for(i=0;i<96;++i) rows[i]=(unsigned int)screen + (unsigned int)i*40;
    REG(559)=0;
    *(volatile unsigned int*)560=(unsigned int)dl;
    REG(708)=0x0E; REG(709)=0xB8; REG(710)=0x38; REG(712)=0;
    REG(752)=1;
    REG(559)=0x22;
}
static void frame(void)
{
    unsigned char t=REG(20);
    while(REG(20)==t) { }
}
int main(void)
{
    Game g;
    unsigned char old_p, old_x, old_y, stick, event, tone=0;
    unsigned int saved_dl=*(volatile unsigned int*)560;
    unsigned char saved_dma=REG(559), saved_cursor=REG(752);
    unsigned char colors[5],i;
    for(i=0;i<5;++i) colors[i]=REG(708+i);
    game_init(&g);
    video();
    rect(0,10,160,1,1); rect(WALL,TOP,2,BOTTOM-TOP,2);
    rect(0,BOTTOM,160,1,1);
    REG(0xD208)=0; REG(0xD20F)=3;
    while(1) {
        frame();
        REG(77)=0; /* Prevent the OS attract mode from changing colors. */
        if(kbhit() && cgetc()==27) break;
        old_p=g.paddle; old_x=g.x/SCALE; old_y=g.y/SCALE;
        rect(PADDLE_X,old_p,2,PADDLE_HEIGHT,0);
        if(g.x>=0) rect(old_x,old_y,2,2,0);
        stick=REG(632);
        event=game_step(&g,!(stick&1),!(stick&2),!REG(644));
        rect(PADDLE_X,g.paddle,2,PADDLE_HEIGHT,1);
        if(g.lives && g.x>=0) rect(g.x/SCALE,g.y/SCALE,2,2,3);
        number(g.score,74,3); number(g.lives,145,1);
        /* The serve indicator disappears in play; red means game over. */
        rect(82,4,3,3,g.lives ? (g.playing ? 0 : 2) : 3);
        if(event) {
            REG(0xD200)=event==EVENT_MISS ? 180 : (event==EVENT_POINT ? 35 : 70);
            REG(0xD201)=0xA8; tone=event==EVENT_MISS ? 12 : 3;
        } else if(tone && !--tone) REG(0xD201)=0;
    }
    REG(0xD201)=0;
    REG(559)=0;
    *(volatile unsigned int*)560=saved_dl;
    for(i=0;i<5;++i) REG(708+i)=colors[i];
    REG(752)=saved_cursor; REG(559)=saved_dma;
    return 0;
}
