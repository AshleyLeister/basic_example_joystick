/*
 * Graphics.c
 *
 *  Created on: Dec 30, 2019
 *      Author: Matthew Zhong
 */

#include <HAL/Graphics.h>

#include <HAL/HAL.h>
#include <HAL/Timer.h>
#include <application.h>

#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"

GFX GFX_construct(uint32_t defaultForeground, uint32_t defaultBackground)
{
    GFX gfx;

    gfx.defaultForeground = defaultForeground;
    gfx.defaultBackground = defaultBackground;

    Crystalfontz128x128_Init();
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    Graphics_initContext(&gfx.context, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);
    Graphics_setFont(&gfx.context, &g_sFontFixed6x8);

    GFX_resetColors(&gfx);
    GFX_clear(&gfx);

    return gfx;
}


void GFX_clear(GFX* gfx)
{
    Graphics_clearDisplay(&gfx->context);
}

void GFX_print(GFX* gfx, char* string, int row, int col)
{
    int yPosition = row * Graphics_getFontHeight(gfx->context.font);
    int xPosition = col * Graphics_getFontMaxWidth(gfx->context.font);

    Graphics_drawString(&gfx->context, (int8_t*) string, -1, xPosition, yPosition, OPAQUE_TEXT);
}



void GFX_drawSolidCircle(GFX* gfx, int x, int y, int radius)
{
    Graphics_fillCircle(&gfx->context, x, y, radius);
}

void GFX_drawHollowCircle(GFX* gfx, int x, int y, int radius)
{
    Graphics_drawCircle(&gfx->context, x, y, radius);
}

void GFX_removeSolidCircle(GFX* gfx, int x, int y, int radius)
{
    uint32_t oldForegroundColor = gfx->foreground;
    GFX_setForeground(gfx, gfx->background);
    GFX_drawSolidCircle(gfx, x, y, radius);
    GFX_setForeground(gfx, oldForegroundColor);
}

void GFX_removeHollowCircle(GFX* gfx, int x, int y, int radius)
{
    uint32_t oldForegroundColor = gfx->foreground;
    GFX_setForeground(gfx, gfx->background);
    GFX_drawHollowCircle(gfx, x, y, radius);
    GFX_setForeground(gfx, oldForegroundColor);
}
////////////////



void GFX_resetColors(GFX* gfx)
{
    gfx->foreground = gfx->defaultForeground;
    gfx->background = gfx->defaultBackground;

    Graphics_setForegroundColor(&gfx->context, gfx->defaultForeground);
    Graphics_setBackgroundColor(&gfx->context, gfx->defaultBackground);
}




void GFX_setForeground(GFX* gfx, uint32_t foreground)
{
    gfx->foreground = foreground;
    Graphics_setForegroundColor(&gfx->context, foreground);
}

void GFX_setBackground(GFX* gfx, uint32_t background)
{
    gfx->background = background;
    Graphics_setBackgroundColor(&gfx->context, background);
}




void make_5digit_NumString(unsigned int num, int8_t *string)
{
    string[0]= (        num  / 10000) +'0';
    string[1]= ((num%10000) / 1000) +'0';
    string[2]= ((num%1000) / 100) +'0';
    string[3]= ((num%100) / 10) +'0';
    string[4]= ((num%10) / 1) +'0';
    string[5]= 0;
}

void drawXY(GFX* gfx, unsigned int x, unsigned int y)
{
    int8_t string[6];

    Graphics_drawString(&gfx->context, (int8_t *)"x=", -1, 10, 5, true);
    make_5digit_NumString(x, string);
    Graphics_drawString(&gfx->context, string, -1, 30, 5, true);

    Graphics_drawString(&gfx->context, (int8_t *)"y=", -1, 10, 15, true);
    make_5digit_NumString(y, string);
    Graphics_drawString(&gfx->context, string, -1, 30, 15, true);
}


void draw_Base(Graphics_Context *g_sContext_p)
{
    Graphics_Rectangle R;
    R.xMin = 0;
    R.xMax = 127;
    R.yMin = 32;
    R.yMax = 96;

    Graphics_drawRectangle(g_sContext_p, &R);
    Graphics_fillCircle(g_sContext_p, 63, 63, 10);
    Graphics_drawString(g_sContext_p, (int8_t *)"circle move #:", -1, 10, 100, false);
    Graphics_drawString(g_sContext_p, (int8_t *)"000", -1, 10, 110, true);
}

void make_3digit_NumString(unsigned int num, char *string)
{
    string[0]= (num/100)+'0';
    string[1]= ((num%100) / 10) + '0';
    string[2]= (num%10)+'0';
    string[3] =0;

}


void MoveCircle(GFX* gfx, bool moveToLeft, bool moveToRight, bool moveToDown, bool moveToUp, Application*app)
{
    static unsigned int x = 63;
    static unsigned int y = 63;

    static unsigned int moveCount = 0;
    static unsigned int pollenCount = 0;
    char string[4];

    if ((moveToLeft && (x>20)) || (moveToRight && (x<110))||(moveToDown && (y>40)) || (moveToUp && (y<90)))
    {

        Graphics_setForegroundColor(&gfx->context, GRAPHICS_COLOR_BLUE);

        Graphics_fillCircle(&gfx->context, x, y, 10);//get rid of previous circle

        if (moveToLeft)//if boolean movetoleft is true
            x = x-10;

        if(moveToRight)//if boolean movetoright is true
            x = x+10;

        if (moveToDown)//if boolean movetodown is true
           y = y+10;

        if(moveToUp)//if boolean movetoup is true
           y = y-10;

        Graphics_setForegroundColor(&gfx->context, GRAPHICS_COLOR_YELLOW);//draw new circle in new location
        Graphics_fillCircle(&gfx->context, x, y, 10);//draw new circle in new location

        moveCount++;
        make_3digit_NumString(moveCount, string);//adds to move count
        Graphics_drawString(&gfx->context, (int8_t *) string, -1, 10, 110, true);



        if ((app->frameIndex==y) && (x==20))
                {
        pollenCount++;
        make_3digit_NumString(pollenCount, string);
        Graphics_drawString(&gfx->context, (int8_t *) string, -1, 30, 110, true);
        }
}

}



void InitFonts() {
    Crystalfontz128x128_Init();
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);
}


void InitGraphics(GFX* gfx) {

    Graphics_initContext(&gfx->context,
                         &g_sCrystalfontz128x128,
                         &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(&gfx->context, GRAPHICS_COLOR_YELLOW);
    Graphics_setBackgroundColor(&gfx->context, GRAPHICS_COLOR_BLUE);
    Graphics_setFont(&gfx->context, &g_sFontCmtt12);

    InitFonts();

    Graphics_clearDisplay(&gfx->context);
}
