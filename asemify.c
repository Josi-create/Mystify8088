
#include <conio.h>
#include <dos.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned char byte;
#define SET_MODE 0x00
#define VIDEO_INT 0x10
#define CGA_MODE 0x04 
#define TEXT_MODE 0x03
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

#define color1 1
#define color2 3
#define background_color 0
#define speed 4
#define MAX_LINES 100

extern void cga_line(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1, unsigned char colour);

int color = 0x00;
int i;
int x1, y1, x2, y2, x1b, y1b, x2b, y2b;         
int xv1, yv1, xv2, yv2, xv1b, yv1b, xv2b, yv2b; 
byte far *CGAeven = (byte far *)MK_FP(0xB800, 0x0000);
byte far *CGAodd = (byte far *)MK_FP(0xB800, 0x2000);

typedef struct
{
    int x1, y1, x2, y2;
} Line;

Line lines[MAX_LINES]; 
int current_line = 0;

void clear_line(int x0, int y0, int x1, int y1)
{
    cga_line(x0, y0, x1, y1, background_color);
}

void clear_lines_buffer()
{
    for (i = 0; i < MAX_LINES; i++)
    {
        clear_line(lines[i].x1, lines[i].y1, lines[i].x2, lines[i].y2);
    }
    current_line = 0;
}

void mystify_init()
{
    x1 = rand() % SCREEN_WIDTH;
    y1 = rand() % SCREEN_HEIGHT;
    x2 = rand() % SCREEN_WIDTH;
    y2 = rand() % SCREEN_HEIGHT;
    x1b = rand() % SCREEN_WIDTH;
    y1b = rand() % SCREEN_HEIGHT;
    x2b = rand() % SCREEN_WIDTH;
    y2b = rand() % SCREEN_HEIGHT;

    xv1 = 1 + rand() % speed;
    yv1 = 1 + rand() % speed;
    xv2 = 1 + rand() % speed;
    yv2 = 1 + rand() % speed;

    xv1b = 1 + rand() % speed;
    yv1b = 1 + rand() % speed;
    xv2b = 1 + rand() % speed;
    yv2b = 1 + rand() % speed;
}

void add_line(int x1, int y1, int x2, int y2, int color)
{

    if (current_line >= MAX_LINES)
    {
        clear_line(lines[current_line % MAX_LINES].x1, lines[current_line % MAX_LINES].y1,
                   lines[current_line % MAX_LINES].x2, lines[current_line % MAX_LINES].y2);
    }

    lines[current_line % MAX_LINES].x1 = x1;
    lines[current_line % MAX_LINES].y1 = y1;
    lines[current_line % MAX_LINES].x2 = x2;
    lines[current_line % MAX_LINES].y2 = y2;
    current_line++;

    cga_line(x1, y1, x2, y2, color);
}

void mystify()
{
    x1 += xv1;
    y1 += yv1;
    x2 += xv2;
    y2 += yv2;
    x1b += xv1b;
    y1b += yv1b;
    x2b += xv2b;
    y2b += yv2b;

    if (x1 < speed || x1 >= SCREEN_WIDTH-speed)
        xv1 = -xv1; 
    if (y1 < speed || y1 >= SCREEN_HEIGHT-speed)
        yv1 = -yv1;
    if (x2 < speed || x2 >= SCREEN_WIDTH-speed)
        xv2 = -xv2;
    if (y2 < speed || y2 >= SCREEN_HEIGHT-speed)
        yv2 = -yv2;
    if (x1b < speed || x1b >= SCREEN_WIDTH-speed)
        xv1b = -xv1b;
    if (y1b < speed || y1b >= SCREEN_HEIGHT-speed)
        yv1b = -yv1b;
    if (x2b < speed || x2b >= SCREEN_WIDTH-speed)
        xv2b = -xv2b;
    if (y2b < speed || y2b >= SCREEN_HEIGHT-speed)
        yv2b = -yv2b;

    add_line(x1, y1, x2, y2, color1);
    add_line(x1b, y1b, x2b, y2b, color2);
}

void clrscr()
{
    printf("\033[30m");  
    printf("\033[2J");   
    printf("\033[1;1H"); 
    printf("\033[0m");   
};

void set_mode(byte mode)
{
    union REGS regs;
    regs.h.ah = SET_MODE;
    regs.h.al = mode;
    int86(VIDEO_INT, &regs, &regs);
}

int main()
{
    char kc = 0;

    set_mode(CGA_MODE);
    clrscr();
    
    mystify_init();


    while (kc != 0x1b)
    {

        if (kbhit())
        {
            kc = getch();
            printf("Key pressed: %02x\n", kc);

            if (kc == 0x20)
            {
                mystify_init();
                clear_lines_buffer();
                clrscr();
            }
        }

        /* Mystify my life */
        mystify(); 
  }

    set_mode(TEXT_MODE);

    return 0;
};
