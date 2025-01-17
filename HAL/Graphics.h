/*
 * Graphics.h
 *
 *  Created on: Dec 30, 2019
 *      Author: Matthew Zhong
 */

#ifndef API_GRAPHICS_H_
#define API_GRAPHICS_H_

#include <HAL/LcdDriver/Crystalfontz128x128_ST7735.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>

#define FG_COLOR GRAPHICS_COLOR_WHITE
#define BG_COLOR GRAPHICS_COLOR_BLACK

struct _GFX
{
    Graphics_Context context;
    uint32_t foreground;
    uint32_t background;
    uint32_t defaultForeground;
    uint32_t defaultBackground;
};
typedef struct _GFX GFX;



GFX GFX_construct(uint32_t defaultForeground, uint32_t defaultBackground);

void GFX_resetColors(GFX* gfx);
void GFX_clear(GFX* gfx);

void GFX_print(GFX* gfx, char* string, int row, int col);
void GFX_setForeground(GFX* gfx, uint32_t foreground);
void GFX_setBackground(GFX* gfx, uint32_t background);

void GFX_drawSolidCircle(GFX* gfx, int x, int y, int radius);
void GFX_drawHollowCircle(GFX* gfx, int x, int y, int radius);

void GFX_removeSolidCircle(GFX* gfx, int x, int y, int radius);
void GFX_removeHollowCircle(GFX* gfx, int x, int y, int radius);


void draw_Base(Graphics_Context *g_sContext_p);

void InitGraphics(GFX* gfx);
void drawXY(GFX* gfx, unsigned int x, unsigned int y);

#endif /* API_GRAPHICS_H_ */
