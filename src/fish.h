#ifndef _FISH_H_
#define _FISH_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_WIDTH 20
#define MIN_WIDTH 4

#define MAX_HEIGHT 20
#define MIN_HEIGHT 4

#define MARGIN 10

#define MAXSPEED 4.0f
#define MINSPEED 0.5f

#define MAXSPOOKEDSPEED 9.0f

#define SPOOKCHANCE 666

typedef enum FishDirection { fd_Right, fd_Left } FishDirection;

typedef struct Fish {
    bool            Visible;
    uint8_t         Color;
    FishDirection   Direction;
    float           X;
    float           Y;
    float           Speed;
    float           Width;
    float           Height;
} Fish;

void Fish_Generate(Fish* fish);
void Fish_Update(Fish* fish);

#ifdef __cplusplus
}
#endif

#endif