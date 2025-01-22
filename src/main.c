#include <graphx.h>
#include <keypadc.h>
#include <stdio.h>
#include <time.h>

#include <sys/rtc.h>

#ifdef DEBUG
#include <debug.h>
#include <stdlib.h>
#endif

#include "fish.h"
#include "render.h"

#define MAX_FISH 15
#define DEFAULT_BACKGROUND_COLOR 24

static inline void begin(Fish* fishes, uint8_t maxFish, uint8_t* backgroundColor)
{
    srand(rtc_Time());
    
    uint8_t readResult = Fish_Read(fishes, &maxFish, backgroundColor);
    if (readResult)
    {
#if DEBUG
        if (readResult != APPVAR_NOT_FOUND)
            dbg_printf("Error reading fish appvar: %d\n", readResult);
#endif
        for (uint8_t i = 0; i < maxFish; i++)
            Fish_Generate(&fishes[i]);
    }
}

static inline bool step(Fish* fishes, const uint8_t maxFish, uint8_t* backgroundColor)
{
    for (uint8_t i = 0; i < maxFish; i += 1)
        Fish_Update(&fishes[i]);

    kb_Scan();
    uint8_t bgc = *backgroundColor;
    if (kb_IsDown(kb_KeyAdd))
        *backgroundColor = bgc + 1;
    else if (kb_IsDown(kb_KeySub))
        *backgroundColor = bgc - 1;
    else if (kb_IsDown(kb_KeyMul))
        *backgroundColor = bgc + 10;
    else if (kb_IsDown(kb_KeyDiv))
        *backgroundColor = bgc - 10;
    else if (kb_IsDown(kb_KeyChs))
        *backgroundColor = 0;
    else if (kb_IsDown(kb_Key0))
        *backgroundColor = DEFAULT_BACKGROUND_COLOR;

    return !(kb_IsDown(kb_KeyDel) | kb_IsDown(kb_KeyClear) | kb_IsDown(kb_KeyMode) | kb_IsDown(kb_KeyEnter));
}

static inline void draw(const Fish* fishes, const uint8_t maxFish, const uint8_t backgroundColor)
{
    gfx_FillScreen(backgroundColor);
    for (uint8_t i = 0; i < maxFish; i += 1)
        Render_Fish(&fishes[i]);
}

int main(void)
{
    Fish Fishes[MAX_FISH];
    uint8_t backgroundColor = DEFAULT_BACKGROUND_COLOR;

    begin(Fishes, MAX_FISH, &backgroundColor);

    gfx_Begin();
    gfx_SetDrawBuffer();

#ifdef DEBUG
    clock_t time = clock();
    uint24_t framecount = 0;
    float fps = 0.0f;
    char buffer[11] = "FPS: XX.XX";
#endif

    while (step(Fishes, MAX_FISH, &backgroundColor))
    {
#ifdef DEBUG
        framecount++;
        if (clock() - time >= CLOCKS_PER_SEC)
        {
            fps = (fps + framecount) / 2.0f;
            framecount = 0;
            time = clock();
        }
#endif
        draw(Fishes, MAX_FISH, backgroundColor);

        if (kb_IsDown(kb_KeyDecPnt))
        {
            gfx_SetTextFGColor(backgroundColor == 0 ? DEFAULT_BACKGROUND_COLOR : ~backgroundColor);
            gfx_SetTextXY(2, GFX_LCD_HEIGHT - 10);
            gfx_PrintUInt(backgroundColor, 1);
        }

#ifdef DEBUG
        gfx_SetTextFGColor(backgroundColor == 0 ? DEFAULT_BACKGROUND_COLOR : ~backgroundColor);
        sprintf(buffer, "FPS: %.2f", fps);
        gfx_PrintStringXY(buffer, 2, 2);
#endif

        gfx_SwapDraw();
    }

    gfx_End();

    uint8_t writeResult = Fish_Write(Fishes, MAX_FISH, backgroundColor);
#if DEBUG
    if (writeResult)
        dbg_printf("Error writing fish appvar: %d\n", writeResult);
#endif

    return writeResult;
}