#ifndef TILEMAP_H
#define TILEMAP_H
#include "raylib.h"

void InitTilemap(void);
void DrawTilemap(void);
int GetMapWidth(void);
int GetMapHeight(void);
void CloseTilemap(void);
Texture2D GetTilemapTexture(void);

#endif