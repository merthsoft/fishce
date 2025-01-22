#include <fileioc.h>
#include <graphx.h>

#include <sys/util.h>

#include "fish.h"

#define APPVAR_NAME             "[FISH"
#define HEADER_LENGTH           5
#define VERSION_FIELD_LENGTH    1
#define NUM_FISH_FIELD_LENGTH   1
#define BG_COLOR_FIELD_LENGTH   1
#define VERSION_NUM             11

#define MAX_WIDTH 20
#define MIN_WIDTH 4

#define MAX_HEIGHT 20
#define MIN_HEIGHT 4

#define MARGIN 5

#define MAX_SPEED 4.0f
#define MIN_SPEED 0.5f

#define MAX_SPOOKED_SPEED 9.0f

#define SPOOK_CHANCE 666

#define BIG_FISH_WIDTH  50
#define BIG_FISH_HEIGHT (GFX_LCD_HEIGHT / 2 - 4)

static inline bool shouldRemove(Fish* fish)
{
    return fish->X < -(fish->Width + MARGIN)
        || fish->X > (fish->Width + MARGIN + GFX_LCD_WIDTH);
}

#define randPercent() (randInt(0, 100)/100.0f)

static inline void generate(Fish* fish) 
{
    fish->Direction = random() % 2;
    fish->Color = randInt(0, 255);

    if (!randPercent()) 
    {
        fish->Width = randInt(MAX_WIDTH, BIG_FISH_WIDTH);
        fish->Height = randInt(MAX_HEIGHT*2, BIG_FISH_HEIGHT);
        
        fish->Y = GFX_LCD_HEIGHT / 2;
        fish->X = fish->Direction == fd_Right ? -MARGIN : (GFX_LCD_WIDTH + fish->Width + MARGIN);
        fish->Speed = randPercent()*MAX_SPEED*3 + MIN_SPEED;
    } else 
    {
        fish->Width = randInt(MIN_WIDTH, MAX_WIDTH);
        fish->Height = randInt(MIN_HEIGHT, MAX_HEIGHT);
        
        fish->Y = randInt(fish->Height * 2 + 1, GFX_LCD_HEIGHT - fish->Height * 2 - 1);
        fish->X = fish->Direction == fd_Right ? -MARGIN : (GFX_LCD_WIDTH + fish->Width + MARGIN);
        fish->Speed = randPercent()*MAX_SPEED + MIN_SPEED;
    }
}

void Fish_Generate(Fish* fish)
{
    generate(fish);
}

void Fish_Update(Fish *fish)
{
    if (!randInt(0, SPOOK_CHANCE))
    {
        fish->Direction = !fish->Direction;
        fish->Speed *= 2.0f;

        if (fish->Speed > MAX_SPOOKED_SPEED)
            fish->Speed = MAX_SPOOKED_SPEED;
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
        generate(fish);
}

bool strnequ(char* src, char* dst, uint8_t len)
{
    for (uint8_t i = 0; i < len; i++)
    {
        if (src[i] != dst[i])
            return false;
    }

    return true;
}

uint8_t Fish_Write(Fish* fishes, uint8_t numFishes, uint8_t backgroundColor)
{
    uint8_t var = ti_Open(APPVAR_NAME, "w+");
    if (var == 0)
    {
        ti_Close(var);
        ti_Delete(APPVAR_NAME);
        return 1;
    }

    if (ti_Write(APPVAR_NAME, sizeof(uint8_t), HEADER_LENGTH, var) != HEADER_LENGTH)
    {
        ti_Close(var);
        ti_Delete(APPVAR_NAME);
        return 2;
    }

    uint8_t versionWrite = VERSION_NUM;
    if (ti_Write(&versionWrite, sizeof(uint8_t), VERSION_FIELD_LENGTH, var) != VERSION_FIELD_LENGTH)
    {
        ti_Close(var);
        ti_Delete(APPVAR_NAME);
        return 3;
    }

    if (ti_Write(&backgroundColor, sizeof(uint8_t), BG_COLOR_FIELD_LENGTH, var) != BG_COLOR_FIELD_LENGTH)
    {
        ti_Close(var);
        ti_Delete(APPVAR_NAME);
        return 4;
    }

    if (ti_Write(&numFishes, sizeof(uint8_t), NUM_FISH_FIELD_LENGTH, var) != NUM_FISH_FIELD_LENGTH)
    {
        ti_Close(var);
        ti_Delete(APPVAR_NAME);
        return 5;
    }

    if (ti_Write(fishes, sizeof(Fish), numFishes, var) != numFishes)
    {
        ti_Close(var);
        ti_Delete(APPVAR_NAME);
        return 6;
    }

    ti_SetArchiveStatus(true, var);
    ti_Close(var);
    return 0;
}

uint8_t Fish_Read(Fish* fishes, uint8_t* numFishes, uint8_t* backgroundColor)
{
    uint8_t var = ti_Open(APPVAR_NAME, "r");
    if (var == 0)
    {
        ti_Close(var);
        return APPVAR_NOT_FOUND;
    }

    if (ti_GetSize(var) <= HEADER_LENGTH + VERSION_FIELD_LENGTH + BG_COLOR_FIELD_LENGTH + NUM_FISH_FIELD_LENGTH)
    {
        ti_Close(var);
        return 1;
    }

    char buffer[HEADER_LENGTH];
    if (ti_Read(buffer, sizeof(char), HEADER_LENGTH, var) != HEADER_LENGTH)
    {
        ti_Close(var);
        return 2;
    }

    if (!strnequ(buffer, APPVAR_NAME, HEADER_LENGTH))
    {
        ti_Close(var);
        return 3;
    }
    
    uint8_t versionRead;
    if (ti_Read(&versionRead, sizeof(uint8_t), VERSION_FIELD_LENGTH, var) != VERSION_FIELD_LENGTH)
    {
        ti_Close(var);
        return 4;
    }
    
    if (versionRead != VERSION_NUM)
    {
        ti_Close(var);
        return 5;
    }

    uint8_t backgroundColorRead;
    if (ti_Read(&backgroundColorRead, sizeof(uint8_t), BG_COLOR_FIELD_LENGTH, var) != BG_COLOR_FIELD_LENGTH)
    {
        ti_Close(var);
        return 6;
    }

    uint8_t numFishRead;
    if (ti_Read(&numFishRead, sizeof(uint8_t), NUM_FISH_FIELD_LENGTH, var) != NUM_FISH_FIELD_LENGTH)
    {
        ti_Close(var);
        return 6;
    }

    uint8_t actualAmountOfFishToRead = numFishRead <= (*numFishes) ? numFishRead : *numFishes;
    
    if (ti_Read(fishes, sizeof(Fish), numFishRead, var) != actualAmountOfFishToRead)
    {
        ti_Close(var);
        return 7;
    }
    
    *backgroundColor = backgroundColorRead;
    *numFishes = numFishRead;
    
    ti_SetArchiveStatus(true, var);
    ti_Close(var);
    return 0;
}