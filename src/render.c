#include <graphx.h>

#include "render.h"

#define TAIL_MARGIN 3
#define EYE_MARGIN 4

static inline void drawBody(const Fish* fish)
{
    gfx_SetColor(fish->Color);
    gfx_FillEllipse(fish->X, fish->Y, fish->Width, fish->Height);
}

static inline void drawEye(const Fish* fish)
{
    uint24_t eyeX;
    uint8_t eyeY;

    switch (fish->Direction) {
        case fd_Left:
            eyeX = fish->X - fish->Width + fish->Width / 4;
            eyeY = fish->Y - EYE_MARGIN;
            break;
        default:
        case fd_Right:
            eyeX = fish->X + fish->Width - fish->Width / 4;
            eyeY = fish->Y - EYE_MARGIN;
            break;
    }

    gfx_SetColor(255);
    gfx_SetPixel(eyeX - 1, eyeY);
    gfx_SetPixel(eyeX + 1, eyeY);
    gfx_SetPixel(eyeX, eyeY - 1);
    gfx_SetPixel(eyeX, eyeY + 1);
    gfx_SetColor(0);
    gfx_SetPixel(eyeX, eyeY);
}

static inline void drawMouth(const Fish* fish)
{
    uint24_t mounthX;

    switch (fish->Direction) {
        case fd_Left:
            mounthX = fish->X - fish->Width;
            break;
        default:
        case fd_Right:
            mounthX = fish->X + fish->Width - fish->Width / 4;
            break;
    }

    gfx_SetColor(0);
    gfx_HorizLine(mounthX, fish->Y, fish->Width / 4);
}

static inline void drawTail(const Fish* fish)
{
    float x1, x2, x3;
    float y1, y2, y3;

    switch (fish->Direction)
    {
        case fd_Left:
            x1 = fish->X + fish->Width - TAIL_MARGIN;
            y1 = fish->Y;

            x2 = fish->X + fish->Width + fish->Width / 4;
            y2 = fish->Y - fish->Height / 2;


            x3 = fish->X + fish->Width + fish->Width / 4;
            y3 = fish->Y + fish->Height / 2;
        break;
        default:
        case fd_Right:
            x1 = fish->X - fish->Width + TAIL_MARGIN;
            y1 = fish->Y;
            
            x2 = fish->X - fish->Width - fish->Width / 4;
            y2 = fish->Y - fish->Height / 2;
            
            x3 = fish->X - fish->Width - fish->Width / 4;
            y3 = fish->Y + fish->Height / 2;
        break;
    }

    gfx_SetColor(fish->Color);
    gfx_FillTriangle(x1, y1, x2, y2, x3, y3);
}

void Render_Fish(const Fish *fish)
{
    drawBody(fish);
    drawEye(fish);
    drawMouth(fish);
    drawTail(fish);
}