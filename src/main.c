#include <graphx.h>
#include <keypadc.h>
#include <stdlib.h>

#include <sys/rtc.h>

#include "fish.h"
#include "render.h"

void begin(Fish* fishes, const uint8_t maxFish);
bool step(Fish* fishes, const uint8_t maxFish);
void draw(const uint8_t backgroundColor, const Fish* fishes, const uint8_t maxFish);

#define MAXFISH 15

int main(void)
{
    Fish Fishes[MAXFISH];
    uint8_t backgroundColor = 24;

    begin(Fishes, MAXFISH);

    gfx_Begin();
    gfx_SetDrawBuffer();

    while (step(Fishes, MAXFISH))
    {
        draw(backgroundColor, Fishes, MAXFISH);
        gfx_SwapDraw();
    }

    gfx_End();

    return 0;
}

void begin(Fish* fishes, const uint8_t maxFish)
{
    srand(rtc_Time());
    
    for (uint8_t i = 0; i < maxFish; i++)
        Fish_Generate(&fishes[i]);
}

bool step(Fish* fishes, const uint8_t maxFish)
{
    for (uint8_t i = 0; i < maxFish; i++)
        Fish_Update(&fishes[i]);

    kb_Scan();
    return !(kb_IsDown(kb_KeyDel) | kb_IsDown(kb_KeyClear) | kb_IsDown(kb_KeyMode) | kb_IsDown(kb_KeyEnter));
}

void draw(const uint8_t backgroundColor, const Fish* fishes, const uint8_t maxFish)
{
    gfx_FillScreen(backgroundColor);
    for (uint8_t i = 0; i < maxFish; i++)
        Render_Fish(&fishes[i]);
}
