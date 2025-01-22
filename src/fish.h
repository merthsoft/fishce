#ifndef _FISH_H_
#define _FISH_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define APPVAR_NOT_FOUND 255

typedef enum FishDirection { fd_Right, fd_Left } FishDirection;

typedef struct Fish {
    FishDirection   Direction;
    uint8_t         Color;
    uint8_t         Width;
    uint8_t         Height;
    float           X;
    float           Y;
    float           Speed;
} Fish;

void Fish_Generate(Fish* fish);
void Fish_Update(Fish* fish);

uint8_t Fish_Write(Fish* fishes, uint8_t numFishes, uint8_t backgroundColor);
uint8_t Fish_Read(Fish* fishes, uint8_t* numFishes, uint8_t* backgroundColor);

#ifdef __cplusplus
}
#endif

#endif