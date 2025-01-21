#include <graphx.h>

#include <sys/util.h>

#include "fish.h"

bool shouldRemove(Fish* fish)
{
    return fish->X < -(fish->Width + 2*MARGIN)
        || fish->X > (fish->Width + 2*MARGIN + GFX_LCD_WIDTH);
}

#define randFloat() (randInt(0, 100)/100.0f)

void Fish_Generate(Fish *fish)
{
    fish->Direction = random() % 2;
    fish->Width = randInt(MIN_WIDTH, MAX_WIDTH);
    fish->Height = randInt(MIN_HEIGHT, MAX_HEIGHT);
    
    fish->Y = randInt(1, GFX_LCD_HEIGHT - (uint24_t)fish->Height / 2 - 1);
    fish->X = fish->Direction == fd_Right ? -MARGIN : (GFX_LCD_WIDTH + fish->Width + MARGIN);
    
    fish->Speed = randFloat()*MAXSPEED + MINSPEED;

    fish->Visible = true;
    fish->Color = randInt(0, 255);
}

void Fish_Update(Fish *fish)
{
    if (!randInt(0, SPOOKCHANCE))
    {
        fish->Direction = !fish->Direction;
        fish->Speed *= 2.0f;

        if (fish->Speed > MAXSPOOKEDSPEED)
            fish->Speed = MAXSPOOKEDSPEED;
    }

    switch (fish->Direction) {
        case fd_Left:
            fish->X -= fish->Speed;
            break;
        case fd_Right:
            fish->X += fish->Speed;
            break;
            break;
    }

    if (shouldRemove(fish))
        Fish_Generate(fish);
}
